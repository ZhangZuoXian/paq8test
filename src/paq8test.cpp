#include<memory>
#include<unistd.h>
#include<fstream>
#include"Encoder.hpp"
#include"Shared.hpp"
#include<getopt.h>
#include<time.h>

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

// #define INFO

// 针对元数据的U32类型的读写
size_t frU32(FILE *f, U32 &data) {
    return fread(&data, sizeof(U32), 1, f);
}
size_t fwU32(FILE *f, U32 &data) {
    // printf("[META] write %u to meta data file\n", data);
    return fwrite(&data, sizeof(U32), 1, f);
}

int main(int argc,char **argv){
        
    // 模式选择：压缩、解压
    Mode mode(COMPRESS);
    mode = (strcmp(argv[1], "-d")) ? COMPRESS : DECOMPRESS;

    // 输入、输、元数据文件
    char filePath[128];
    char metaPath[128];
    char foutPath[128];
    FILE* fin = NULL;
    FILE* metaData = NULL;
    FILE* fout = NULL;
    sprintf(metaPath, "/home/zzx/paq8test/output/%s.meta", argv[2]);
    if(mode == COMPRESS) {
        //根据第二个参数选择文件
        sprintf(filePath, "/home/zzx/data/%s", argv[2]);
        sprintf(foutPath, "/home/zzx/paq8test/output/%s.p8t", argv[2]); // p8t = paq8test
        metaData = fopen(metaPath, "w");
    }
    else {
        sprintf(filePath, "/home/zzx/paq8test/output/%s.p8t", argv[2]);
        sprintf(foutPath, "/home/zzx/paq8test/output/%s", argv[2]);
        metaData = fopen(metaPath, "r");
        if(metaData == NULL){
            printf("Meta file \"%s.meta\" no exist\n", argv[2]);
            return 0;
        }
    }
    fin = fopen(filePath, "r");
    if(fin == NULL){
        printf("File \"%s\" no exist\n", argv[2]);
        fclose(metaData);
        return 0;
    }
    fout = fopen(foutPath, "w");

    // 初始化
    Shared shared;
    shared.init(2);
    clock_t t;

    if(mode == COMPRESS) {
        // 统计文件大小
        fseek(fin, 0, SEEK_END);
        U32 fsize = ftell(fin);
        fseek(fin, 0, SEEK_SET);
        // 计算分块数量
        U32 block_size;
        sscanf(argv[1], "%u", &block_size); //单位为KB
        block_size *= 1024; //单位换算为Byte
        if(block_size == 0 || block_size > fsize) { //0代表不分块
            block_size = fsize;
        }
        // 计算数据块数量
        U32 block_num;
        block_num = fsize / block_size;
        if(fsize % block_size > 0){
            block_num++;
        }
        // 计算训练数据量
        double update_ratio; 
        sscanf(argv[3], "%lf", &update_ratio);
        U32 update_block_num; 
        // 训练量有两种，一种是指定训练的数据块数量，一种是指定训练数据占比
        // 指定训练数据块数量
        // update_block_num = update_ratio;
        // 指定训练数据占比（向下取整）
        update_block_num = block_num * update_ratio / 100;

        if(update_block_num <= 0) {
            update_block_num = 1; // update_block必须是正数
        }
        if(update_block_num > block_num) {
            update_block_num = block_num;
        }

        // 重训练的超参数k和θ
        double para_k_ratio;
        U32 para_k = update_block_num;
        double para_theta = 0.8;
        sscanf(argv[4], "%lf", &para_k_ratio);
        para_k = block_num * para_k_ratio / 100;
        if(para_k <= 0) {
            update_block_num = 1;
        }
        if(para_k > block_num) {
            para_k = block_num;
        }
        sscanf(argv[5], "%lf", &para_theta);

        // 输出统计信息
        printf("Begin compressing %s\n", argv[2]);
        printf("file size: %u\n", fsize);
        printf("block size : %u B, block num: %u\n", block_size, block_num);
        printf("train block: %u(%.2lf%%)\n", update_block_num, update_ratio);
        printf("k: %u(%.2lf%%) theta: %.2lf\n", para_k, para_k_ratio, para_theta);
        
        // 写入元数据
        fwU32(metaData, fsize);
        fwU32(metaData, block_size);
        fwU32(metaData, block_num);
        fwU32(metaData, update_block_num);
        fwU32(metaData, para_k);
        U32 theta_U32 = (U32)(para_theta * 100);
        fwU32(metaData, theta_U32);


        //压缩处理
        Encoder *en = NULL;
        char c;
        U32 byteCount = 0;
        U32 train_block = 0;
        U32 static_block = 0;
        U32 block_count = 0; //这是将动态训练的多个数据块看作一个数据块的块计数
        U32 in_size = 0;
        U32 out_size = 0;
        U32 in_pos = 0;
        U32 out_pos = 0;
        // clock_t train_t;
        U32 train_times = 0;
        double comp_ratio;
        double train_ratio;



        double comp_ratios[para_k]; // TODO：确定一下检测的数据量要是多少
        for(int i = 0; i < para_k; i++) {
            comp_ratios[i] = 0.0;
        }
        // U32 ratios_index = 0;
        double ratio_sum = 0.0;

        en = new Encoder(&shared, COMPRESS, fout);
        t = clock();

        while(byteCount < fsize) {
            for(int i = 0; i < block_size && byteCount < fsize; i++) {
                c=getc(fin);
                en->compressByte(c);
                byteCount++;
            }
            train_block++;

            if(byteCount < fsize && shared.getUpdateState() == true && 
                train_block < update_block_num) {
                    continue;
            }

            en->flush();
            shared.reset();
            en->reset();

            //将压缩前、后大小保存至元数据
            in_size = ftell(fin) - in_pos;
            in_pos = ftell(fin);
            out_size = ftell(fout) - out_pos;
            out_pos = ftell(fout);
            fwU32(metaData, in_size);
            fwU32(metaData, out_size);
            block_count++;

#ifdef INFO
            if(shared.getUpdateState()) {
                printf("Dynamic ");
            }
            else{
                printf("Static ");
            }
            comp_ratio = ((double)in_size) / out_size;
            printf("compress %u B to %u B\tcompress ratio: %lf\n", 
                in_size, out_size, comp_ratio);
#endif

            if(shared.getUpdateState() == true) { 
                shared.staticPara();
                // train_t = clock() - t;
                train_times += 1;
                train_ratio = ((double)in_size) / out_size;
                static_block = 0;
            }
            else {
                int i = static_block % para_k;
                comp_ratio = ((double)in_size) / out_size;
                ratio_sum += comp_ratio - comp_ratios[i];
                comp_ratios[i] = comp_ratio;
                
                if(static_block >= para_k &&
                     ratio_sum / para_k < train_ratio * para_theta) {
                    // retrain
                    // delete en;
                    // en = new Encoder(&shared, COMPRESS, fout);
                    // TODO：真正的参数reset
                    shared.dynamicPara();
                    train_block = 0;
#ifdef INFO
                    printf("re train: train ration is %.2lf, and average compress ration is %.2lf\n", 
                        train_ratio, ratio_sum / para_k);
#endif
                }
                static_block++;
            }
        }

        //输出压缩统计信息
        t = clock() - t;
        out_size = ftell(fout);
        comp_ratio = ((double)fsize) / out_size;
        // double train_comp_ratio = ((double)train_size) / train_comp_size;
        printf("compressed size: %u B\ncompress ratio: %lf\n", out_size, comp_ratio);
        // printf("dynamic compresse ratio: %lf\n", train_comp_ratio);
        printf("compression time = %lf s\n",((float) t)/ CLOCKS_PER_SEC);
        printf("train %d times\n", train_times);
        // printf("train time=%lf s\n",((float) train_t)/ CLOCKS_PER_SEC);
        // printf("static compress time=%lf s\n",((float) t-train_t)/ CLOCKS_PER_SEC);

        //修改元数据中block数量
        fseek(metaData, sizeof(U32)*2, SEEK_SET);
        fwU32(metaData, block_count);

        delete en;
    }
    else {
        U32 fsize, block_size, block_num, update_block_num;
        frU32(metaData, fsize);
        frU32(metaData, block_size);
        frU32(metaData, block_num); //这个block_num是把动态压缩看成一个数据块的
        frU32(metaData, update_block_num);
        // TODO：读取k和theta两个参数，注意theta要除100
        printf("Begin decompressing %s\n", argv[2]);
        printf("file size: %u\n", fsize);
        printf("block size : %u B, block num: %u\n", block_size, block_num);
        printf("train block: %u\n", update_block_num);

        //解压处理
        Encoder *en = NULL;
        char c;
        U32 byteCount = 0;
        U32 blockCount = 0;
        U32 before_comp_size = 0;
        U32 after_comp_size = 0;
        U32 in_pos = 0;
        U32 out_pos = 0;

        en = new Encoder(&shared, DECOMPRESS, fin);
        t = clock();

        while(byteCount < fsize) {
            frU32(metaData, before_comp_size);
            frU32(metaData, after_comp_size);

#ifdef INFO
            printf("origin block size: %u, compressed block size: %u, ", 
                before_comp_size, after_comp_size);
#endif

            for(int i = 0; i < before_comp_size; i++) {
                c = en->decompressByte();
                putc(c, fout);
                byteCount++;
            }
            blockCount++;
            // fseek(fin, -3, SEEK_CUR); // 文件回退3字节

#ifdef INFO
            if(shared.getUpdateState()) {
                printf("Dynamic ");
            }
            else{
                printf("Static ");
            }
            printf("decompress %ld B to %u B\n", 
                ftell(fin) - in_pos, before_comp_size);
#endif

            // 检查数据长度
            if(ftell(fin) - in_pos != after_comp_size) {
                printf("Decompress size error on block %d\n", blockCount);
                return -1;
            }
            in_pos = ftell(fin);

            shared.reset();
            en->reset();
            if(shared.getUpdateState() == true) { 
                shared.staticPara();
            }
        }

        //输出压缩统计信息
        t = clock() - t;
        printf("Decompress Done\n");
        printf("compression time=%lf s\n",((float) t)/ CLOCKS_PER_SEC);

        delete en;
    }

    fclose(fin);
    fclose(fout);
    fclose(metaData);

    printf("\n"); // 为了重定向的输出分割开，方便查看
    return 0;
}
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

int main(int argc,char **argv){
        
    // 模式选择：压缩、解压
    Mode mode(COMPRESS);
    mode = (strcmp(argv[1], "-d")) ? COMPRESS : DECOMPRESS;

    // 输入输出元数据文件
    char filePath[128];
    char metaPath[128];
    char foutPath[128];
    FILE* fp = NULL;
    FILE* metaData = NULL;
    FILE* out = NULL;
    sprintf(metaPath, "/home/zzx/paq8test/output/%s.meta", argv[2]);
    if(mode == COMPRESS) {
        if(strcmp(argv[2], "enwik8") == 0){ //根据第二个参数选择文件
            sprintf(filePath, "/home/zzx/data/enwik8");
        }
        else if(strcmp(argv[2], "enwik9") == 0){
            sprintf(filePath, "/home/zzx/data/enwik9");
        }
        else if(strcmp(argv[2], "book1") == 0 || strcmp(argv[2], "book2") == 0){
            sprintf(filePath, "/home/zzx/data/calgarycorpus/%s", argv[2]);
        }
        else{
            sprintf(filePath, "/home/zzx/data/silesia/%s", argv[2]);
        }
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
    fp = fopen(filePath, "r");
    if(fp == NULL){
        printf("File \"%s\" no exist\n", argv[2]);
        fclose(metaData);
        return 0;
    }
    out = fopen(foutPath, "w");

    Shared shared;
    shared.init(2);
    clock_t t;

    if(mode == COMPRESS) {
        // 计算分块数量
        U32 block_size;
        // 统计文件大小
        fseek(fp, 0, SEEK_END);
        U32 fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        sscanf(argv[1], "%u", &block_size); //单位为KB
        block_size *= 1024; //单位换算为Byte
        if(block_size == 0) {
            block_size = fsize; //0代表不分块
        }
        // 计算数据块数量
        U32 blocks;
        blocks = fsize / block_size;
        if(fsize % block_size > 0){
            blocks++;
        }

        // 计算训练数据量
        U32 update_ratio; 
        sscanf(argv[3], "%u", &update_ratio);
        U32 update_block; 
        // 训练量有两种，一种是指定训练的数据块数量，一种是指定训练数据占比
        // 指定训练数据块数量
        update_block = update_ratio;
        // 指定训练数据占比（向下取整）
        // update_block = blocks * update_ratio / 100;

        // if(update_block <= 0) {
        //     update_block = 1; // update_block必须是正数
        // }

        // 输出统计信息
        printf("Begin compressing %s\n", argv[2]);
        printf("file size: %u\n", fsize);
        printf("block size : %u B, block num: %u\n", block_size, blocks);
        printf("train block: %u\n", update_block);
        // TODO:测试这里需不需要空格
        fprintf(metaData, "%u%u%u%u", fsize, block_size, blocks, update_block);

        //压缩处理
        Encoder *en = NULL;
        Encoder *train_en = NULL;
        char c;
        U32 byteCount = 0;
        U32 blockCount = 0;
        U32 csize = 0;
        U32 out_pos = 0;

        en = new Encoder(&shared, COMPRESS, out);
        t = clock();

        while(byteCount < fsize){
            for(int i = 0; i < block_size; i++) {
                c=getc(fp);
                en->compressByte(c);
                byteCount++;
                if(byteCount >= fsize){
                    break;
                }
            }
            blockCount++;
            if(shared.getUpdateState() == true && blockCount >= update_block) { // training
                shared.staticPara();
                train_en = en;
                en->flush();
                en = new Encoder(&shared, COMPRESS, out, train_en);
            }
            else if(shared.getUpdateState() == false) {
                en->flush();
                en = new Encoder(&shared, COMPRESS, out, train_en);
            }

            csize = ftell(out) - out_pos;
            out_pos = ftell(out);
            fprintf(metaData, "%u", csize);

            if(shared.updateState){
                printf("Dynamic ");
            }
            else{
                printf("Static ");
            }
            printf("Block compressed size: %u B\n", csize);
        }
        
        //动态、静态压缩信息
        // std::cout<<"dynamic compress " << byteThreshold << " B to " << dynamicByte << " B" <<std::endl;
        // printf("dynamic compression time=%lf s\n",((float) dynamic_t)/ CLOCKS_PER_SEC);

        // csize = ftell(out) - csize;
        // std::cout<<"static compress " << byteCount - byteThreshold << " B to " << csize << " B" <<std::endl;
        // t = clock() - start - t;
        // printf("static compression time=%lf s\n",((float) t)/ CLOCKS_PER_SEC);

        //输出压缩统计信息
        t = clock() - t;
        csize = ftell(out);
        printf("compressed size: %u B\n", csize);
        printf("compression time=%lf s\n\n",((float) t)/ CLOCKS_PER_SEC);
    }
    else {
        U32 fsize, block_size, blocks, update_block;
        fscanf(metaData, "%u%u%u%u", &fsize, &block_size, &blocks, &update_block);
        printf("Begin decompressing %s\n", argv[2]);
        printf("file size: %u\n", fsize);
        printf("block size : %u B, block num: %u\n", block_size, blocks);
        printf("train block: %u\n", update_block);

        //解压处理
        Encoder *en = NULL;
        Encoder *train_en = NULL;
        char c;
        U32 byteCount = 0;
        U32 blockCount = 0;
        U32 csize = 0;
        U32 out_pos = 0;

        en = new Encoder(&shared, DECOMPRESS, out);
        t = clock();

        while(byteCount < fsize){
            for(int i = 0; i < block_size; i++) {
                putc(en->decompressByte(), out);
                byteCount++;
                if(byteCount >= fsize){
                    break;
                }
            }
            blockCount++;
            if(shared.getUpdateState() == true && blockCount >= update_block) { // training
                shared.staticPara();
                train_en = en;
                fseek(out, -3, SEEK_CUR); // 文件回退3字节
                en = new Encoder(&shared, DECOMPRESS, out, train_en);
            }
            else if(shared.getUpdateState() == false) {
                fseek(out, -3, SEEK_CUR);
                en = new Encoder(&shared, DECOMPRESS, out, train_en);
            }

            csize = ftell(out) - out_pos;
            out_pos = ftell(out);

            if(shared.updateState){
                printf("Dynamic ");
            }
            else{
                printf("Static ");
            }
            printf("Block decompressed size: %u B\n", csize);
        }        
    }

    fclose(fp);
    fclose(out);
    fclose(metaData);

}
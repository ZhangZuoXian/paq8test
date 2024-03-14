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

#define MAX_FILE_NAME 256

// #define INFO

// 针对元数据的U32类型的读写
// size_t frU32(FILE *f, U32 &data) {
//     return fread(&data, sizeof(U32), 1, f);
// }
// size_t fwU32(FILE *f, U32 &data) {
//     // printf("[META] write %u to meta data file\n", data);
//     return fwrite(&data, sizeof(U32), 1, f);
// }

typedef struct _COMP_INFO {
    U32 uncomp_byte;
    U32 comped_byte;
    clock_t comp_t;
} COMP_INFO;

int main(int argc,char **argv){

    /* 1. 检查输入参数 */
    if (argc!=7 && argv[1][0]!='d') {
        printf(
            "lpaq1 file compressor (C) 2007, Matt Mahoney\n"
            "Licensed under GPL, http://www.gnu.org/copyleft/gpl.html\n"
            "\n"
            // "To compress:     lpaq1 N input output    (N=0..9, uses 3+3*2^N MB)\n"
            // "To decompress: lpaq1 d input output    (needs same memory)\n");
            "To compress:     lpaq1 N input blockSize trainBlock k θ (N=0..9, uses 3+3*2^N MB)\n"
            "To decompress: lpaq1 d input (needs same memory)\n"); // 所需的其他信息都从压缩数据头中获取
        return 1;
    }

    /* 2. 初始化 */
    clock_t start_t = clock();
    Shared shared;
    char filePath[MAX_FILE_NAME];
    FILE *in = NULL;
    FILE *out = NULL;  
    U32 block_size = 0;         // 数据块长度（目前只是用来计算压缩比）
    U32 block_num = 0;          // 文件包含的数据块数量（目前不需要使用）
    U32 train_block = 0;        // 每次训练使用的数据块数量
    U32 para_k = 0;             // 参数k，即判断是否需要重训练时数据块的数量
    double para_theta = 0.0;    // 参数θ，即判断是否需要重训练时压缩比的阈值  
    U32 fsize = 0;              // 初始文件大小
    U8 comp_lv = 0;             // 压缩等级，0~12
    COMP_INFO dynamic_info = {0, 0, 0};
    COMP_INFO static_info = {0, 0, 0};
    clock_t t;
        
    /* 3. 获得模式（压缩/解压） */
    Mode mode;
    mode = (strcmp(argv[1], "d")) ? COMPRESS : DECOMPRESS;

    /* 4. 输入输出文件处理 */
    if(mode == COMPRESS) {
        sprintf(filePath, "/home/zzx/data/%s", argv[2]);
        in = fopen(filePath, "rb");
        sprintf(filePath, "../output/%s.p8t", argv[2]);
        out = fopen(filePath, "wb");
    } else {
        sprintf(filePath, "../output/%s.p8t", argv[2]);
        in = fopen(filePath, "rb");
        sprintf(filePath, "../output/%s.decmp", argv[2]);
        out = fopen(filePath, "wb");
    }
    if (!in || !out) perror(argv[2]), exit(1);    

    /* 5. 压缩、解压处理 */
    // Compress
    if(mode == COMPRESS) {
        /* 5.1 获得并写入元数据信息 */
        sscanf(argv[1], "%hhu", &comp_lv);
        assert(comp_lv >= 0 && comp_lv <= 12);
        fwrite(&(comp_lv), sizeof(U8), 1, out);
        shared.init(comp_lv);

        fseek(in, 0, SEEK_END);
        fsize = ftell(in);
        fseek(in, 0, SEEK_SET);
        fwrite(&(fsize), sizeof(U32), 1, out);

        // 计算分块数量
        sscanf(argv[3], "%u", &block_size); //单位为KB
        fwrite(&(block_size), sizeof(U32), 1, out);
        block_size *= 1024; //单位换算为Byte
        if(block_size == 0 || block_size > fsize) { //0代表不分块
            block_size = fsize;
        }
        // 计算数据块数量
        block_num = fsize / block_size;
        if(fsize % block_size > 0){
            block_num++;
        }
        // 计算训练数据量
        double update_ratio; 
        sscanf(argv[4], "%lf", &update_ratio);
        // 指定训练数据占比（向下取整）
        train_block = block_num * update_ratio / 100;
        if(train_block <= 0) {
            train_block = 1; // update_block必须是正数
        } else if(train_block > block_num) {
            train_block = block_num;
        }
        // 重训练的超参数k和θ
        double para_k_ratio;
        double para_theta = 0.8;
        sscanf(argv[5], "%lf", &para_k_ratio);
        para_k = block_num * para_k_ratio / 100;
        if(para_k <= 0) {
            para_k = 1;
        } else if(para_k > block_num) {
            para_k = block_num;
        }
        sscanf(argv[6], "%lf", &para_theta);
        // 输出统计信息
        printf("compress level: %d\n", comp_lv);
        printf("input file: %s\n", argv[2]);
        printf("file size: %u\n", fsize);
        printf("block size : %u B, block num: %u\n", block_size, block_num);
        printf("train block: %u(%.2lf%%)\n", train_block, update_ratio);
        printf("k: %u(%.2lf%%) theta: %.2lf\n", para_k, para_k_ratio, para_theta);
        // 写入元数据
        fwrite(&(train_block), sizeof(U32), 1, out);
        fwrite(&(para_k), sizeof(U32), 1, out);
        fwrite(&(para_theta), sizeof(double), 1, out);

        /* 5.2 生成动态压缩器、压缩 */
        char c;
        U32 block_size_uncomp = 0;
        U32 block_size_comped = 0;
        U32 in_pos = 0;
        U32 out_pos = 0;
        U32 byteCount = 0;
        U32 dynamic_block = 0;
        U32 static_block = 0;
        U32 train_times = 1;
        double comp_ratio;
        double dynamic_ratio;
        double static_ratios[para_k];
        for(int i = 0; i < para_k; i++) {
            static_ratios[i] = 0.0;
        }
        double static_r_sum = 0.0;

        Encoder en(&shared, COMPRESS, out);
        t = clock();
        while(byteCount < fsize) {
            for(int i = 0; i < block_size && byteCount < fsize; i++) {
                c=getc(in);
                en.compressByte(c);
                byteCount++;
            }
            dynamic_block++;

            // 动态数据块没压完，继续压
            if(byteCount < fsize && shared.updateState == true && 
                dynamic_block < train_block) {
                    continue;
            }

            en.flush(); 
            en.blockReset();

            //将压缩前、后大小保存至元数据
            block_size_uncomp = ftell(in) - in_pos;
            in_pos = ftell(in);
            block_size_comped = ftell(out) - out_pos;
            out_pos = ftell(out);
            comp_ratio = ((double)block_size_uncomp) / block_size_comped;

#ifdef INFO
            if(shared.updateState) {
                printf("Dynamic ");
            } else {
                printf("Static ");
            }
            printf("compress %u B to %u B\tcompress ratio: %lf\n", 
                block_size_uncomp, block_size_comped, comp_ratio);
#endif

            if(shared.updateState == true) { 
                dynamic_info.uncomp_byte += block_size_uncomp;
                dynamic_info.comped_byte += block_size_comped;
                shared.updateState = false;
                dynamic_ratio = comp_ratio;
                static_block = 0;
                dynamic_info.comp_t += clock() - t;
                t = clock();
            }
            else {
                int i = static_block % para_k;
                static_r_sum += comp_ratio - static_ratios[i];
                static_ratios[i] = comp_ratio;
                static_block++;
                
                if(static_block >= para_k &&
                     static_r_sum / para_k < dynamic_ratio * para_theta) {
                    train_times += 1;
                    shared.updateState = true;
                    dynamic_block = 0;
                    for(U32 j = 0; j < para_k; j++) {
                        static_ratios[j] = 0.0;
                    }
                    static_r_sum = 0.0;
                    static_info.comp_t += clock() - t;
                    t = clock();
                }
            }
        }

        if(shared.updateState == false) {
            static_info.comp_t += clock() - t;
        }

        //输出压缩统计信息

        comp_t = (double)dynamic_info.comp_t/CLOCKS_PER_SEC;
        // printf("%u %u %.2lf\n", dynamic_info.uncomp_byte, dynamic_info.comped_byte, comp_t);
        printf("dynamic compress %u to %u in %.2lf sec\n\tratio = %.6lf, thoughtout = %.6lf KB/s.\n", 
            dynamic_info.uncomp_byte, dynamic_info.comped_byte, comp_t, 
            (double)(dynamic_info.uncomp_byte) / dynamic_info.comped_byte,
            dynamic_info.uncomp_byte / comp_t / 1024);

        comp_t = (double)static_info.comp_t/CLOCKS_PER_SEC;
        // printf("%u %u %.2lf\n", static_info.uncomp_byte, static_info.comped_byte, comp_t);
        printf("static compress %u to %u in %.2lf sec\n\tratio = %.6lf, thoughtout = %.6lf KB/s.\n\n", 
            static_info.uncomp_byte, static_info.comped_byte, comp_t,
            (double)(static_info.uncomp_byte) / static_info.comped_byte,
            static_info.uncomp_byte / comp_t / 1024);
    }
    else {
        /* 5.1 获得并输出元数据信息 */
        fread(&(comp_lv), sizeof(U8), 1, in);
        shared.init(comp_lv);
        fread(&(fsize), sizeof(U32), 1, in);
        fread(&(block_size), sizeof(U32), 1, in);
        block_size *= 1024;
        fread(&(train_block), sizeof(U32), 1, in);
        fread(&(para_k), sizeof(U32), 1, in);
        fread(&(para_theta), sizeof(double), 1, in);
        // 输出统计信息
        printf("compress level: %d\n", comp_lv);
        printf("input file: %s\n", argv[2]);
        printf("file size: %u\n", fsize);
        printf("block size : %u B\n", block_size);
        printf("train block: %u\n", train_block);
        printf("k: %u theta: %.2lf\n", para_k, para_theta);

        if(block_size == 0) {
            block_size = fsize;
        }

        /* 5.2 生成动态压缩器、解压 */
        char c;
        U32 byte_count = 0;
        U32 block_size_uncomp = 0;
        U32 block_size_comped = 0;
        U32 in_pos = 0;
        U32 out_pos = 0;
        U32 train_times = 0;
        // 重训练阈值相关
        double comp_ratio;
        double dynamic_ratio;
        U32 static_block = 0;
        double static_ratios[para_k]; 
        for(int i = 0; i < para_k; i++) {
            static_ratios[i] = 0.0;
        }
        double static_r_sum = 0.0;

        Encoder en(&shared, DECOMPRESS, in);
        t = clock();

        while(byte_count < fsize) {
            U32 cur_blk_size = block_size;
            if(shared.updateState) {
                cur_blk_size *=  train_block;
            }
            for(U32 i = 0; i < cur_blk_size && byte_count < fsize; i++) {
                c = en.decompressByte();
                putc(c, out);
                byte_count++;
            }
            en.blockReset();

            // 计算压缩比，这里的压缩前后大小与压缩时正好相反
            block_size_comped = ftell(in) - in_pos;
            in_pos = ftell(in);
            block_size_uncomp = ftell(out) - out_pos;
            out_pos = ftell(out);
            comp_ratio = ((double)block_size_uncomp) / block_size_comped;            

#ifdef INFO
            if(shared.updateState) {
                printf("Dynamic ");
            }
            else{
                printf("Static ");
            }
            printf("compress %u B to %u B\tcompress ratio: %lf\n", 
                block_size_uncomp, block_size_comped, comp_ratio);
#endif

            if(shared.updateState == true) { 
                shared.updateState = false;
                dynamic_ratio = comp_ratio;
                static_block = 0;
            }
            else {
                U32 i = static_block % para_k;
                static_r_sum += comp_ratio - static_ratios[i];
                static_ratios[i] = comp_ratio;
                static_block++;
                
                if(static_block >= para_k &&
                     static_r_sum / para_k < dynamic_ratio * para_theta) {
                    shared.updateState = true;
                    for(U32 j = 0; j < para_k; j++) {
                        static_ratios[j] = 0.0;
                    }
                    static_r_sum = 0.0;                    
                }
            }
        }

        //输出压缩统计信息
        t = clock() - t;
        printf("Decompress Done\n");
        printf("compression time=%lf s\n",((float) t)/ CLOCKS_PER_SEC);
        printf("train %d times\n", train_times);

    }

    fclose(in);
    fclose(out);

    // 检查解压后文件和原文件是否相同
    if(mode == DECOMPRESS) {
        printf("Checking files\n");
        sprintf(filePath, "/home/zzx/data/%s", argv[2]);
        printf("Origin file: %s\n", filePath);
        FILE *origin_file = fopen(filePath, "rb");
        sprintf(filePath, "../output/%s.decmp", argv[2]);
        printf("Decompressed file: %s\n", filePath);
        FILE *decomp_file = fopen(filePath, "rb");

        fseek(origin_file, 0, SEEK_END);
        U32 len_origin=ftell(origin_file);
        fseek(origin_file, 0, SEEK_SET);
        fseek(decomp_file, 0, SEEK_END);
        U32 len_decomp=ftell(decomp_file);
        fseek(decomp_file, 0, SEEK_SET);
        
        bool is_same = true;
        U32 error_line = 1;
        char c;
        if(len_origin != len_decomp) {
            is_same = false;
            error_line = 0;
        }
        else {
            for(U32 i = 0; i < len_origin; i++) {
                c = getc(origin_file);
                if(c != getc(decomp_file)) {
                    is_same = false;
                    break;
                }
                if(c == '\n') {
                    error_line += 1;
                }                
            }
        }
        fclose(origin_file);
        fclose(decomp_file);
        if(is_same) {
            printf("success!\n");
        } else {
            printf("ERROR! file different in line %u\n", error_line);
        }
    }

    return 0;
}
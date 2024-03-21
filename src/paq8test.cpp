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

int main(int argc,char **argv){

    /* 1. 检查输入参数 */
    if (argc!=3 && argv[1][0]!='d') {
        printf(
            "To compress:   paq8test N input\n"
            "To decompress: paq8test d input\n"); // 所需的其他信息都从压缩数据头中获取
        return 1;
    }

    /* 2. 初始化 */
    clock_t start_t = clock();
    clock_t comp_t;
    Shared shared;
    char filePath[MAX_FILE_NAME];
    FILE *in = NULL;
    FILE *out = NULL;  
    FILE* deCom = fopen("../decomress.txt", "w");
    U32 fsize = 0;              // 初始文件大小
    U8 comp_lv = 0;             // 压缩等级，0~12
        
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

        // 输出统计信息
        printf("compress level: %d\n", comp_lv);
        printf("input file: %s\n", argv[2]);
        printf("file size: %u\n", fsize);

        /* 5.2 生成动态压缩器、压缩 */
        char c;
        U32 byteCount = 0;

        Encoder en(&shared, COMPRESS, out, deCom);
        comp_t = clock();
        while(byteCount < fsize) {
            c=getc(in);
            en.compressByte(c);
            byteCount++;
        }
        en.flush(); 
        comp_t = clock() - comp_t;


        //输出压缩统计信息
        double t = (double)(clock() - start_t)/CLOCKS_PER_SEC;
        double c_t = (double)(comp_t)/CLOCKS_PER_SEC;
        printf("%u -> %ld, compress ration = %.2lf\n", 
            fsize, ftell(out), ((double)fsize) / ftell(out));
        // printf("%ld %ld %.6lf %.6f\n", ftell(in), ftell(out), t, c_t);
        printf("time: %.2lf, comp time: %.2lf\n", t, c_t);
        printf("all thoughtout: %.2lf KB/s, comp thoughtout: %.2lf KB/s\n\n", fsize / t / 1024, fsize / c_t / 1024);
    }
    else {
        /* 5.1 获得并输出元数据信息 */
        fread(&(comp_lv), sizeof(U8), 1, in);
        shared.init(comp_lv);
        fread(&(fsize), sizeof(U32), 1, in);

        // 输出统计信息
        printf("compress level: %d\n", comp_lv);
        printf("input file: %s\n", argv[2]);
        printf("file size: %u\n", fsize);

        /* 5.2 生成动态压缩器、解压 */
        char c;
        U32 byte_count = 0;

        Encoder en(&shared, DECOMPRESS, in, deCom);
        while(byte_count < fsize) {
            c = en.decompressByte();
            putc(c, out);
            byte_count++;
        }

        //输出压缩统计信息
        start_t = clock() - start_t;
        printf("Decompress Done\n");
        printf("Decompression time=%lf s\n",((double) start_t)/ CLOCKS_PER_SEC);
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
#include <stdio.h>
#include <stdlib.h>    
#include <string.h>    
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>

typedef enum {COMPRESS, DECOMPRESS} Mode;
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

const char *fname = NULL;
char inFile[256];

int divide_file(U32 blk_size) {
    FILE *fin = fopen(inFile, "r");
    if(fin == NULL) {
        printf("cannot open %s\n", inFile);
        return 0;
    }

    size_t file_size;
    fseek(fin, 0, SEEK_END);
    file_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    U32 blk_num;
    blk_num = file_size / blk_size;
    blk_num += (file_size % blk_size ? 1 : 0);

    printf("open file \"%s\", size: %lu divide into %d (block size = %d)\n", 
        fname, file_size, blk_num, blk_size / 1024);

    // 创建目录
    char dirName[256];
    sprintf(dirName, "./output/%s", fname);
    if(access(dirName, F_OK) != 0 && mkdir(dirName, S_IRWXU) == -1) {
        printf("something wrong with mkdir \"%s\", errno: %d\n", dirName, errno);
        return 0;
    }

    char outFile[256];
    FILE *fout = NULL;
    char buf[blk_size+1];
    size_t rw_size;
    size_t r_size;
    size_t w_size;
    size_t pos = 0;
    for(U32 i = 0; i < blk_num; i++) {
        sprintf(outFile, "./output/%s/%s_%d", fname, fname, i);
        fout = fopen(outFile, "w");
        assert(fout);

        memset(buf, 0, sizeof(buf));
        r_size = fread(buf, 1, blk_size, fin);
        w_size = fwrite(buf, 1, r_size, fout);
        if(r_size != w_size) {
            printf("something wrong with read and write file \"%s\"\n", outFile);
            fclose(fout);
            fclose(fin);
            return 0;
        }
        fclose(fout);
    }
    fclose(fin);
    return blk_num;
}

int main(int argc, char **argv) {
    // printf(
    //     "paq8l file compressor (C) 2007, Matt Mahoney\n"
    //     "Licensed under GPL, http://www.gnu.org/copyleft/gpl.html\n"
    //     "\n"
    //     "To compress:   paq8l N blocksize input (N=0..9, uses 3+3*2^N MB)\n"
    //     "To decompress: paq8l d input (needs same memory)\n");

    Mode mode;
    const char* comp_lv = argv[1];
    const U16 blk_size = atoi(argv[2]);
    assert(blk_size >=0 && blk_size <= 1024); // blk_size为0到1MB

    if(argv[1][0] == 'd') { // 解压未实现
        printf("Decompression not yet realized\n");
    } 
    mode = COMPRESS;

    // 拼接路径获得filename
    fname = argv[3];
    memset(inFile, 0, sizeof(inFile));
    sprintf(inFile, "/home/zzx/data/%s", fname);

    // 构建system的命令
    char cmd[1024];
    char outFile[256];
    // 检测log是否存在，如果存在则删除（清空）
    char logFile[256];
    sprintf(logFile, "./output/%s.log", fname);
    if(access(logFile, F_OK) == 0) {
        unlink(logFile);
    }

    struct timeval start;

    if(blk_size == 0) { // 不分块
        printf("open file \"%s\"\n", fname);
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, "./paq8_for_block %s %s 0 1 1 1 ./output/%s/%s >> ./output/%s.log",
            comp_lv, inFile, fname, fname, fname); 
        gettimeofday(&start, NULL); 
        system(cmd);
    }
    else {
        // 文件分块
        U32 blk_num = divide_file(blk_size * 1024);
        if(blk_num == 0) {
            printf("Error in divide file\n");
            return 1;
        }
        // 压缩
        char inFile_split[256];
        gettimeofday(&start, NULL); 
        for(U32 i = 0; i < blk_num; i++) {
            sprintf(inFile_split, "./output/%s/%s_%d", fname, fname, i);
            memset(cmd, 0, sizeof(cmd));
            sprintf(cmd, "./paq8_for_block %s %s 0 1 1 1 %s.paq >> ./output/%s.log",
                comp_lv, inFile_split, inFile_split, fname);
            system(cmd);
        }
    }
    // 解析日志获得信息
    FILE *flog = fopen(logFile, "r");
    U32 uncmp_block_size;
    U32 cmped_block_size;
    U32 uncmp_total_size = 0;
    U32 cmped_total_size = 0;
    double time_block;
    double time_total = 0.0;
    double time_comp_block;
    double time_comp_total = 0.0;
    while(fscanf(flog, "%u %u %lf %lf", 
        &uncmp_block_size, &cmped_block_size, &time_block, &time_comp_block) == 4) {
        uncmp_total_size += uncmp_block_size;
        cmped_total_size += cmped_block_size;
        time_total += time_block;
        time_comp_total += time_comp_block;
    }
    printf("%u -> %u in %1.2f sec, in level %s.\n", 
        uncmp_total_size, cmped_total_size, time_total, comp_lv);
    printf("compress ratio = %.6lf\ncopmress time with init: %.2lf sec, thoughtout = %.6lf KB/s.\n", 
        ((double)uncmp_total_size) / cmped_total_size, time_total, uncmp_total_size / time_total / 1024);
    printf("compress time: %.2lf sec, thoughtout = %.6lf KB/s.\n", time_comp_total, 
        uncmp_total_size / time_comp_total / 1024);
    struct timeval end;
    gettimeofday(&end, NULL);
    double time_all = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    time_all /= 1000000;
    printf("all time: %.2lf sec, thoughtout = %.6lf KB/s.\n\n", time_all, 
        uncmp_total_size / time_all / 1024);

    return 0;
}

#include<iostream>
#include<memory>
#include<unistd.h>
#include<fstream>
#include"Encoder.hpp"
#include"Shared.hpp"
#include<getopt.h>
#include<time.h>

using namespace std;

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

U32 x1=0, x2=0xffffffff;

long BLOCK_SIZE;
FILE* fp = NULL;
FILE* deCom = fopen("/home/zzx/paq8test/originPaq8test/decomress.txt","w");

int main(int argc,char **argv){

    Shared shared;
    clock_t t;

    // shared.level = 2;
    shared.init(2);
    // static const struct option longopts[] = {
    //     {"decompress",  0,  NULL,   'd'},
    //     {"compress",    0,  NULL,   'c'},
    // };

    sscanf(argv[1], "%ld", &BLOCK_SIZE); //第一个参数为分块大小，单位为KB
    BLOCK_SIZE *= 1024; //单位换算为Byte
    if(strcmp(argv[2], "enwik8") == 0){ //根据第二个参数选择文件
        fp = fopen("/home/zzx/paq8test/data/enwik8/enwik8","r");
    }
    else if(strcmp(argv[2], "enwik9") == 0){
        fp = fopen("/home/zzx/paq8test/data/enwik9","r");
    }
    else{
        char filePath[64];
        sprintf(filePath, "/home/zzx/paq8test/data/calgarycorpus//%s", argv[2]);
        fp = fopen(filePath, "r");
    }
    if(fp == NULL){
        printf("File \"%s\" no exist\n", argv[2]);
        return 0;
    }
    
    Mode mode(COMPRESS);
    mode = (argc == 3) ? COMPRESS : DECOMPRESS;

    //生成压缩器
    //使用压缩器编码，并统计信息
    if(mode == COMPRESS){

        FILE* out = fopen("/home/zzx/paq8test/originPaq8test/output.txt","w");

        printf("Begin compression:....\n");
        Encoder *en = NULL;

        //统计文件大小
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        std::cout << "the size is : " << size << "B\n";
        fseek(fp, 0, SEEK_SET);

        //压缩处理
        char c;
        t = clock();

        long byteCount = 0;
        bool notEnd = 1;
        size = 0;
        long sizeTmp = 0;
        while(notEnd){
            // printf("%x\t",c);
            en = new Encoder(&shared, COMPRESS, out, deCom);
            for(byteCount = 0; byteCount < BLOCK_SIZE; byteCount++){
                if((c=getc(fp)) == EOF){
                    notEnd = 0;
                    break;
                }
                en->compressByte(c);
            }
            //多余小数刷新回压缩文件中
            en->flush();
            delete en;

            sizeTmp = size;
            size = ftell(out);
            std::cout<<"Block compressed size: "<<size - sizeTmp<<"B"<<std::endl;
        }
        
        t = clock()-t;
        
        //输出压缩统计信息
        size = ftell(out);
        std::cout<<"compressed size: "<<size<<"B"<<std::endl;
        printf("compression time=%lf s\n",((float) t)/ CLOCKS_PER_SEC);
        fclose(out);
    } else {
        FILE* out = fopen("/home/zzx/paq8test/originPaq8test/output.txt","r");
        Encoder en(&shared, DECOMPRESS,out,deCom);
        printf("Begin decompression:....\n");
        for(int i=0;i<4;i++){
            putc(en.decompressByte(),deCom);
            // cout<<en.decompressByte();
        }
        // uint8_t a = 'a';
        // putc(a,deCom);
        fclose(out);
    }

    fclose(fp);
    fclose(deCom);

}
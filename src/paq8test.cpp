
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

U32 x1=0,x2=0xffffffff;

#define DEBUG true

// FILE* fp = fopen("/home/ghj/lpaq1/test/src/enwik8/enwik8","r");
#ifdef DEBUG 
    FILE* fp = fopen("/home/ghj/lpaq1/test/src/easy.txt","r");
#else 
    FILE* fp = fopen("/home/ghj/lpaq1/test/src/calgarycorpus/book1","r");
#endif

FILE* deCom = fopen("/home/ghj/c++_project/compress/paq8test/decomress.txt","w");

int main(int argc,char **argv){
    // ifstream file("/home/ghj/lpaq1/test/src/calgarycorpus/book1");

    Shared shared;
    clock_t t;

    // shared.level = 2;
    shared.init(2);
    // static const struct option longopts[] = {
    //     {"decompress",  0,  NULL,   'd'},
    //     {"compress",    0,  NULL,   'c'},
    // };
    
    Mode mode(COMPRESS);
    mode = (argc == 1) ? COMPRESS : DECOMPRESS;

    //生成压缩器
    //使用压缩器编码，并统计信息
    if(mode == COMPRESS){

        FILE* out = fopen("/home/ghj/c++_project/compress/paq8test/output.txt","w");

        printf("Begin compression:....\n");
        Encoder en(&shared, COMPRESS,out,deCom);
        //统计文件大小
        fseek(fp,0,SEEK_END);
        long size = ftell(fp);
        // printf("the size is %ll\n",size);
        std::cout<<"the size is : "<<size<<"B\n";
        fseek(fp, 0, SEEK_SET);
        //压缩处理
        char c;
        t = clock();

        while((c=getc(fp))!=EOF){
            printf("%x\t",c);
            en.compressByte(c);
        }
        //多余小数刷新回压缩文件中
        en.flush();
        
        t = clock()-t;
        
        //输出压缩统计信息
        size = ftell(out);
        std::cout<<" ---> "<<size<<"B"<<std::endl;
        printf("compression time=%lf s\n",((float) t)/ CLOCKS_PER_SEC);
        printf("model prediction cost time = %lf s\n",((float) en.modelTimeCost )/ CLOCKS_PER_SEC);
        printf("\t Match model prediction cost time = %lf s\n",((float) shared.MatchModelCostTime )/ CLOCKS_PER_SEC);
        printf("\t normal model prediction cost time = %lf s\n",((float) shared.NormalModelCostTime )/ CLOCKS_PER_SEC);
        
        printf("map update cost time = %lf s\n", ((float) en.mapUpdateCost)/ CLOCKS_PER_SEC);
        fclose(out);
    } else {
        FILE* out = fopen("/home/ghj/c++_project/compress/paq8test/output.txt","r");
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
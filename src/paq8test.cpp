
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

U32 BLOCK_SIZE;
U32 updateBlock; //预训练数据块的数量

FILE* fp = NULL;
FILE* deCom = fopen("/home/zzx/paq8test/decomress.txt","w");

int main(int argc,char **argv){

    Shared shared;
    clock_t t;

    // shared.level = 2;
    shared.init(2);
    // static const struct option longopts[] = {
    //     {"decompress",  0,  NULL,   'd'},
    //     {"compress",    0,  NULL,   'c'},
    // };

    sscanf(argv[1], "%u", &BLOCK_SIZE); //第一个参数为分块大小，单位为KB
    char filePath[128];
    if(strcmp(argv[2], "enwik8") == 0){ //根据第二个参数选择文件
        sprintf(filePath, "/home/zzx/data/enwik8");
    }
    else if(strcmp(argv[2], "enwik9") == 0){
        sprintf(filePath, "/home/zzx/data/enwik9");
    }
    else if(strcmp(argv[2], "BOOK1") == 0 || strcmp(argv[2], "BOOK2") == 0){
        sprintf(filePath, "/home/zzx/data/calgary/%s", argv[2]);
    }
    else{
        sprintf(filePath, "/home/zzx/data/silesia/%s", argv[2]);
    }
    fp = fopen(filePath, "r");
    if(fp == NULL){
        printf("File \"%s\" no exist\n", argv[2]);
        return 0;
    }
    
    Mode mode(COMPRESS);
    mode = (argc == 3) ? COMPRESS : DECOMPRESS;

    //生成压缩器
    //使用压缩器编码，并统计信息
    if(mode == COMPRESS){
        char foutPath[128];
        sprintf(foutPath, "/home/zzx/paq8test/output/%s", argv[2]);
        FILE* out = fopen(foutPath, "w");

        printf("Begin compression %s\n", argv[2]);
        // std::cout << "block size is : " << BLOCK_SIZE << " KB\n";
        std::cout << "train ratio is : " << BLOCK_SIZE << " \n";
        Encoder *en = NULL;

        //统计文件大小
        fseek(fp, 0, SEEK_END);
        U32 fsize = ftell(fp);
        std::cout << "original size is : " << fsize << " B\n";
        fseek(fp, 0, SEEK_SET);

        //数据块统计
        U32 fblock;
        if(BLOCK_SIZE == 0){
            fblock = 1;
        }
        else{
            fblock = (fsize >> 10) / BLOCK_SIZE;
            if(fsize % (1024 * BLOCK_SIZE) > 0){
                fblock++;
            }
        }

        U32 updateRatio = BLOCK_SIZE; //更新或者说预训练的数据占总数据的百分比，暂时使用命令行的第二个参数，其值存储在BLOCK_SIZE中
        U32 byteThreshold = fsize * updateRatio / 100;
        updateBlock = fblock * updateRatio / 100; 
        if((fblock * updateRatio) % 100 > 0){
            updateBlock++;
        }
        // std::cout << "block : " << fblock << ", dynamic update block : " << updateBlock << "\n";

        //压缩处理
        char c;
        BLOCK_SIZE *= 1024; //单位换算为Byte
        U32 byteCount = 0;
        bool notEnd = 1;
        U32 csize = 0;
        U32 sizeTmp = 0;

        U32 blockCount = 0;

        en = new Encoder(&shared, COMPRESS, out, deCom);

        clock_t start = clock();

        //动态压缩数据统计
        U32 dynamicByte;
        clock_t dynamic_t;
        
        int test_byte_count = 0;
        int last_out_size = 0;

        while(notEnd){
            // // en = new Encoder(&shared, COMPRESS, out, deCom);
            // for(int i = 0; BLOCK_SIZE == 0 || i < BLOCK_SIZE; i++){
            //     c=getc(fp);
            //     en->compressByte(c);
            //     byteCount++;
            //     if(byteCount >= fsize){
            //         notEnd = 0;
            //         break;
            //     }
            // }
            // //多余小数刷新回压缩文件中
            // // en->flush();
            // // delete en;

            // sizeTmp = csize;
            // csize = ftell(out);
            // if(shared.updateState){
            //     std::cout<<"Dynamic ";
            // }
            // else{
            //     std::cout<<"Static ";
            // }
            // std::cout<<"Block compressed size: "<<csize - sizeTmp<<"B"<<std::endl;

            // blockCount++;
            // if(blockCount >= updateBlock){
            //     shared.staticPara();
            // }

            //为不分块测试重写这一段
            if(byteCount == byteThreshold){
                csize = ftell(out);
                dynamicByte = csize;
                t = clock() - start;
                dynamic_t = t;
                shared.staticPara();
                last_out_size = dynamicByte;
            }
            c=getc(fp);
            en->compressByte(c);
            byteCount++;
            if(shared.getUpdateState() == false) {
                test_byte_count++;
                if(test_byte_count >= 1024 * 1024) {
                    csize = ftell(out) - last_out_size;
                    last_out_size = ftell(out);
                    std::cout<<"static compress " << test_byte_count << " B to " << csize << " B\t";
                    double r = ((double)test_byte_count) / csize;
                    std::cout<<"compress ratio: " << r <<std::endl;
                    test_byte_count = 0;
                }
            }
            if(byteCount >= fsize){
                en->flush(); 
                if(byteCount == byteThreshold){
                    csize = ftell(out);
                    dynamicByte = csize;
                    t = clock() - start;
                    dynamic_t = t;
                    shared.staticPara();
                }
                if(shared.getUpdateState() == false) {
                    test_byte_count++;
                    if(test_byte_count >= 10 * 1024) {
                        csize = ftell(out) - last_out_size;
                        last_out_size = ftell(out);
                        csize = ftell(out) - csize;
                        std::cout<<"static compress " << test_byte_count << " B to " << csize << " B\t";
                        double r = ((double)test_byte_count) / csize;
                        std::cout<<"compress ratio: " << r <<std::endl;
                    }
                }
                notEnd = 0;
                break;
            }
        }

        //多余小数刷新回压缩文件中
        // en->flush();
        delete en;
        
        //动态、静态压缩信息
        std::cout<<"dynamic compress " << byteThreshold << " B to " << dynamicByte << " B" <<std::endl;
        printf("dynamic compression time=%lf s\n",((float) dynamic_t)/ CLOCKS_PER_SEC);

        csize = ftell(out) - dynamicByte;
        std::cout<<"static compress " << byteCount - byteThreshold << " B to " << csize << " B" <<std::endl;
        t = clock() - start - t;
        printf("static compression time=%lf s\n",((float) t)/ CLOCKS_PER_SEC);

        //输出压缩统计信息
        csize = ftell(out);
        std::cout<<"compressed size: "<<csize<<" B"<<std::endl;
        t = clock() - start;
        printf("compression time=%lf s\n\n",((float) t)/ CLOCKS_PER_SEC);
        fclose(out);
    } else {
        FILE* out = fopen("/home/zzx/paq8test/paq8test/output.txt","r");
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
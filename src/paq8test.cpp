
#include<iostream>
#include<memory>
#include<unistd.h>
#include<fstream>
#include"Encoder.hpp"
#include"Shared.hpp"

using namespace std;

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

U32 x1=0,x2=0xffffffff;

// FILE* fp = fopen("/home/ghj/lpaq1/test/src/enwik8/enwik8","r");
FILE* fp = fopen("/home/ghj/lpaq1/test/src/calgarycorpus/book1","r");
FILE* out = fopen("/home/ghj/c++_project/compress/output.txt","w");

int main(){
    // ifstream file("/home/ghj/lpaq1/test/src/calgarycorpus/book1");
    Shared shared;
    // shared.level = 2;
    shared.init(2);
    Encoder en(&shared, COMPRESS,out);
    
    char c;
    
    while((c=getc(fp))!=EOF){
        en.compressByte(c);
    }

    fclose(fp);

}
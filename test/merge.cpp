#include <stdio.h>
#include <string>
using namespace std;

const size_t buf_size = 256;

int main(int argc,char **argv) {

    int file_num = argc - 1;

    char filePath[file_num][128];
    string foutPath("/home/zzx/data/");
    string filename;
    FILE* fin[file_num];
    FILE* fout = NULL;

    for(int i = 0; i < file_num; i++) {
        sprintf(filePath[i], "/home/zzx/data/%s", argv[1+i]);
        fin[i] = fopen(filePath[i], "r");
        if(fin[i] == NULL){
            printf("File \"%s\" no exist\n", argv[1+i]);
            i--;
            while(i >= 0) {
                fclose(fin[i]);
            }
            return 0;
        }
        foutPath += argv[1+i][0];
        filename += argv[1+i];
        filename += ' ';
    }

    fout = fopen(foutPath.c_str(), "w");
    filename += "\n\n";
    fwrite(filename.c_str(), 1, filename.size(), fout);

    char buf[257];
    size_t r_size, w_size;
    for(int i = 0; i < file_num; ) {
        r_size = w_size = -1;
        r_size = fread(buf, 1, buf_size, fin[i]);
        w_size = fwrite(buf, 1, r_size, fout);
        if(r_size != w_size) {
            printf("something wrong with read and write file \"%s\"\n", argv[1+i]);
        }
        if(r_size < buf_size) {
            i++;
        }
    }

    for(int i = 0; i < file_num; i++) {
        fclose(fin[i]);
    }
    fclose(fout);

    return 0;
}
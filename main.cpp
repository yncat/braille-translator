#include "base.h"
#include "analyser.h"
void show_file(string);
int main(int argc, char* argv[]){
if(argc!=2) {
printf("invalid parameter\n");
return 0;
}
if(strncmp(argv[1],"show",4)==0){
printf("showing original:\n");
show_file("test.txt");
return 0;
}

analyser a;
a.load("test.txt");
a.generateTokens();
a.analyseTokens();
a.extractTokens();
if(strcmp(argv[1],"braille")==0){
printf("Reformatted for braille:\n");
show_file("debug.txt");
}
if(strcmp(argv[1],"preprocess")==0){
printf("Morphological analysis:\n");
show_file("mecab_debug.txt");
}
return 0;
}

void show_file(string name){
FILE* fp=fopen(name.c_str(),"r");
if(fp==NULL){
printf("Error: cannot read the specified file.\n");
return;
}

char buf[5000];
while(fgets(buf,sizeof(buf),fp)){
printf(buf);
}
fclose(fp);
}

#include "base.h"
#include "analyser.h"
#include <iostream>
void show_file(string);
int main(int argc, char* argv[]){
cout << "starting" << endl;
if(argc!=2) {
printf("invalid parameter\n");
return 0;
}
if(strncmp(argv[1],"show",4)==0){
printf("showing original:\n");
show_file("test.txt");
return 0;
}

cout << "initializing analyser" << endl;
analyser a;
cout << "loading test.txt" << endl;
a.load("test.txt");
cout << "generating tokens" << endl;
a.generateTokens();
cout << "analysing tokens" << endl;
a.analyseTokens();
cout << "checking commandline parameters" << endl;
if(strcmp(argv[1],"braille")==0){
cout << "extracting tokens" << endl;
a.extractTokens();
printf("Reformatted for braille:\n");
show_file("debug.txt");
}
if(strcmp(argv[1],"out")==0){
cout << "outputting to outbraille.bse" << endl;
a.outputTo("outbraille.bse");
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

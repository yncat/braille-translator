#include "parser.h"
#include <stdio.h>
#include <iostream>
outputHandler::outputHandler(){
format=FORMAT_BSE;
settings.charsPerLine=32;
settings.linesPerPage=22;
}

outputHandler::~outputHandler(){
}

int outputHandler::output(vector<brlToken> tokens, char* fname){
if(!fname) return 0;
brailleFormat_base* formatter=NULL;
switch(format){
case FORMAT_BSE:
formatter=new brailleFormat_BSE();
break;
}
if(!formatter) return 0;
int numPages=1;
int numLines=0;
BCSTR ln, tmp;
for(auto itr=tokens.begin();itr!=tokens.end();++itr){
tmp=formatter->getBrailleCode((*itr).read);
if(ln.size()+tmp.size()>settings.charsPerLine){
BCSTR l=ln+tmp;
parsed_lines.push_back(l);
numLines++;
if(numLines==settings.linesPerPage) numPages++;
}//if
ln+=tmp;
}//for
BCSTR out;
out.reserve(1024*100);
for(auto itr=parsed_lines.begin();itr!=parsed_lines.end();++itr){
out+=(*itr);
}
FILE* fp=fopen(fname,"r");
if(!fp) return 0;
int headerSize;
char* header=formatter->getHeaderPtr(settings.charsPerLine,settings.linesPerPage,numPages,&headerSize);
fwrite(header,headerSize,1,fp);
fwrite(out.c_str(),out.size(),1,fp);
free(header);
fclose(fp);
}//func

BCSTR brailleFormat_BSE::getBrailleCode(BCSTR str){
}

char* brailleFormat_BSE::getHeaderPtr(int charsParLine, int linesPerPage, int numPages, int* out_headerSize){
}

void stringutil::set(BCSTR s){
str=s;
index=0;
}

bool stringutil::isZenkaku(int i){
BCCHAR c=str[index];
if(c>=0x81 && c<=0xfc){
if(c<=0x9f || c>=0xe0){
return true;
}
}
return false;
}

BCSTR stringutil::getAt(int i){
BCSTR ret;
if(isZenkaku(i)){
ret=str.substr(i,2);
return ret;
}else{
ret=str.substr(i,1);
return ret;
}
}

bool stringutil::checkSmallCharAfter(BCSTR c){
if(c=="キ") return true;
if(c=="ギ") return true;
if(c=="シ") return true;
if(c=="ジ") return true;
if(c=="シ") return true;
if(c=="ジ") return true;
if(c=="ニ") return true;
if(c=="フ") return true;
if(c=="ブ") return true;
if(c=="ピ") return true;
if(c=="チ") return true;
return false;
}

BCSTR stringutil::getNext(){
BCSTR ret;
if(index>=str.size()) return ret;
if(isZenkaku(index)){
ret=str.substr(index,2);
if(index!=str.size()-3 && checkSmallCharAfter(ret)){
BCSTR next=getAt(index+2);
if(next=="ャ" || next=="ュ" || next=="ョ"){
ret=str.substr(index,4);
index+=4;
return ret;
}//if
}//if
index+=2;
return ret;
}else{
ret=str.substr(index,1);
index++;
return ret;
}//if
}//func


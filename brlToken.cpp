#include "brlToken.h"
#include <iostream>

brlToken::brlToken(){
afterSpaces=0;
afterLinefeeds=0;
num=false;
alpha=false;
require36=false;
require3456=false;
require56=false;
}

void brlToken::import(BCSTR in){
vector<BCSTR> elems;
BCSTR item;
BCCHAR tmp;
for(int i=0;i<in.size();i++){
tmp=in[i];
if(tmp==','){
if (!item.empty()){
elems.push_back(item);
item.clear();
}
}else{
item+=tmp;
}
}
if (!item.empty()) elems.push_back(item);

//元の文字列を取る
int found=-1;
for(int i=0;i<elems[0].size();i++){
if(elems[0][i]=='\t'){
found=i;
break;
}else{
 str+=elems[0][i];
}
}
//品詞のタイプを撮る
type=elems[0].substr(found+1);
//サブタイプを撮る
subType=elems[1];
//活用のタイプを取る
conjugationType=elems[4];
//活用のサブタイプを取る
conjugationSubType=elems[5];

if(elems.size()>=8) read=elems[7];
else read="*";
//読みが「*」と解釈されているものはそのまま出力
if(read=="*") read=str;
if(read=="!") type="記号";
if(read==".") type="記号";
if(type!="記号") checkEnglish();
}

void brlToken::checkEnglish(){
alphaType=0;
BCUCHAR c;
alpha=true;
for(int i=0;i<read.size();i++){
c=read[i];
if(c>='A' && c<='Z') continue;
if(c>='a' && c<='z') continue;
if(c=='.' || c==',' || c=='-' || c== '\'' || c==';' || c==':') continue;
alpha=false;
break;
}
if(alpha){
if(read[0]>='A' && read[0]<='Z'){
alphaType=1;
if(read.size()>1){
if(read[1]>='A' && read[1]<='Z') alphaType=2;
}//if
}//if
}//if
}//func




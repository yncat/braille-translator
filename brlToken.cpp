#include "brlToken.h"
#include <iostream>

brlToken::brlToken(){
afterSpaces=0;
afterLinefeeds=0;
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

//���̕���������
int found=-1;
for(int i=0;i<elems[0].size();i++){
if(elems[0][i]=='\t'){
found=i;
break;
}else{
 str+=elems[0][i];
}
}
//�i���̃^�C�v���B��
type=elems[0].substr(found+1);
//�T�u�^�C�v���B��
subType=elems[1];
//���p�̃^�C�v�����
conjugationType=elems[4];
//���p�̃T�u�^�C�v�����
conjugationSubType=elems[5];

if(elems.size()>=8) read=elems[7];
else read="*";
}

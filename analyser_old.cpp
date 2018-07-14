#include "analyser.h"
#include "parser.h"
#include <iostream>

analyser::analyser(){
ifstream ifs("dic\\after_rules.dic");
if(ifs.fail()) return;
BCSTR str;
while(getline(ifs,str)){
if(str.empty()) continue;
size_t pos=str.find("\t");
BCSTR left=str.substr(0,pos);
BCSTR right=str.substr(pos+1);
dic_after_rules[left]=right;
}
}

int analyser::load(BCSTR filename){
ifstream ifs(filename.c_str());
if (ifs.fail()) return ERR_INFILE;
int len;
ifs.seekg(0,ios::end);
len = ifs.tellg ();
ifs.seekg(0,ios::beg);
BCCHAR* buf = new BCCHAR [len+1];
memset(buf,0,len+1);
ifs.read (buf, len);
txt=buf;
delete(buf);
return ERR_NONE;
}

int analyser::generateTokens(){
if(txt=="") return ERR_NOT_LOADED;
  mecab_t *mecab;
  const mecab_node_t *node;
  const BCCHAR *result;
  int i;
  size_t len;
int argc=3;
BCSTR prm1="brlcnv";
BCSTR prm2="-r";
BCSTR prm3="mecabrc";
BCCHAR* argv[3]={const_cast<BCCHAR*>(prm1.c_str()),const_cast<BCCHAR*>(prm2.c_str()),const_cast<BCCHAR*>(prm3.c_str())};
mecab=mecab_new(argc, argv);
if(!mecab) return ERR_MECAB_INSTANTIATE;
result = mecab_sparse_tostr(mecab,txt.c_str());
if(!result) return ERR_MECAB_PARSE;
BCSTR result_str=result;
  mecab_destroy(mecab);
ofstream o("mecab_debug.txt");
o << result_str;
o.close();
size_t index=0;
size_t pos;
bool brk=false;
//�g�[�N������
while(true){
if(index>=result_str.size()) break;
BCSTR item;
pos=result_str.find("\n",index);
if(pos==BCSTR_NPOS){
item=result_str.substr(index);
brk=true;
}else{
item=result_str.substr(index,pos-index);
}
if(item.find(",")==BCSTR_NPOS) break;
brlToken token;
token.import(item);
tokens.push_back(token);
if(brk) break;
index=pos+(sizeof(BCCHAR)*1);
}
return ERR_NONE;
}

int analyser::analyseTokens(){
if(tokens.size()==0) return ERR_NO_TOKENS;
brlToken *previous, *current, *next;
for(int i=0;i<tokens.size();i++){
//MessageBox(NULL,current->read.c_str(),"test",MB_OK);
if(i==0) previous=NULL;
else previous=&tokens[i-1];
if(i==tokens.size()-1) next=NULL;
else next=&tokens[i+1];
current=&tokens[i];

//�����̌�ɂ̓X�y�[�X���P����āA�Ȃ����A�u�n�v�u�w�v�́A�u���v�u�G�v�ɒu���B�������A���Ɂu�L���v������邩�A�������A�����Ă��邩�A���̃g�[�N�����u�悤�v�Ɉ�v����ꍇ�ɂ̓X�y�[�X�����Ȃ��B�܂���O�Ƃ��āA�u�Ƃ��āv�́u�Ɓ@���āv�ɒu���B
if(current->type=="����"){
if(next && next->type!="�L��" && next->type!="����" && next->read!="���E") current->afterSpaces=1;
if(current->read=="�n") current->read="��";
if(current->read=="�w") current->read="�G";
if(current->read=="�g�V�e") current->read="�g�@�V�e";
if(current->read.size()>2){
//�擪����1���������āA���ꂪ�u�j�v�A�u���v�Ȃ�A���̌�ɃX�y�[�X������B�i�ɑ΂��āv��u��ʂ��āv�Ȃǁj
BCSTR chars=current->read.substr(0,2);
if(chars=="�j") current->read.replace(0,2,"�j�@");
else if(chars=="��") current->read.replace(0,2,"���@");
}
}

//�������̌�ɂ̓X�y�[�X�P�����A�������̗�O�ƂƊ��S��v���邩�A��낪�L���̏ꍇ�E�������ɂ̓L�����Z��
if(current->type=="������"){
if(current->read!="�}�V��" && current->read!="�_�b"){
if(next && next->type!="�L��" && next->type!="������") current->afterSpaces=1;
}
}

//�g�[�N���@�u�\�m�v�Ɗ��S��v����Ƃ��̓X�y�[�X�P�BGCC��0x5c�����������Ă���Ȃ��̂ŁA������Ƃւ񂾂��ǃI�}�W�i�C�B
if(current->read=="�\\�m") current->afterSpaces=1;

//�g�[�N���@�u�\���i�v�Ɗ��S��v����Ƃ��̓X�y�[�X�P�BGCC��0x5c�����������Ă���Ȃ��̂ŁA������Ƃւ񂾂��ǃI�}�W�i�C�B
if(current->read=="�\\���i") current->afterSpaces=1;

//�L���u�B�v�̌�ɂ̓X�y�[�X�Q�A����ȊO�̌�ɂ̓X�y�[�X�P�B�������A���ʂ̂Ƃ��͖����B
if(current->type=="�L��"){
if(current->subType.find("����")==BCSTR_NPOS){
if(current->read=="�B") current->afterSpaces=2;
else current->afterSpaces=1;
}
}

if(current->type=="����"){
//���݂̃g�[�N���������ŁA���������������Ȃ�X�y�[�X�B�������A�����ł����Ă��A�ڔ������������Ă�����L�����Z���B
if(next && next->type=="����" || next->type=="����"){
if(next->subType!="�ڔ�") current->afterSpaces=1;
}
//�����ŁA�u�m�v�Ɗ��S��v������A�ȑO�̃g�[�N���̃X�y�[�X��ł������i�`�`���ꂽ�̂��@�Ȃǁj
if(previous && current->read=="�m") previous->afterSpaces=0;
}

if(current->type=="�`�e��"){
//���̂Ƃ���A�`�e���̌�ɂ͂��ׂăX�y�[�X���󂯂�
current->afterSpaces=1;
//�������A�u�i�N�v�Ɋ��S��v���A�����u���C�v�ł���΃L�����Z���B�u�G�N�X�g���[���Ȃ��炢�v�ȂǁB
if(next && current->read=="�i�N" && next->read=="���C") current->afterSpaces=0;
}

//���݂̃g�[�N�����ڑ�����������A�O��̃X�y�[�X�������I�ɂP�ɂ���
if(current->type=="�ڑ���"){
if(previous && previous->type=="����"){
 previous->afterSpaces=1;
current->afterSpaces=1;
}
}

//���̃g�[�N���������Ȃ�X�y�[�X�P��
if(next && next->type=="����") current->afterSpaces=1;

//���݂̃g�[�N���������Ȃ�X�y�[�X�P��
if(current->type=="����") current->afterSpaces=1;

if(current->type=="����"){
//���݂̃g�[�N���������ŁA���������Ȃ�X�y�[�X�P�B�i�u������́v�A�u���グ�邱�Ɓv�Ȃǁj
if(next && next->type=="����") current->afterSpaces=1;
}

//���̃g�[�N�����@����->�T�ϊ��p�̏ꍇ�̓X�y�[�X�P��
if(next && next->conjugationType.find("�T��")!=BCSTR_NPOS) current->afterSpaces=1;

//���݂̃g�[�N�����@����->�T�ϊ��p->��{�`�̏ꍇ�̓X�y�[�X�P��
if(current->conjugationType.find("�T��")!=BCSTR_NPOS && current->conjugationSubType=="��{�`") current->afterSpaces=1;

//���̃g�[�N�����u���E�v�ł���΁A�X�y�[�X��ł������B�u���O�Ȃ̂ŁA�������[�h�́u���E�v�ł����B
if(next && next->read=="���E") current->afterSpaces=0;

//�擪�Ɍ���Ȃ��u�E�v�́u�[�v�ɒu��
//�������A�����񂪁u�E�v�Ɋ��S��v���鎞�����͂��̏������΂��B
if(current->read!="�E"){
while(true){
size_t pos=current->read.find("�E",1);
if(pos==BCSTR_NPOS) break;//�E�������Ȃ����甲����
if(pos>0){
if(current->type=="����" && pos>=current->read.size()-2) break;//�����̂Ƃ��́A�Ō�́u�E�v�ł���Βu�����Ȃ��Ŕ�����
else current->read.replace(pos,2,"�[");
}
}
}else{//�u�E�v�Ɗ��S��v�Ȃ̂Łu�[�v�ɒu��
current->read="�[";
}

//�ǂ݂��u*�v�Ɖ��߂���Ă�����̂͂��̂܂܏o��
if(current->read=="*"){
 current->read=current->str;
}

//�O�̃g�[�N�������̂Ƃ��A�u�c�L�v�́u�K�c�v�ɒu��
if(previous && previous->subType=="��"){
size_t pos=current->read.find("�c�L");
if(pos!=BCSTR_NPOS) current->read.replace(pos,4,"�K�c");
}

//���݂̃g�[�N���������̂Ƃ��́A�����Ȃ�ꍇ�ɂ��X�y�[�X���󂯂Ȃ�
if(current->subType=="��") current->afterSpaces=0;

//���[����K�p�����̂ŁAdic/after_rules.dic�̒��g�̒u�������K����K�p
for(boost::unordered::unordered_map<BCSTR,BCSTR>::iterator itr=dic_after_rules.begin();itr!=dic_after_rules.end();++itr){
if(current->read==itr->first) current->read=itr->second;
}

//���[���L�q�����܂�
}
return ERR_NONE;
}

int analyser::extractTokens(){
if(tokens.size()==0) return ERR_NO_TOKENS;
read="";
for(int i=0;i<tokens.size();i++){
read+=tokens[i].read;
for(int j=0;j<tokens[i].afterSpaces;j++){
read+="�@";
}
for(int j=0;j<tokens[i].afterLinefeeds;j++){
read+="\n";
}
}
ofstream o("debug.txt");
o << read;
o.close();
return ERR_NONE;
}

int analyser::outputTo(BCSTR fname){
outputHandler h;
h.output(tokens,fname);
return 1;
}


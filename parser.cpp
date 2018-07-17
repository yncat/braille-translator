#include "parser.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
outputHandler::outputHandler(){
format=FORMAT_BSE;
settings.charsPerLine=32;
settings.linesPerPage=22;
}

outputHandler::~outputHandler(){
}

int outputHandler::output(vector<brlToken> tokens, BCSTR fname){
if(fname.empty()) return 0;
brailleFormat_base* formatter=NULL;
switch(format){
case FORMAT_BSE:
formatter=new brailleFormat_BSE();
break;
}
if(!formatter) return 0;
formatter->initialize();
numPages=1;
numLines=0;
BCSTR ln, tmp;
auto itr=tokens.begin();
int spc, lf;
bool ended=false;
while(true){
tmp.clear();
spc=0;
lf=0;
while(true){
if(itr==tokens.end()){
ended=true;
break;
}//if
if((*itr).num) tmp+=formatter->getBrailleCode("#");
tmp+=formatter->getBrailleCode((*itr).read);
if((*itr).afterSpaces>0){
spc=(*itr).afterSpaces;
lf=(*itr).afterLinefeeds;
itr++;
break;
}//if
itr++;
}//while

if(ln.size()+tmp.size()>settings.charsPerLine){
addLine(ln);
ln.clear();
}//if
for(int i=0;i<spc;i++){
tmp+=" ";
}//for
ln+=tmp;
if(lf>0){
for(int i=0;i<lf;i++){
addLine(ln);
ln.clear();
}//for
}//if
if(ended) break;
}//while
BCSTR out;
out.reserve(1024*100);
for(auto itr=parsed_lines.begin();itr!=parsed_lines.end();++itr){
out+=(*itr);
}
FILE* fp=fopen(fname.c_str(),"w");
if(!fp) return 0;
int headerSize;
char* header=formatter->getHeaderPtr(settings.charsPerLine,settings.linesPerPage,numPages,&headerSize);
fwrite(header,headerSize,1,fp);
fwrite(out.c_str(),out.size(),1,fp);
free(header);
fclose(fp);
}//func

void outputHandler::addLine(BCSTR ln){
BCSTR l=ln+"\x0a";
parsed_lines.push_back(l);
numLines++;
if(numLines==settings.linesPerPage){
 numPages++;
numLines=0;
}//if
}//func

BCSTR brailleFormat_base::getBrailleCode(BCSTR str){
BCSTR ret;
ret.reserve(100);
util.set(str);
BCSTR tmp;
while(true){
tmp=util.getNext();
if(tmp.empty()) break;
ret+=translationTable[tmp];
}
return ret;
}

void brailleFormat_BSE::initialize(){
translationTable["�A"]="A";
translationTable["�C"]="B";
translationTable["�E"]="C";
translationTable["�G"]="F";
translationTable["�I"]="I";
translationTable["�J"]="*";
translationTable["�L"]="<";
translationTable["�N"]="%";
translationTable["�P"]="$";
translationTable["�R"]="[";
translationTable["�T"]=":";
translationTable["�V"]="\\";
translationTable["�X"]="?";
translationTable["�Z"]="]";
translationTable["�\\"]="W";//�\ �̏I���̓V�K\�Ȃ̂ŃG�X�P�[�v���Ȃ��ƃo�O��
translationTable["�^"]="O";
translationTable["�`"]="R";
translationTable["�c"]="N";
translationTable["�e"]="Q";
translationTable["�g"]="T";
translationTable["�i"]="K";
translationTable["�j"]="L";
translationTable["�k"]="M";
translationTable["�l"]="P";
translationTable["�m"]="S";
translationTable["�n"]="U";
translationTable["�q"]="V";
translationTable["�t"]="X";
translationTable["�w"]="&";
translationTable["�z"]="!";
translationTable["�}"]="Z";
translationTable["�~"]="(";
translationTable["��"]="Y";
translationTable["��"]="=";
translationTable["��"]=")";
translationTable["��"]="/";
translationTable["��"]="+";
translationTable["��"]=">";
translationTable["��"]="E";
translationTable["��"]="H";
translationTable["��"]="D";
translationTable["��"]="G";
translationTable["��"]="J";
translationTable["��"]="'";
translationTable["��"]="9";
translationTable["��"]="0";
//����
translationTable["�K"]="\"*";
translationTable["�M"]="\"<";
translationTable["�O"]="\"%";
translationTable["�Q"]="\"]";
translationTable["�S"]="\"[";
translationTable["�U"]="\":";
translationTable["�W"]="\"\\";
translationTable["�Y"]="\"?";
translationTable["�["]="\"$";
translationTable["�]"]="\"W";
translationTable["�_"]="\"O";
translationTable["�a"]="\"R";
translationTable["�d"]="\"N";
translationTable["�f"]="\"Q";
translationTable["�h"]="\"T";
translationTable["�o"]="\"U";
translationTable["�r"]="\"V";
translationTable["�u"]="\"X";
translationTable["�x"]="\"&";
translationTable["�{"]="\"!";
//������
translationTable["�p"]=",U";
translationTable["�s"]=",V";
translationTable["�v"]=",X";
translationTable["�y"]=",&";
translationTable["�|"]=",!";
//4�̓_������(���Ⴋ�カ��)
translationTable["�L��"]="@*";
translationTable["�L��"]="@%";
translationTable["�L��"]="[";
translationTable["�V��"]="@:";
translationTable["�V��"]="@?";
translationTable["�V�F"]="@$";
translationTable["�V��"]="@W";
translationTable["�`��"]="@O";
translationTable["�`��"]="@N";
translationTable["�`�F"]="@Q";
translationTable["�`��"]="@T";
translationTable["�j��"]="K";
translationTable["�j��"]="M";
translationTable["�j��"]="@S";
translationTable["�q��"]="@U";
translationTable["�q��"]="@X";
translationTable["�q��"]="@!";
translationTable["�~��"]="@Z";
translationTable["�~��"]="@Y";
translationTable["�~��"]="@)";
translationTable["����"]="@E";
translationTable["����"]="@D";
translationTable["����"]="@J";
//45�̓_������(���Ⴌ�ガ��)
translationTable["�M��"]="^*";
translationTable["�M��"]="^%";
translationTable["�M��"]="[";
translationTable["�W��"]="^:";
translationTable["�W��"]="^?";
translationTable["�W�F"]="^$";
translationTable["�W��"]="^W";
translationTable["�r��"]="^U";
translationTable["�r��"]="^X";
translationTable["�r��"]="^!";
//26�̓_������(���������ӂ�)
translationTable["�E�F"]="5F";
translationTable["�E�H"]="5I";
translationTable["�c�@"]="5O";
translationTable["�e��"]="5N";
translationTable["�t�@"]="5U";
translationTable["�t�B"]="5V";
translationTable["�t�F"]="5&";
translationTable["�t�H"]="5!";
//256�̓_������(���@���F���H)
translationTable["���@"]="4U";
translationTable["���B"]="4V";
translationTable["���F"]="4&";
translationTable["���H"]="4!";
//����L��
translationTable["�b"]="1";
translationTable["�["]="3";
translationTable["�A"]=";";
translationTable["�B"]="4";
translationTable["�H"]="5";
translationTable["?"]="5";
//����
translationTable["0"]="J";
translationTable["1"]="A";
translationTable["2"]="B";
translationTable["3"]="C";
translationTable["4"]="D";
translationTable["5"]="E";
translationTable["6"]="F";
translationTable["7"]="G";
translationTable["8"]="H";
translationTable["9"]="I";
//�A���t�@�x�b�g�啶��
translationTable["A"]="A";
translationTable["B"]="B";
translationTable["C"]="C";
translationTable["D"]="D";
translationTable["E"]="E";
translationTable["F"]="F";
translationTable["G"]="G";
translationTable["H"]="H";
translationTable["I"]="I";
translationTable["J"]="J";
translationTable["K"]="K";
translationTable["L"]="L";
translationTable["M"]="M";
translationTable["N"]="N";
translationTable["O"]="O";
translationTable["P"]="P";
translationTable["Q"]="Q";
translationTable["R"]="R";
translationTable["S"]="S";
translationTable["T"]="T";
translationTable["U"]="U";
translationTable["V"]="V";
translationTable["W"]="W";
translationTable["X"]="X";
translationTable["Y"]="Y";
translationTable["Z"]="Z";
//�A���t�@�x�b�g������
translationTable["a"]="A";
translationTable["b"]="B";
translationTable["c"]="C";
translationTable["d"]="D";
translationTable["e"]="E";
translationTable["f"]="F";
translationTable["g"]="G";
translationTable["h"]="H";
translationTable["i"]="I";
translationTable["j"]="J";
translationTable["k"]="K";
translationTable["l"]="L";
translationTable["m"]="M";
translationTable["n"]="N";
translationTable["o"]="O";
translationTable["p"]="P";
translationTable["q"]="Q";
translationTable["r"]="R";
translationTable["s"]="S";
translationTable["t"]="T";
translationTable["u"]="U";
translationTable["v"]="V";
translationTable["w"]="W";
translationTable["x"]="X";
translationTable["y"]="Y";
translationTable["z"]="Z";
//���̑�
translationTable["#"]="#";//������
translationTable["/36"]="-";//�q����
translationTable["/6"]=",";//�啶����
}

char* brailleFormat_BSE::getHeaderPtr(int charsPerLine, int linesPerPage, int numPages, int* out_headerSize){
char* ret=(char*)malloc(520);//�{����514�o�C�g�Ȃ񂾂���ǁAsprintf ��515�o�C�g�ڂ�NULL����������ł��܂��āAfree ����Ɨ��������鋰�낵���o�b�t�@���ł����Ⴄ����520�ɂ��Ă�
memset(ret,0,520);
sprintf(ret+504,"%04d%d%d\x0a\x0d",numPages,charsPerLine,linesPerPage);//crlf�������I��
*out_headerSize=514;
return ret;
}

void stringutil::set(BCSTR s){
str=s;
index=0;
}

bool stringutil::isZenkaku(int i){
BCUCHAR c=str[index];
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
if(c=="�L") return true;
if(c=="�M") return true;
if(c=="�V") return true;
if(c=="�W") return true;
if(c=="�V") return true;
if(c=="�W") return true;
if(c=="�j") return true;
if(c=="�t") return true;
if(c=="�u") return true;
if(c=="�s") return true;
if(c=="�`") return true;
return false;
}

BCSTR stringutil::getNext(){
BCSTR ret;
if(index>=str.size()) return ret;
if(isZenkaku(index)){
ret=str.substr(index,2);
if(index!=str.size()-3 && checkSmallCharAfter(ret)){
BCSTR next=getAt(index+2);
if(next=="��" || next=="��" || next=="��"){
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


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
translationTable["ア"]="A";
translationTable["イ"]="B";
translationTable["ウ"]="C";
translationTable["エ"]="F";
translationTable["オ"]="I";
translationTable["カ"]="*";
translationTable["キ"]="<";
translationTable["ク"]="%";
translationTable["ケ"]="$";
translationTable["コ"]="[";
translationTable["サ"]=":";
translationTable["シ"]="\\";
translationTable["ス"]="?";
translationTable["セ"]="]";
translationTable["ソ\"]="W";//ソ の終わりはシガ\なのでエスケープしないとバグる
translationTable["タ"]="O";
translationTable["チ"]="R";
translationTable["ツ"]="N";
translationTable["テ"]="Q";
translationTable["ト"]="T";
translationTable["ナ"]="K";
translationTable["ニ"]="L";
translationTable["ヌ"]="M";
translationTable["ネ"]="P";
translationTable["ノ"]="S";
translationTable["ハ"]="U";
translationTable["ヒ"]="V";
translationTable["フ"]="X";
translationTable["ヘ"]="&";
translationTable["ホ"]="!";
translationTable["マ"]="Z";
translationTable["ミ"]="(";
translationTable["ム"]="Y";
translationTable["メ"]="=";
translationTable["モ"]=")";
translationTable["ヤ"]="/";
translationTable["ユ"]="+";
translationTable["ヨ"]=">";
translationTable["ラ"]="E";
translationTable["リ"]="H";
translationTable["ル"]="D";
translationTable["レ"]="G";
translationTable["ロ"]="J";
translationTable["ワ"]="'";
translationTable["ヲ"]="9";
translationTable["ン"]="0";
//濁音
translationTable["ガ"]="\"*";
translationTable["ギ"]="\"<";
translationTable["グ"]="\"%";
translationTable["ゲ"]="\"]";
translationTable["ゴ"]="\"[";
translationTable["ザ"]="\":";
translationTable["ジ"]="\"\\";
translationTable["ズ"]="\"?";
translationTable["ゼ"]="\"$";
translationTable["ゾ"]="\"W";
translationTable["ダ"]="\"O";
translationTable["ヂ"]="\"R";
translationTable["ヅ"]="\"N";
translationTable["デ"]="\"Q";
translationTable["ド"]="\"T";
translationTable["バ"]="\"U";
translationTable["ビ"]="\"V";
translationTable["ブ"]="\"X";
translationTable["ベ"]="\"&";
translationTable["ボ"]="\"!";
//半濁音
translationTable["パ"]=",U";
translationTable["ピ"]=",V";
translationTable["プ"]=",X";
translationTable["ペ"]=",&";
translationTable["ポ"]=",!";
//4の点をつける(きゃきゅきょ)
translationTable["キャ"]="@*";
translationTable["キュ"]="@%";
translationTable["キョ"]="[";
translationTable["シャ"]="@:";
translationTable["シュ"]="@?";
translationTable["シェ"]="@$";
translationTable["ショ"]="@W";
translationTable["チャ"]="@O";
translationTable["チュ"]="@N";
translationTable["チェ"]="@Q";
translationTable["チョ"]="@T";
translationTable["ニャ"]="K";
translationTable["ニュ"]="M";
translationTable["ニョ"]="@S";
translationTable["ヒャ"]="@U";
translationTable["ヒュ"]="@X";
translationTable["ヒョ"]="@!";
translationTable["ミャ"]="@Z";
translationTable["ミュ"]="@Y";
translationTable["ミョ"]="@)";
translationTable["リャ"]="@E";
translationTable["リュ"]="@D";
translationTable["リョ"]="@J";
//45の点をつける(ぎゃぎゅぎょ)
translationTable["ギャ"]="^*";
translationTable["ギュ"]="^%";
translationTable["ギョ"]="[";
translationTable["ジャ"]="^:";
translationTable["ジュ"]="^?";
translationTable["ジェ"]="^$";
translationTable["ジョ"]="^W";
translationTable["ビャ"]="^U";
translationTable["ビュ"]="^X";
translationTable["ビョ"]="^!";
//26の点をつける(うえうぉふぁ)
translationTable["ウェ"]="5F";
translationTable["ウォ"]="5I";
translationTable["ツァ"]="5O";
translationTable["テュ"]="5N";
translationTable["ファ"]="5U";
translationTable["フィ"]="5V";
translationTable["フェ"]="5&";
translationTable["フォ"]="5!";
//256の点をつける(ヴァヴェヴォ)
translationTable["ヴァ"]="4U";
translationTable["ヴィ"]="4V";
translationTable["ヴェ"]="4&";
translationTable["ヴォ"]="4!";
//特殊記号
translationTable["ッ"]="1";
translationTable["ー"]="3";
translationTable["、"]=";";
translationTable["。"]="4";
translationTable["？"]="5";
translationTable["?"]="5";
//数字
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
//アルファベット大文字
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
//アルファベット小文字
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
//その他
translationTable["#"]="#";//数字符
translationTable["/36"]="-";//繋ぎ符
translationTable["/6"]=",";//大文字符
}

char* brailleFormat_BSE::getHeaderPtr(int charsPerLine, int linesPerPage, int numPages, int* out_headerSize){
char* ret=(char*)malloc(520);//本当は514バイトなんだけれど、sprintf が515バイト目にNULLを書き込んでしまって、free すると落っこちる恐ろしいバッファができちゃうから520にしてる
memset(ret,0,520);
sprintf(ret+504,"%04d%d%d\x0a\x0d",numPages,charsPerLine,linesPerPage);//crlfを強制的に
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


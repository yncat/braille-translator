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
//トークン分割
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

//助詞の後にはスペースを１個入れて、なおかつ、「ハ」「ヘ」は、「ワ」「エ」に置換。ただし、次に「記号」が現れるか、助詞が連続しているか、次のトークンが「よう」に一致する場合にはスペースを入れない。また例外として、「として」は「と　して」に置換。
if(current->type=="助詞"){
if(next && next->type!="記号" && next->type!="助詞" && next->read!="ヨウ") current->afterSpaces=1;
if(current->read=="ハ") current->read="ワ";
if(current->read=="ヘ") current->read="エ";
if(current->read=="トシテ") current->read="ト　シテ";
if(current->read.size()>2){
//先頭から1文字数えて、それが「ニ」、「ヲ」なら、その後にスペースを入れる。（に対して」や「を通して」など）
BCSTR chars=current->read.substr(0,2);
if(chars=="ニ") current->read.replace(0,2,"ニ　");
else if(chars=="ヲ") current->read.replace(0,2,"ヲ　");
}
}

//助動詞の後にはスペース１個だが、いくつかの例外とと完全一致するか、後ろが記号の場合・助動詞にはキャンセル
if(current->type=="助動詞"){
if(current->read!="マショ" && current->read!="ダッ"){
if(next && next->type!="記号" && next->type!="助動詞") current->afterSpaces=1;
}
}

//トークン　「ソノ」と完全一致するときはスペース１個。GCCは0x5c問題を処理してくれないので、ちょっとへんだけどオマジナイ。
if(current->read=="ソ\ノ") current->afterSpaces=1;

//トークン　「ソンナ」と完全一致するときはスペース１個。GCCは0x5c問題を処理してくれないので、ちょっとへんだけどオマジナイ。
if(current->read=="ソ\ンナ") current->afterSpaces=1;

//記号「。」の後にはスペース２個、それ以外の後にはスペース１個。ただし、括弧のときは無視。
if(current->type=="記号"){
if(current->subType.find("括弧")==BCSTR_NPOS){
if(current->read=="。") current->afterSpaces=2;
else current->afterSpaces=1;
}
}

if(current->type=="名詞"){
//現在のトークンが名詞で、次が名詞か動詞ならスペース。ただし、名詞であっても、接尾属性が着いていたらキャンセル。
if(next && next->type=="名詞" || next->type=="動詞"){
if(next->subType!="接尾") current->afterSpaces=1;
}
//名詞で、「ノ」と完全一致したら、以前のトークンのスペースを打ち消す（～～されたのが　など）
if(previous && current->read=="ノ") previous->afterSpaces=0;
}

if(current->type=="形容詞"){
//今のところ、形容詞の後にはすべてスペースを空ける
current->afterSpaces=1;
//ただし、「ナク」に完全一致し、次が「ライ」であればキャンセル。「エクストリームなくらい」など。
if(next && current->read=="ナク" && next->read=="ライ") current->afterSpaces=0;
}

//現在のトークンが接続詞だったら、前後のスペースを強制的に１にする
if(current->type=="接続詞"){
if(previous && previous->type=="名詞"){
 previous->afterSpaces=1;
current->afterSpaces=1;
}
}

//次のトークンが副詞ならスペース１個
if(next && next->type=="副詞") current->afterSpaces=1;

//現在のトークンが副詞ならスペース１個
if(current->type=="副詞") current->afterSpaces=1;

if(current->type=="動詞"){
//現在のトークンが動詞で、次が名詞ならスペース１個。（「買える力」、「作り上げること」など）
if(next && next->type=="名詞") current->afterSpaces=1;
}

//次のトークンが　動詞->サ変活用の場合はスペース１個
if(next && next->conjugationType.find("サ変")!=BCSTR_NPOS) current->afterSpaces=1;

//現在のトークンが　動詞->サ変活用->基本形の場合はスペース１個
if(current->conjugationType.find("サ変")!=BCSTR_NPOS && current->conjugationSubType=="基本形") current->afterSpaces=1;

//次のトークンが「ヨウ」であれば、スペースを打ち消す。置換前なので、検索ワードは「ヨウ」でいい。
if(next && next->read=="ヨウ") current->afterSpaces=0;

//先頭に現れない「ウ」は「ー」に置換
//ただし、文字列が「ウ」に完全一致する時だけはこの処理を飛ばす。
if(current->read!="ウ"){
while(true){
size_t pos=current->read.find("ウ",1);
if(pos==BCSTR_NPOS) break;//ウがもうないから抜ける
if(pos>0){
if(current->type=="動詞" && pos>=current->read.size()-2) break;//動詞のときは、最後の「ウ」であれば置換しないで抜ける
else current->read.replace(pos,2,"ー");
}
}
}else{//「ウ」と完全一致なので「ー」に置換
current->read="ー";
}

//読みが「*」と解釈されているものはそのまま出力
if(current->read=="*"){
 current->read=current->str;
}

//前のトークンが数のとき、「ツキ」は「ガツ」に置換
if(previous && previous->subType=="数"){
size_t pos=current->read.find("ツキ");
if(pos!=BCSTR_NPOS) current->read.replace(pos,4,"ガツ");
}

//現在のトークンが数字のときは、いかなる場合にもスペースを空けない
if(current->subType=="数") current->afterSpaces=0;

//ルールを適用したので、dic/after_rules.dicの中身の置き換え規則を適用
for(boost::unordered::unordered_map<BCSTR,BCSTR>::iterator itr=dic_after_rules.begin();itr!=dic_after_rules.end();++itr){
if(current->read==itr->first) current->read=itr->second;
}

//ルール記述ここまで
}
return ERR_NONE;
}

int analyser::extractTokens(){
if(tokens.size()==0) return ERR_NO_TOKENS;
read="";
for(int i=0;i<tokens.size();i++){
read+=tokens[i].read;
for(int j=0;j<tokens[i].afterSpaces;j++){
read+="　";
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


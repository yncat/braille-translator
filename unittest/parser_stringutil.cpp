#include "../parser.h"
#include <iostream>
int main(){
stringutil util;
util.set("AAアイウエオ、ニャーン、キャー");
BCSTR s;
int cnt=0;
while(true){
cnt++;
s=util.getNext();
if(s.empty()) break;
cout << cnt << "文字目: " << s << endl;
if(cnt==50) break;
}//while
return 0;
}//main

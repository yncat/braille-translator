#include "../parser.h"
#include <iostream>
int main(){
stringutil util;
util.set("AA�A�C�E�G�I�A�j���[���A�L���[");
BCSTR s;
int cnt=0;
while(true){
cnt++;
s=util.getNext();
if(s.empty()) break;
cout << cnt << "������: " << s << endl;
if(cnt==50) break;
}//while
return 0;
}//main

#ifndef __BRLTOKEN_H
#define __BRLTOKEN_H
#include "base.h"
#include <string>
#include <vector>

class brlToken{
public:
BCSTR str;
BCSTR read;
BCSTR type;
BCSTR subType;
BCSTR conjugationType;
BCSTR conjugationSubType;
bool num, alpha;
int afterSpaces;
int afterLinefeeds;


brlToken();
void import(BCSTR);
};
#endif
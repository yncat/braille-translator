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
int alphaType;
bool require36;
bool require3456;
bool require56;
int afterSpaces;
int afterLinefeeds;


brlToken();
void import(BCSTR);
void checkEnglish();
};
#endif
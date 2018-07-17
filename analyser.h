#ifndef __ANALYSER_H
#include "base.h"
#define __ANALYSER_H
#include <string>
#include <list>
#include <fstream>
#include <boost/unordered_map.hpp>
#include "mecab.h"
#include "brlToken.h"
#include <sstream>
#include <vector>

#define CONTEXT_NORMAL 0
#define CONTEXT_ALPHABET 1
#define CONTEXT_NUMBER 2
class analyser{
private:
int context;
list<BCSTR> txt;
BCSTR read;
vector <brlToken> tokens;
boost::unordered_map <BCSTR, BCSTR> dic_after_rules;
bool checkNumber36(BCSTR);
public:
analyser();
int load(BCSTR);
int generateTokens();
int analyseTokens();
int extractTokens();
int outputTo(BCSTR);
};
#endif
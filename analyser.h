#ifndef __ANALYSER_H
#include "base.h"
#define __ANALYSER_H
#include <string>
#include <fstream>
#include <boost/unordered_map.hpp>
#include "mecab.h"
#include "brlToken.h"
#include <sstream>
#include <vector>

class analyser{
private:
BCSTR txt;
BCSTR read;
vector <brlToken> tokens;
boost::unordered_map <BCSTR, BCSTR> dic_after_rules;
public:
analyser();
int load(BCSTR);
int generateTokens();
int analyseTokens();
int extractTokens();

};
#endif
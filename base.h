#ifndef __BASE_H
#define __BASE_H
#include <string>
using namespace std;
typedef string BCSTR;
typedef unsigned char BCCHAR;

#define BCSTR_NPOS string::npos

#define ERR_NONE 1
#define ERR_INFILE -1
#define ERR_NOT_LOADED -2
#define ERR_MECAB_INSTANTIATE -3
#define ERR_MECAB_PARSE -4
#define ERR_NO_TOKENS -5
#endif

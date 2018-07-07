#ifndef __PARSER__
#define __PARSER__
#include "base.h"
#include <list>
#include <vector>
#include "brlToken.h"

#define FORMAT_BRL 0
#define FORMAT_BSE 1

typedef struct{
int charsPerLine;
int linesPerPage;
} outputSettings;

class outputHandler{
private:
int format;
int charsPerLine;
int linesPerPage;
outputSettings settings;
list<BCSTR> parsed_lines;
public:
outputHandler();
~outputHandler();
int output(vector<brlToken> tokens, char* fname);
int setFormat(int);
int getFormat();
int setOutputSettings(outputSettings*);
int getOutputSettings(outputSettings*);
};

class brailleFormat_base{
public:
virtual BCSTR getBrailleCode(BCSTR str)=0;
virtual char* getHeaderPtr(int charsParLine, int linesPerPage, int numPages, int* out_headerSize) =0;
};

class brailleFormat_BSE: public brailleFormat_base{
BCSTR getBrailleCode(BCSTR str);
char* getHeaderPtr(int charsParLine, int linesPerPage, int numPages, int* out_headerSize);
};

class stringutil{
private:
BCSTR str;
int index;
BCSTR getAt(int);
bool isZenkaku(int index);
bool checkSmallCharAfter(BCSTR);
public:
void set(BCSTR);
BCSTR getNext();
};

#endif


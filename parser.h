#ifndef __PARSER__
#define __PARSER__
#include "base.h"
#include <list>
#include <vector>
#include <unordered_map>
#include "brlToken.h"

#define FORMAT_BRL 0
#define FORMAT_BSE 1

#define PARSE_MODE_NORMAL 0
#define PARSE_MODE_ALPHABET 1

typedef struct{
int charsPerLine;
int linesPerPage;
} outputSettings;

class brailleFormat_base;

class outputHandler{
private:
brailleFormat_base* formatter;
int format;
int charsPerLine;
int linesPerPage;
int numLines, numPages;
outputSettings settings;
list<BCSTR> parsed_lines;
public:
outputHandler();
~outputHandler();
int output(vector<brlToken> tokens, BCSTR fname);
void addLine(BCSTR);
void addNewPage();
int setFormat(int);
int getFormat();
int setOutputSettings(outputSettings*);
int getOutputSettings(outputSettings*);
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

class brailleFormat_base{
protected:
unordered_map<BCSTR, BCSTR> translationTable;
stringutil util;
public:
virtual void initialize()=0;
virtual BCSTR getBrailleCode(BCSTR str);
virtual char* getHeaderPtr(int charsParLine, int linesPerPage, int numPages, int* out_headerSize) =0;
};

class brailleFormat_BSE: public brailleFormat_base{
void initialize();
char* getHeaderPtr(int charsParLine, int linesPerPage, int numPages, int* out_headerSize);
};

#endif


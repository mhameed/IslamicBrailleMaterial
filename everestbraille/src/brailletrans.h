#ifndef BRAILLETRANS_H
#define BRAILLETRANS_H

#define BRL_LINE_MAX 1024

typedef struct _BrlPinTranslation  BrlPinTranslation;
typedef unsigned char BrailleCell;
extern BrlPinTranslation brl2everest;
extern BrlPinTranslation everest2brl;

void   brlTransTblAdd(char ascii, BrailleCell braille);
int    brlTransTblLookup(char in, BrailleCell*out);
void   brlTransTblRead(FILE* infile);
void   brlTransTblPrint();
int    brlTransTblNumEntries();

char * brlToString(BrailleCell brl);
void   brlPinTranslate(struct _BrlPinTranslation *trans,BrailleCell in, BrailleCell * out);
void   brlWriteLine(FILE * out,BrailleCell  * buf, int len);

void fatalError(char * msg,...);
#endif

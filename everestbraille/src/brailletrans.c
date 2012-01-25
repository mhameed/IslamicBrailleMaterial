#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "brailletrans.h"

#define TRANS_TBL_MAX  1024

/**  an entry in the translation table, 'ascii' is the ascii character and
     'braille' is the binary encoded braille cell.
*/
typedef struct _TransTblEntry{
  char ascii;
  char braille;
} TransTblEntry;

/*
   a pin to pin translationn table. 
 */
struct _BrlPinTranslation {
  char pins[8];
};


BrlPinTranslation brl2everest = {{0,1,2,4,5,6,3,7}};
BrlPinTranslation everest2brl ={{0,1,2,6,3,4,5,7}};

/** The translation table is an array of TransTblEntry structures.
    for the moment this is a static array.
*/
TransTblEntry transTbl[TRANS_TBL_MAX];
int numTransEntries = 0;

/** helper function which resembles printf, and then dies, printing
    the an error to stderr
*/
void fatalError(char * msg,...){
  va_list ap; 

  va_start(ap,msg);  
  vfprintf(stderr,msg,ap);
  fprintf(stderr,"\n");
  va_end(ap);
  exit(1);
}


void brlTransTblAdd(char ascii, BrailleCell braille){
  if(numTransEntries >=TRANS_TBL_MAX){
    fatalError("too many entries in the translation table");
  }
  transTbl[numTransEntries].ascii = ascii;
  transTbl[numTransEntries].braille = braille;
  numTransEntries++;
}

/** performs a lookup on the translation table, looks up character "in"
    and sets the value of char "out" to the value found in the translation
    table.
    
    Returns nonzero if the "in" was found in the translation table, or
    zero if "in" was not found in the translation table
*/
int brlTransTblLookup(char in, BrailleCell*out){
  int i;
  for(i = 0; i < numTransEntries;i++){
    if(transTbl[i].ascii==in){
      *out = (BrailleCell)transTbl[i].braille;
      //      fprintf(stderr,"got %c\n",in);
      return 1;
    }
  }
  return 0;
}
 

/**
   reads a tranlsation table in from a file stream. 
*/
void brlTransTblRead(FILE* infile){
  int lineNum =1;
  int nparts;
  char  buffer[BRL_LINE_MAX];
  
   
  while(NULL!=fgets(buffer,BRL_LINE_MAX,infile)){
    char braille_chars[8];
    char ascii_val;
    int num_brailles;
    char braille_val =0;
    int i;

    nparts = sscanf(buffer,"%c,%d,[%c%c%c%c%c%c%c%c]",
		    &ascii_val,
		    &num_brailles,
		    braille_chars,
		    braille_chars+1,
		    braille_chars+2,
		    braille_chars+3,
		    braille_chars+4,
		    braille_chars+5,
		    braille_chars+6,
		    braille_chars+7);
    if(nparts!=10){
      fatalError("Unable to parse line %d of input",lineNum);
    }
    //convert braille chars to byte value
    for (i = 0;i<8;i++){
      if(braille_chars[i]!=' '){
	braille_val = braille_val | 0x1<<i;
      }
    }
    brlTransTblAdd(ascii_val,braille_val); 
  }
}

/**
   dumps a translation table to the same format as the input (to stdout)
*/ 
void brlTransTblPrint(){
  int i; 
  for(i =0;i < numTransEntries;i++){
    printf("%c,1,[%s]\n",transTbl[i].ascii,brlToString(transTbl[i].braille));
  }
}

int    brlTransTblNumEntries(){
  return numTransEntries;
}

/**
debugging, prints a braille cell to the output
output string is of the form "1 34 5 7 "
*/

char * brlToString(BrailleCell brl){
  static char buf[9];
  int i;
  
  for(i=0;i<8;i++){
    if(brl & 1<<i)
     buf[i] = '0' + i+1;
    else
      buf[i] = ' ';
  }
  buf[8] = 0;
  return buf;
}


/*
  translates braille cells from one order to another, 
*/
void brlPinTranslate(BrlPinTranslation *trans,BrailleCell in, BrailleCell * out){
  *out =0;
  unsigned char one =1;
  int i;
  for(i=0;i<8;i++){
    if(in & (one<<i))
      *out|=one<<trans->pins[i];
  }
}


void   brlWriteLine(FILE * outf,BrailleCell  * buf, int len){
  int i;
  unsigned char y = len/255;
  unsigned char x = len%255;
  putc(27,outf);
  putc(92,outf);
  putc(x,outf);
  putc(y,outf);

  for(i = 0;i<len;i++){
    char out;
    brlPinTranslate(&brl2everest,buf[i],&out);
    putc(out,outf);
  }
  putc(10,outf);
}

#include <stdio.h> 
#include <stdlib.h> 
#include <stdarg.h> 

#include "brailletrans.h"

int main(int argc,char** argv){
  const char * transfilename ;
  int readc;
  if(argc !=2){
    fatalError("Usage: %s <translation_file> ",argv[0]);
  }

  transfilename = argv[1];

  FILE * transfile = fopen(transfilename,"r");  
  
  if(transfile==0){
    fatalError("Unable to open translation file \"%s\"",transfilename);
  }
  
  brlTransTblRead(transfile);

  fprintf(stderr,"Read %d entries from %s\n",brlTransTblNumEntries(),transfilename);
  
  char linebuf[BRL_LINE_MAX];
  int llen =0;
  
  while(EOF!=(readc=getc(stdin))){
    char in; char out;
    in = (char)readc;
    
    if(brlTransTblLookup(in,&out)){
      linebuf[llen++] = out;
    }else{
      if(in==10){
	brlWriteLine(stdout,linebuf,llen);
	llen = 0;
      }else{
	fprintf(stderr,"missed char %c (%d)\n",in,in);
      }
    }
  }
  if(llen>0){
    brlWriteLine(stdout,linebuf,llen);
  }
  return 0;
}


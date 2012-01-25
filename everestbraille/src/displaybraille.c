/*
Emulates transparent mode of Everest Braille printer, and outputs
braille cells as utf8 (which may be displayed on a unicode compatible
terminal or browser)
*/

#include <stdio.h>
#include <wchar.h> 
#include <locale.h>
#include <stdarg.h>
//#define DEBUG 1
#include "brailletrans.h"

#if DEBUG
void dbg(char * msg,...){
  va_list ap;
  va_start(ap,msg);
  vfprintf(stderr,msg,ap);
  fprintf(stderr,"\n");
  va_end(ap);
}
#else

#define dbg(X,...)

/* void dbg(char * msg,...){ */
/*   va_list ap; */
/*   va_start(ap,msg); */
/*   va_end(ap); */
/* } */
#endif

typedef enum _OutputMode{
  START_TRANS1,
  START_TRANS2,
  TRANS,
  ASCII,
  GRAPHIC,
  ESC
} OutputMode;


#define ESC_TRANS 92
#define ESC_ACTIVATE_GRAPHIC 3
#define ESC_DEACTIVATE_GRAPHIC 4




int main(int argc,char ** argv){
  int c;
  setlocale(LC_ALL, "");
  OutputMode mode = ASCII;
  int trans_len;
  
  size_t ccount =0;

  while(EOF!=(c=fgetc(stdin))){
    
    switch(mode){
    case ASCII:
      if(c==27){
	mode=ESC;
      }else{
	printf("%c",c);
      }
      break;
    case ESC:
      {
	switch(c){
	case ESC_TRANS:
	  {
	    mode=START_TRANS1;
	    trans_len =0;
	  }
	  break;
	case ESC_ACTIVATE_GRAPHIC:
	  {
	    //	    mode=GRAPHIC;
	    dbg("entering graphic");
	  }
	  break;
	case ESC_DEACTIVATE_GRAPHIC:
	  {
	    //	    mode=ASCII;
	    dbg("leaving graphic");

	  }
	  break;
	default:
	  {
	    fprintf(stderr,"unknown escape %d at char %d\n",c,ccount);
	    mode=ASCII;
	  };
	}
      }
      break;
    case START_TRANS1:
      trans_len+=c;
      mode = START_TRANS2;
      break;
    case START_TRANS2:
      trans_len+=c*256;
      if(trans_len==0)
	mode=ASCII;
      else{
	mode = TRANS;
	dbg("starting transparent mode with len %d\n",trans_len);
      }
      break;
    case TRANS:
    case GRAPHIC:
      {    
	unsigned char out;
	brlPinTranslate(&everest2brl,c,&out);
	if(out==0){
	  printf(" ");
	}else{
	  wchar_t nc = 0x2800  + (wchar_t)out;
	  printf("%lc",(wint_t)nc);
	}
	if(mode==TRANS)
	  trans_len--;
      }
      break;
    default:
      fprintf(stderr,"unknown state %c\n",mode);
      mode=ASCII;
    }
   
    
    if(mode==TRANS&& trans_len==0){
      dbg("ending transpartent mode\n");
      mode=ASCII;
    }
    ccount++;
  }
  return 0;
}


/*

	For manually editing roms
	by Ron Spain

*/



#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>



typedef struct{
 char*infile;
 char*outfile;
 bool verbose;
}Opt;


Opt opt={
 .infile=0,
 .outfile=0,
 .verbose=1,
};


uint8_t*rom=0;
unsigned bytes=0;
bool go=0;


char*help=
 "\n"
 "Usage:\n"
 "./romedit -i <infile> -o <outfile> <instructions>\n"
 "Instructions are separated by comma, semicolon, or space.\n"
 "\n"
 "Options:\n"
 "-i infile\n"
 "-o outfile\n"
 "-v verbosity, 0 or 1\n"
 "-h this help\n"
 "\n"
 "Examples:\n"
 "./romedit -i inrom.sms -o outrom.sms 04fa=fb 04fb=00\n"
 "./romedit -i=rom1.sms -o=rom2.sms 04fa:fb,04fb:00\n"
 "\n";



void die(char*s){
 fflush(stdout);
 fputs("\n=== ERROR ===\n",stdout);
 perror(s);
 exit(1);
}


void*mal(int n){
 void*m=malloc(n);
 if(!m)die("malloc");
 return m;
}


FILE*fop(char*a,char*b){
 FILE*f=fopen(a,b);
 if(!f)die(a);
 return f;
}


void loadRom(char*s){
 int n,r;
 FILE*f=fop(s,"rb");
 fseek(f,0,SEEK_END);
 n=ftell(f);
 if(n<=0)die("Empty ROM");
 rewind(f);
 rom=mal(n);
 r=fread(rom,1,n,f);
 if(r!=n)die("Save failed");
 fclose(f);
 bytes=n;
}


void romedit(char*s){
 char*p;
 int a,b;
 if(*s=='-')return;
 while(1){
  a=strtol(s,0,16);
  if(a<0||a>=bytes)die("Editing out of bounds");
  p=strpbrk(s,"=:");
  if(!p)die("Bad command");
  b=strtol(++p,0,16);
  if(b<0||b>=256)die("Cannot set byte to value outside of range 0-255");
  if(opt.verbose)printf("Changing byte %06x from %02x to %02x \n",a,rom[a],b);
  rom[a]=b;
  s=strpbrk(s,",; ");
  if(!s)break;
  ++s;
 }
}


bool parseArg(char*a,char*b){
 bool r=1;

 if(a[0]!='-'){
  if(go)romedit(a);
  else return 0;
 }
 if(go)return 1;

 switch(a[1]){
  case 'h':
  puts(help);
  exit(0);
 }

 if(!b)die("No corresponding value given for option");
 if(a[1]&&a[2]){
  r=0;
  b=&a[2];
  while(*b=='=')++b;
 }

 switch(a[1]){

  case 'i':
  opt.infile=b;
  break;

  case 'o':
  opt.outfile=b;
  break;

  case 'v':
  opt.verbose=atoi(b);
  break;

  default:
  puts(a);
  die("Bad option");

 }
 return r;
}


void parseArgs(int argc,char**argv){
 bool r;
 while(--argc>0){
  ++argv;
  r=parseArg(*argv,argc?argv[1]:0);
  argc-=r;
  argv+=r;
 }
}


void saveRom(char*s){
 FILE*f=fop(s,"wb");
 int r=fwrite(rom,1,bytes,f);
 if(r!=bytes)die("Save failed");
 fclose(f);
}


int main(int argc,char**argv){
 if(argc<2){
  puts(help);
  return 1;
 }
 parseArgs(argc,argv);
 loadRom(opt.infile);
 go=1;
 parseArgs(argc,argv);
 saveRom(opt.outfile);
 return 0;
}



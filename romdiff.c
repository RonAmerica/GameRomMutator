/*

	A simple diff for roms
	Made by Ron Spain

*/


#include<stdio.h>
#include<stdlib.h>


char*help=
 "\n"
 "RomDiff - A simple diff for roms\n"
 "\n"
 "Usage:\n"
 "romdiff rom1.bin rom2.bin [outputFormat]\n"
 "Default format: \"%06x\\t%02x %02x\\n\"\n"
 "\n";

char*fmt="%06x\t%02x %02x\n";



FILE*fop(char*s){
 FILE*f=fopen(s,"rb");
 if(!f){
  perror(s);
  exit(1);
 }
 return f;
}


void romdiff(char*name1,char*name2){
 int x=0;
 int c1,c2;
 FILE*f1=fop(name1);
 FILE*f2=fop(name2);
 while(!feof(f1)&&!feof(f2)){
  c1=fgetc(f1);
  c2=fgetc(f2);
  if(c1!=c2){
   printf(fmt,x,c1,c2);
  }
  ++x;
 }
 if(!feof(f1)||!feof(f2)){
  puts("Different lengths");
 }
 fclose(f1);
 fclose(f2);
}


int main(int argc,char**argv){
 if(argc<3||argc>4){
  puts(help);
  return argc!=1;
 }
 if(argc==4)fmt=argv[3];
 romdiff(argv[1],argv[2]);
 return 0;
}


/*

	Game ROM Mutator
    Creates mutated copies of a ROM
    by W R "Ron" Spain Jr

*/


#pragma GCC optimize("Os")


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<errno.h>
#include<stdbool.h>


#define VERBOSE_DEFAULT	(1)

// default number of mutated roms to create
#define GEN_DEFAULT		(8)

// mutation rate default
#define COUNT_DEFAULT	(5.0f)

// maximum number of files for this program to open
#define FILES_MAX		(256)

// max num of roms we can generate
#define GEN_MAX			(FILES_MAX-1)


typedef struct{
 char*append;
 float rate;
 float pBegin;
 float pEnd;
 float count;
 int num;
 int begin;
 int end;
 bool verbose;
}Opt;


// setting default values
Opt opt={
 .count=COUNT_DEFAULT,
 .num=GEN_DEFAULT,
 .verbose=VERBOSE_DEFAULT,
 .append="-mutated-%d",
};


// 1st run of parseArgs is to load args, 2nd for doing work
bool doWork=0;

char*archive[]={
 "zip",
 "7z",
 "xz",
 "rar",
 "tar",
 0
};



__attribute__((cold))
void die(char*s){
 if(s){
  fflush(stdout);
  fputs("\n== ERROR ==\n",stderr);
  if(errno)perror(s);
  else{
   fputs(s,stderr);
   putc('\n',stderr);
  }
 }
 exit(1);
}


__attribute__((cold))
void*mal(int n){
 char*m;
 m=malloc(n);
 if(!m)die("malloc");
 return m;
}


__attribute__((cold))
FILE*fop(char*nm,char*op){
 FILE*f;
 f=fopen(nm,op);
 if(!f)die(nm);
 return f;
}


__attribute__((cold))
void seedRand(){
 time_t t=time(0);
 srand48(t);
 srand(t);
 for(int x=0;x<8;++x){
  drand48();
  rand();
 }
}


__attribute__((cold))
int flen(char*s){
 FILE*f;
 int r;
 f=fopen(s,"rb");
 if(!f)die(s);
 fseek(f,0,SEEK_END);
 r=ftell(f);
 fclose(f);
 return r;
}


__attribute__((cold))
void warn(char*s){
 printf("Warning: %s \n",s);
}



#pragma GCC optimize("O2")


__attribute__((hot))
void genName(char*a,char*b,int x){
 char*p;
 int n,r;
 p=strrchr(b,'/');
 if(p)b=p+1;//ignoring dir to put in cwd
 p=strrchr(b,'.');
 if(!p)p=b+strlen(b);
 n=p-b;
 memcpy(a,b,n);
 a+=n;
 r=sprintf(a,opt.append,x);
 a+=r;
 sprintf(a,"%s",p);
}


__attribute__((hot))
char*nameLoc(int x,bool b){
 char*m;
 m=mal(32);
 sprintf(m,"%d",x);
 if(!x)strcpy(m,b?"end":"start");
 return m;
}


__attribute__((cold))
void chkExt(char*s){
 char*p,**e=archive;
 p=strrchr(s,'.');
 if(!p)return;
 ++p;
 while(*e){
  if(!strcmp(p,*e)){
   puts("You might be trying to mutate an archive. Bad.");
   break;
  }
  ++e;
 }
}


__attribute__((hot))
void mutate(char*s){
 FILE*fp[FILES_MAX];
 int mc[FILES_MAX]={0};
 int c,f,k,fl,x=0;
 int bytes;
 char*name,*fm="rb";

 c=32+strlen(s)+strlen(opt.append);
 name=mal(c);
 strcpy(name,s);

 fl=flen(name);
 if(fl<0)die("Bad file");
 if(!fl)die("Empty file");

 if(opt.pBegin>0.0f){
  opt.begin=fl*.01*opt.pBegin;
 }

<<<<<<< HEAD
 if(opt.pEnd!=0.0f){
  opt.end=fl*.01*opt.pEnd;
 }

 if(opt.end<=0){
  opt.end=fl+opt.end;
 }

 bytes=opt.end-opt.begin;
=======
 if(opt.pEnd>0.0f){
  opt.end=fl*.01*opt.pEnd;
 }

 bytes=(opt.end?opt.end:fl)-opt.begin;
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e

 if(opt.count>0.0f){
  if(bytes)opt.rate=opt.count/bytes;
 }

 if(opt.verbose){
  char*a,*b;
  printf("File length: %d \n",fl);
  a=nameLoc(opt.begin,0);
  b=nameLoc(opt.end,1);
  printf("Mutating bytes range: %s to %s \n",a,b);
  printf("Mutating %d bytes \n",bytes);
  printf("Mutation rate: %f \n",opt.rate);
  printf("Anticipated mutations: %.2f \n",bytes*opt.rate);
  free(a);
  free(b);
  if(opt.end && opt.end<opt.begin)warn("Ends before start!");
  if(opt.end && opt.end==opt.begin)warn("Zero byte range!");
  if(opt.begin>=fl)warn("Starts mutating after end of file!");
<<<<<<< HEAD
  if(opt.end>fl)warn("Set to end after end of file!");
  if(opt.rate<0.0f)warn("Mutation rate is negative!");
  else if(opt.rate==0.0f)warn("Mutation rate is zero!");
  if(bytes*opt.rate>=40)warn("Mutation rate is way too high!");
  else if(bytes*opt.rate>=10)warn("Mutation rate is dangerously high!");
=======
  if(opt.end>=fl)warn("Set to end after end of file!");
  if(opt.rate<0.0f)warn("Mutation rate is negative!");
  if(opt.rate==0.0f)warn("Mutation rate is zero!");
  if(opt.rate>=0.001f)warn("Mutation rate is dangerously high!");
  if(opt.rate>=0.01f)warn("Mutation rate is too high!");
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
  if(bytes<=0)warn("No bytes to be mutated!");
  chkExt(name);
 }

 for(f=0;f<opt.num;++f){
  if(f)genName(name,s,f);
  fp[f]=fop(name,fm);
  fm="wb";
 }

 while(!feof(fp[0])){
  k=getc(fp[0]);
  if(k==EOF)break;
  for(f=1;f<opt.num;++f){
   c=k;
<<<<<<< HEAD
   if(x>=opt.begin && x<opt.end && drand48()<opt.rate){
=======
   if(x>=opt.begin && (!opt.end || x<opt.end) && drand48()<opt.rate){
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
    ++mc[f];
    if(1&rand())c=rand();
    else c+=2*(1&rand())-1;
    c&=255;
   }
   putc(c,fp[f]);
  }
  ++x;
 }

 for(f=0;f<opt.num;++f){
  fclose(fp[f]);
  if(!f)continue;
  genName(name,s,f);
  if(opt.verbose)printf(" #%d %s Mutations=%d ",f,name,mc[f]);
  if(!mc[f]){
   remove(name);
   if(opt.verbose)printf("Removed");
  }
  puts(" ");
 }
}


#pragma GCC optimize("Os")


__attribute__((cold))
void showHelp(){
 puts(
  "Game Rom Mutator by W R \"Ron\" Spain Jr\n\n"
  "\tOptions:\n"
  "-n number of mutated roms to create\n"
  "-b at which byte to begin mutating\n"
  "-B location as percent to begin mutating, overrides -b\n"
<<<<<<< HEAD
  "-e at which byte to end mutating, <=0 from end of file\n"
=======
  "-e at which byte to end mutating, 0 for end of file\n"
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
  "-E location as percent to end mutating, overrides -e\n"
  "-v set verbosity 1 or 0\n"
  "-r rate of mutation e.g. 0.0001\n"
  "-c avg mutation count per rom e.g. 5.2, overrides -r\n"
  "-a append string to file names e.g. \"-mutated-%d\"\n"
  "-h or -? for this help\n"
 );
}


__attribute__((cold))
bool parseArg(char*a,char*b){
 int i=0;
 float f=0;
<<<<<<< HEAD
 bool r=1;
=======
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
 if(a[0]!='-'){
  if(doWork)mutate(a);
  return 0;
 }
 switch(a[1]){
<<<<<<< HEAD
  case 0:
=======
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
  case 'h':
  case '?':
  showHelp();
  die(0);
 }
<<<<<<< HEAD
 if(a[2]){
  b=&a[2];
  if(*b=='=')++b;
  r=0;
 }
 if(doWork)goto Done;
=======
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
 if(b){
  i=atoi(b);
  f=atof(b);
 }else die("No parameter given for option");
<<<<<<< HEAD
 switch(a[1]){
=======
 if(!doWork)switch(a[1]){
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e

  case 'n':
  ++i;
  if(i>GEN_MAX)die("Too many output files");
  opt.num=i;
  break;

  case 'b':
  opt.begin=i;
  break;

  case 'e':
  opt.end=i;
  break;

  case 'v':
  opt.verbose=i;
  break;

  case 'B':
  opt.pBegin=f;
  break;

  case 'E':
  opt.pEnd=f;
  break;

  case 'r':
  opt.rate=f;
  break;

  case 'c':
  opt.count=f;
  break;

  case 'a':
<<<<<<< HEAD
  opt.append=b;
  break;

  default:
  puts(a);
  die("Bad option");
  break;

 }
 Done:
 return r;
=======
  opt.append=strdup(b);
  break;

 }
 return 1;
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
}


__attribute__((cold))
void parseArgs(int argc,char**argv){
 char*a,*b;
 while(--argc>0){
  ++argv;
  a=argv[0];
  b=0;
  if(argc)b=argv[1];
  if(parseArg(a,b)){
   ++argv;
   --argc;
  }
 }
}


__attribute__((cold))
int main(int argc,char**argv){
 seedRand();
 if(argc<2){
  showHelp();
  die("Too few args");
 }
 parseArgs(argc,argv);
 doWork=1;
 parseArgs(argc,argv);
 return 0;
}



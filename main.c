#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include "dirent.h"
#include "grep.h"

#define DISPLAY 100
#define COUNT 101
#define STATUS_DEFAULT DISPLAY
#define MATCH 200
#define UNMATCH 201
#define TYPE_DEFAULT MATCH
#define YES 11
#define NO 10
#define SENSITIVE 300
#define INSENSITIVE 301
#define CASE_DEFAULT SENSITIVE
#define WHOLEWORD 400
#define NWHOLEWORD 401
#define WHOLEINSENSITIVE 45
#define PURPLE "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[0m"

typedef struct flags{
	int type; //matchword or unmatchword
	int Word; //matchwholeword or unmatchwholeword
	int Case; //insensitive or sensitive 
	int printfilename;//to print or not print the filename
	int line;//to print the linenumber of match
	int maximum;//to have a maximum num of search 
	int mpattern;// to have  multiple search from files
	int status;//display or count
	int quiet;//exits with 1 if match found without any display
	int recursion;//recursively searches in directory
	int filenameonly;//to print only the filenames of the files that have a match with the pattern
}flags;
flags check_mode(char *);
int open_file(char *,int, char *,flags,char *);
void recursion(char *,char *,int,flags,int num);

int main(int argc,char * argv[]){
	char *dir;
	int i=0,j=0,k=0,n;
	line l;
	flags f;
	int z=strlen(argv[1]);
	if(strcmp(argv[1],"--help")==0){
		printf("Usage: grep [OPTION]... PATTERN [FILE]...\nSearch for PATTERN in each FILE.\n Example: grep -i 'hello world' menu.h main.c\n \n Pattern selection and interpretation:\n -e, --regexp=PATTERN      use PATTERN for matching \n -i, --ignore-case         ignore case distinctions \n -w, --word-regexp         force PATTERN to match only whole words \n \n Miscellaneous: \n -v, --invert-match        select non-matching lines \n -V, --version             display version information and exit \n     --help                display this help text and exit \n \n Output control: \n -m, --max-count=NUM       stop after NUM selected lines \n -n, --line-number         print line number with output lines \n      --line-buffered       flush output on every line\n -H, --with-filename       print file name with output lines \n -h, --no-filename         suppress the file name prefix on output \n      --label=LABEL         use LABEL as the standard input file name prefix\n  -q, --quiet, --silent     suppress all normal output\n \n  -r, --recursive           like --directories=recurse \n  -l, --files-with-matches  print only names of FILEs with selected lines \n  -c, --count               print only a count of selected lines per FILE");
	}
	else if(argc<3){
printf("Usage: grep [OPTION]... PATTERN [FILE]... \n  Try 'grep --help' for more information.");
		return 0;
	}
	
  
	else if(argv[1][0]!='-' && strcmp(argv[1],"--help")!=0){
		i=2;
		while (i<argc){
			f.type=TYPE_DEFAULT;
			f.status=STATUS_DEFAULT;
			f.filenameonly=NO;
			if(argc>3)
				f.printfilename=YES;
			else 
				f.printfilename=NO;
			f.Case= SENSITIVE;
			open_file(argv[i],0,argv[1],f,dir);
			i++;
		}
	}
	else if(argv[1][0]=='-' && argv[1][z-1]!='m' && argv[1][1]!='r' && argv[1][1]!='l' && argv[1][1]!='e' && strcmp(argv[1],"--help")!=0){
		i=3;
		while(i<argc){
			f=check_mode(argv[1]);
			if (f.printfilename!=YES && f.printfilename!=NO){
				if(argc>4)
					f.printfilename=YES;
				else 
					f.printfilename=NO;
			}
			//printf("%d",f.printfilename);
			open_file(argv[i],0,argv[2],f,dir);
			i++;
		}
	}
	else if(argv[1][0]=='-' && (argv[1][1]=='r'||argv[1][1]=='l')){//RECURSION
		if(argc==3){
			f=check_mode(argv[1]);
			if(f.printfilename!=YES && f.printfilename!=NO)
				f.printfilename = YES;
			
			recursion(argv[2],".",0,f,0);
		}
		if(argc>3){
			f=check_mode(argv[1]);
			if(f.printfilename!=YES && f.printfilename!=NO)
				f.printfilename = YES;
			recursion(argv[2],argv[argc-1],0,f,1);
		}
	}
	else if(argv[1][0]=='-' && argv[1][z-1]=='m'){
		i=4;
		while(i<argc){
			f=check_mode(argv[1]);
			if (f.printfilename!=YES && f.printfilename!=NO){
				if(argc>5)
					f.printfilename=YES;
				else 
					f.printfilename=NO;
			}
			open_file(argv[i],atoi(argv[2]),argv[3],f,dir);
			i++;
		}
	}
	else if(argv[1][0]=='-' && argv[1][1]=='e'){//MULTIPLE PATTERN SEARCH
		//printf("2");
	
		 int i=0,k=0,l=0;
		  i=3;//location for where filenames begin
		  k=2;//location of the pattern
		  l=1;// location of the modes
		  while(i<argc){
			f=check_mode(argv[l]);
			//printf("%d",f.line);
			if (f.printfilename!=YES && f.printfilename!=NO){
				if(argc>4)
					f.printfilename=YES;
				else 
					f.printfilename=NO;
			}
			open_file(argv[i],0,argv[k],f,dir);
			i=i+3;
			k=k+3;
			l=l+3;
		}
	}
	return 0;
}
int open_file(char *filename,int num , char *pattern ,flags f,char *dir){
	if((f.recursion== YES||f.filenameonly==YES) && num==1){//for recursion in a particular directory we change the current directory to the directory
		//printf("1");
		chdir(dir);
	}
	int n,l=0,j=0,q=0,k=0,count =0,z=0;
	char ch;
	line l1;
	init(&l1);//initializes the line that stores each character in a while in a node
	FILE *fp;
	int flag=1,flag1=1,flag3=0,flag4=0;
	fp=fopen(filename,"r+");
	if(fp==NULL){
		perror("Invalid File Argument");
		exit(0);	
	}
	while((ch =getc(fp))!=EOF){
		if(ch!='\n' && ch!='\0'){
			insert(&l1,ch);
		}
		else{
			k++;
			if(f.Case==SENSITIVE){
				n=matchpattern(l1,pattern);
				if(f.maximum==YES && n && f.Word!=WHOLEWORD && f.type!=UNMATCH){//count of matched lines
					j++;
				}
				if(f.type==UNMATCH && !n){//count of number of unmatched lines
					l++;
				}
			}
			else if(f.Case==INSENSITIVE){
				n=caseinsensitive(l1,pattern);
				if(f.maximum==YES && n){//count of lines with insensitives match
					j++;
				}
				flag4=1;//mode contains insensitive search
			}
			if(f.Word==WHOLEWORD){
				n=matchwholewords(l1,pattern);
				flag3=1;
				if(f.maximum==YES && n){
					j++;
				}
			}
			if(flag3==1 && flag4==1){//returns all those line that are casesensitive as well as wholewords
				n=caseinsensitiveandwholeword(l1,pattern);
			}
			if(f.type==MATCH && (n==1)){//gives output of those lines that matches the pattern given 
				if(f.quiet==YES){
					exit(0);
				}
				if(f.status==DISPLAY){
					if(f.maximum==YES){//only prints upto 'num' lines for -m
						if(j<=num){
							if(f.printfilename==YES){
								printf(PURPLE "%s:" WHITE,filename);
								if(f.line==YES){//tp print the line number of match
									printf(CYAN "%d:" WHITE,k);
								}
							}
							traverse(l1);
						}
					}
					else{// all those flags that are not -m
						if(f.printfilename==YES){
							if(f.filenameonly==YES && flag==1){
								printf(PURPLE "%s: \n" WHITE,filename);
								flag++;//to print the filename for -l only once
							}
							if(f.filenameonly==NO){
								printf(PURPLE "%s: " WHITE,filename);
							}
							if(f.line==YES && (f.filenameonly==NO)){
								printf(CYAN "%d: " WHITE,k);
							}
						}
						else{	//if printing the filename is not required
							if(f.line==YES && f.filenameonly==NO){//to print the line number of match
								printf(CYAN "%d: " WHITE,k);
							}
						}
						
						if(f.filenameonly!=YES){
			
							traverse(l1);
						}
					}
				}
				else if(f.status=COUNT){
						count++;
				}
			}
			
			else if(f.type==UNMATCH && (n==0)){
				if(f.status==DISPLAY){
					if(f.maximum==YES){//only prints upto 'num' lines for -m which do not match the pattern
						if(l<=num){
							if(f.printfilename==YES){
	
								printf(PURPLE "%s:" WHITE,filename);

								if(f.line==YES){//toprint the line number of match

									printf(CYAN "%d:" WHITE,k);
								}
							}
							traverse(l1);
						}
					}
					else{// all those flags that are not -m
						if(f.printfilename==YES){
	
							if(f.filenameonly==YES && flag1==1){
								printf(PURPLE "%s: \n" WHITE,filename);
								flag1++;//print the filename for -l only once
							}
							if(f.filenameonly==NO){
								//printf("hi");
								printf(PURPLE "%s: " WHITE,filename);
							}
							if(f.line==YES && (f.filenameonly==NO)){//print the linenumber of the search 

								printf(CYAN "%d: " WHITE,k);
							}
						}
						else{	//if printing the filename is not required
							if(f.line==YES && f.filenameonly==NO){//to print the line number of match

								printf(CYAN "%d: " WHITE,k);
							}
						}
						
						if(f.filenameonly==NO){// if it is not to print only the filename
			
							traverse(l1);
						}
					}
				}
				else if(f.status==COUNT){
					count++;
				}
			}
			destroyLine(&l1);//destroys the existing line to intialize it to the next line
			init(&l1);//initializes the list for the next line
			
		}
	
	}
	if(f.status==COUNT){//prints the number of lines that match with a particular search
		if(f.printfilename == YES){
			printf(PURPLE "%s: " WHITE,filename);
		}
		printf("%d\n",count);
	}
}

flags check_mode(char *ch){ //checks which flages are to be applied to match with the pattern

		int i=0, j=1;
		flags f;
		char arr[13]="vciwnmelhHqr";
		while(arr[i]!='\0'){
			j=1;
			while(ch[j]!='\0'){
				if(arr[i]==ch[j]){
					
					if(i==0){
						f.type=UNMATCH; 
					}
					if(i==1){
						f.status=COUNT;
					}
					if(i==2){
						f.Case=INSENSITIVE;
					}
					if(i==3){
						f.Word=WHOLEWORD;
					}
					if(i==4){
						f.line=YES;
					}
					if(i==5){
						f.maximum=YES;
					}
					if(i==6){
						f.mpattern=YES;
					}
					if(i == 7){
						f.filenameonly=YES;
					}
					if(i == 8){
						f.printfilename=NO;
					}
					if( i == 9){
						f.printfilename=YES;
					}
					if(i == 10){
						f.quiet=YES;
					}
					if(i == 11){
						f.recursion=YES;
					}
					j=1;
					break;
				}
				else{
					j++;
				}
			}
			i++;
		}
		
		if( f.type!=UNMATCH){//if the type is not unmatch we assign it the default value
			f.type=MATCH;
		}
		if(f.Case!=INSENSITIVE){//if case is not INSENSITIVE we assign it the default value
			f.Case=SENSITIVE;
		}
		if( f.status!=COUNT){//if it does not display the count then it prints the lines
			f.status=DISPLAY;
		}
		if(f.line!=YES){// if the line number is not the be printed we assign it NO
			f.line=NO;
		}
		if(f.maximum!=YES){// if -m is not one of the flags we assign NO
			f.maximum=NO;
		}
		
		if(f.filenameonly!=YES){// if there is no need to print only filename the we assign it NO
			f.filenameonly=NO;
		}
		
		if(f.quiet!=YES){// if -q is not one of the flags we assign it NO
			f.quiet=NO;
		}
		if(f.recursion!=YES){// if recursion is not required to be performed
			f.recursion=NO;	
		}
		if(f.Word!=WHOLEWORD){// if  Word to be match is not WHOLEWORD we assign it the default value
			f.Word=NWHOLEWORD;
		}
		return f;
}
//function for recursively going through the folders and return to open_file the path of each file
void recursion( char *pattern,char *name,int indent, flags f,int num){
		char path1[1024];
		int static flag6;
		DIR *dir;
		struct dirent *entry;
		if(!(dir=opendir(name))){
			return;
		}
		while((entry= readdir(dir))!=NULL){
		  if(entry->d_type == DT_DIR){
				char path[1024];
				if((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0)){
					continue;
				}
				snprintf(path,sizeof(path), "%s/%s",name,entry->d_name);
				strcpy(path1,path);	
				flag6=1;
				recursion(pattern,path,indent+2,f,num);
			}else{
				//printf("%s",entry->d_name);
				if(strcmp(entry->d_name,"grep")!=0){
					
					if(flag6!=1){
						open_file(entry->d_name,num,pattern,f,name);
					}
					else{
						
						snprintf(path1,sizeof(path1),"%s/%s",name,entry->d_name);
						//printf("%s",path1);
						open_file(path1,num,pattern,f,name);
					}
				}
			}
		}
	
	closedir(dir);
}

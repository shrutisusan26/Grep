#include "grep.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
//int countbytes=0;
void init (line *l){
	l->head=NULL;
	l->tail=NULL;
}
void insert(line *l,char  character){
	node *newnode=(node *)malloc (sizeof(node));
	if(!newnode){
		return;
	}
	newnode->next=NULL;
	newnode->prev=NULL;
	newnode->ch[0]=character;
	newnode->ch[1]='\0';
	if(l->head==NULL && l->tail==NULL){
		l->head=newnode;
		l->tail=newnode;
		return;	
	}
	else{
		newnode->prev=l->tail;
		l->tail->next=newnode;
		l->tail=newnode;
		newnode->next=NULL;
	}
	return;
}
void traverse(line l){
	if(l.head==NULL)
		return;
	node *ptr;
	ptr=l.head;
	while(ptr->next){
		printf("%c",ptr->ch[0]);
		ptr=ptr->next;
	}
	if(ptr==l.tail){
		printf("%c \n",ptr->ch[0]);
	}
	return;
}
void destroyLine(line *l){
	node *temp;
	if (l->head==NULL && l->tail ==NULL){
		return;
	}
	else{
		while(l->tail !=l->head){
			temp=l->tail;
			l->tail=temp->prev;
			l->tail->next=NULL;
			temp->prev=NULL;
			free(temp);
		}
		if(l->tail==l->head){
			//printf("DESTROY");
			temp=l->tail;
			l->tail=NULL;
			l->head=NULL;
			free(temp);
		}
	}
}
//to lower the string 
void lower_string(char * s){
	   int c = 0;
	   while (s[c] != '\0') {
	      if (s[c] >= 'A' && s[c] <= 'Z') {
		 s[c] = s[c] + 32;
	      }
	      c++;
	   }
	return ;
}
int caseinsensitive(line l, char *pattern){
	char pattern2[100];	
	strcpy(pattern2,pattern);
	line l2;
	init(&l2);
	node *ptr1;
	if (l.head==NULL){
		return 0;
	}
	ptr1=l.head;
	while(ptr1->next){
		insert(&l2,ptr1->ch[0]);
		ptr1=ptr1->next;
	}
	if(ptr1==l.tail){
		insert(&l2,ptr1->ch[0]);
	}
	lower_string(pattern2);
	//printf("%s",pattern2);
	node *ptr;
	ptr=l2.head;
	while(ptr->next){
		lower_string(ptr->ch);
		//printf("%c",ptr->ch[0]);
		ptr=ptr->next;
	}
	if(ptr->next==NULL){
		lower_string(ptr->ch);
		//printf("%c\n",ptr->ch[0]);
	}	
	//traverse(l2);
	int i=0;
	ptr=l2.head;
	while((ptr->next)  && (pattern2[i]!='\0')){ 
		if(pattern2[i]==ptr->ch[0] ){
				ptr=ptr->next;
				i++;
				continue;
			}
			else{ 	
			      if(i!=0){
					i=0;
				}
				else{
					ptr=ptr->next;
				}
			}
		}
	if(ptr==l2.tail){
		if(pattern2[i]==ptr->ch[0])
				i++;
		
	}
	 if(pattern2[i]=='\0'){
			//printf("found");
			return 1;
		}
		else{
			return 0;
		}
}
int donotmatchpattern(line l, char *pattern){
	char pattern2[100];	
	strcpy(pattern2,pattern);
	line l2;
	init(&l2);
	node *ptr1;
	if (l.head==NULL){
		return 0;
	}
	ptr1=l.head;
	while(ptr1->next){
		insert(&l2,ptr1->ch[0]);
		ptr1=ptr1->next;
	}
	if(ptr1==l.tail){
		insert(&l2,ptr1->ch[0]);
	}
	//printf("%s",pattern2);
	node *ptr;
	int i=0;
	ptr=l2.head;
	while((ptr->next)  && (pattern2[i]!='\0')){ 
		if(pattern2[i]==ptr->ch[0] ){
				ptr=ptr->next;
				i++;
				continue;
			}
			else{ 	
			      if(i!=0){
					i=0;
				}
				else{
					ptr=ptr->next;
				}
			}
		}
	if(ptr==l2.tail){
		if(pattern2[i]==ptr->ch[0])
				i++;
	}
	 if(pattern2[i]=='\0'){
			//printf("found");
			return 0;
		}
		else{
			return 1;
		}
}
int matchwholewords(line l, char *pattern){

	char pattern2[100];	
	strcpy(pattern2,pattern);
	line l2;
	init(&l2);
	if (l.head==NULL){
		return 0;
	}
	node *ptr1;
	ptr1=l.head;
	while(ptr1->next){
		insert(&l2,ptr1->ch[0]);
		ptr1=ptr1->next;
	}
	if(ptr1==l.tail){
		insert(&l2,ptr1->ch[0]);
	}
	node *ptr;
	//traverse(l2);
	int i=0;
	ptr=l2.head;
	while((ptr->next)  && (pattern2[i]!='\0')){ 
		if(pattern2[i]==ptr->ch[0] ){
				ptr=ptr->next;
				i++;
				continue;
			}
			else{ 	
			      if(i!=0){
					i=0;
				}
				else{
					ptr=ptr->next;
				}
			}
		}
	if(ptr==l2.tail){
		if(pattern2[i]==ptr->ch[0])
				i++;
	}
	  if((pattern2[i]=='\0')&&((ptr->ch[0]==' ') || (ptr->ch[0]=='(') || (ptr->ch[0]=='/') || ptr->ch[0]==')'|| ptr->ch[0]=='<'|| ptr->ch[0]=='>'|| ptr->ch[0]=='!'|| ptr->ch[0]=='-'||ptr->ch[0]=='*'||ptr->ch[0]==','||ptr->ch[0]=='"'||ptr->ch[0]==';'||ptr->ch[0]==':'||ptr->ch[0]=='\t')){
			//printf("found");
			return 1;
		}
		else{
			return 0;
		}
}
 int matchpattern(line l, char *pattern){
	char pattern2[100];	
	strcpy(pattern2,pattern);
	if (l.head==NULL){
		return 0;
	}
	line l2;
	init(&l2);
	node *ptr1;
	ptr1=l.head;
	while(ptr1->next){
		insert(&l2,ptr1->ch[0]);
		ptr1=ptr1->next;
	}
	if(ptr1==l.tail){
		insert(&l2,ptr1->ch[0]);
	}
	node *ptr;
	//traverse(l2);
	int i=0;
	ptr=l2.head;
	while((ptr->next)  && (pattern2[i]!='\0')){ 
		if(pattern2[i]==ptr->ch[0]){
				ptr=ptr->next;
				i++;
				
				continue;
		}
		else{ 	
			      if(i!=0){
					i=0;
				}
				else{
					
					ptr=ptr->next;
					
				}
			}
		}
	if(ptr==l2.tail){
			if(pattern2[i]==ptr->ch[0])
				i++;
	}
	 if(pattern2[i]=='\0'){
			
			//printf("found");
			return 1;
		}
		else{
			return 0;
		}
}
int caseinsensitiveandwholeword(line l, char *pattern){
	char pattern2[100];	
	strcpy(pattern2,pattern);
	line l2;
	init(&l2);
	node *ptr1;
	if (l.head==NULL){
		return 0;
	}
	ptr1=l.head;
	while(ptr1->next){
		insert(&l2,ptr1->ch[0]);
		ptr1=ptr1->next;
	}
	if(ptr1==l.tail){
		insert(&l2,ptr1->ch[0]);
	}
	lower_string(pattern2);
	//printf("%s",pattern2);
	node *ptr;
	ptr=l2.head;
	while(ptr->next){
		lower_string(ptr->ch);
		//printf("%c",ptr->ch[0]);
		ptr=ptr->next;
	}
	if(ptr->next==NULL){
		lower_string(ptr->ch);
		//printf("%c\n",ptr->ch[0]);
	}	
	//traverse(l2);
	int i=0;
	ptr=l2.head;
	while((ptr->next)  && (pattern2[i]!='\0')){ 
		if(pattern2[i]==ptr->ch[0] ){
				ptr=ptr->next;
				i++;
				continue;
			}
			else{ 	
			      if(i!=0){
					i=0;
				}
				else{
					ptr=ptr->next;
				}
			}
		}
	if(ptr==l2.tail){
		if(pattern2[i]==ptr->ch[0])
				i++;
		
	}
	 if((pattern2[i]=='\0')&&((ptr->ch[0]==' ') || (ptr->ch[0]=='(') || (ptr->ch[0]=='/') || ptr->ch[0]==')'|| ptr->ch[0]=='<'|| ptr->ch[0]=='>'|| ptr->ch[0]=='!'|| ptr->ch[0]=='-'||ptr->ch[0]=='*'||ptr->ch[0]==','||ptr->ch[0]=='"'||ptr->ch[0]==';'||ptr->ch[0]==':'||ptr->ch[0]=='\t')){
			//printf("found");
			return 1;
		}
		else{
			return 0;
		}
}

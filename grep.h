typedef struct node{
	char  ch[2];
	struct node *next,*prev;
}node;
typedef struct line{
	node *head;
	node *tail;
}line;
void init(line *l);
void insert(line  *l,char character );
void traverse(line l);
void destroyLine(line *l);
int caseinsensitive(line l, char *pattern);// -i //-h // -H
void lower_string(char s[]);
int donotmatchpattern(line l, char *pattern);// -v // -H
int matchpattern(line l,char *pattern);// -c & -m -r -l -n -h -H -q
int caseinsensitiveandwholeword(line l, char *pattern);
int matchwholewords(line l,char *pattern); //-w -H -h
int maxnumsearch(line l,char *pattern); //

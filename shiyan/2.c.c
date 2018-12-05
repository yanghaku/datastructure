#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#define MAXN 100005
#define T int
#define DEFAULT_SIZE 100
typedef struct{
	int _size,_cap;
	T* _elem;
}Stack;
void init_stack(Stack* st){
	st->_size=0;
	st->_cap=DEFAULT_SIZE;
	st->_elem=(T*)malloc(sizeof(T)*st->_cap);
}
void expand(Stack* st){
	if(st->_size!=st->_cap)return;
	st->_cap<<=1;
	T* old=st->_elem;
	st->_elem=(T*)malloc(sizeof(T)*st->_cap);
	for(int i=0;i<st->_size;++i)st->_elem[i]=old[i];
	free(old);
}
void shrink(Stack *st){
	if((st->_cap>>2)<st->_size||st->_cap<DEFAULT_SIZE)return;
	st->_cap>>=1;
	T* old=st->_elem;
	st->_elem=(T*)malloc(sizeof(T)*st->_size);
	for(int i=0;i<st->_size;++i)st->_elem[i]=old[i];
	free(old);
}
void push(Stack* st,T val){
	if(st->_size==st->_cap)expand(st);
	st->_elem[st->_size++]=val;
}
T top(Stack* st){ return st->_elem[st->_size-1]; }
void pop(Stack* st){
	if(st->_size>0)--st->_size;
	if((st->_cap>>1)>=st->_size)shrink(st);
}
void destroy(Stack* st){
	free(st->_elem);
}

int check(char* s){
	Stack st;init_stack(&st);
	for(int i=0;s[i];++i){
		if(s[i]=='('||s[i]=='[')push(&st,s[i]);
		else if(s[i]==']'){
			if(st._size==0||top(&st)!='[')return 0;
			pop(&st);
		}
		else if(s[i]==')'){
			if(st._size==0||top(&st)!='(')return 0;
			pop(&st);
		}
	}
	destroy(&st);
	return st._size==0?1:0;
}
char* change_to_postfix(char* s){
	Stack st;init_stack(&st);
	static char ans[MAXN];
	int len=0;
	for(int i=0;s[i];++i){
		if(s[i]>='0'&&s[i]<='9'){
			while(s[i]>='0'&&s[i]<='9')ans[len++]=s[i++];
			ans[len++]=' ';
			--i;
		}
		else if(s[i]==')'||s[i]==']'){
			while(st._size&&top(&st)!='['&&top(&st)!='('){
				ans[len++]=top(&st);
				pop(&st);
			}
			assert(top(&st)=='['||top(&st)=='(');
			pop(&st);
		}
		else if(s[i]=='('||s[i]=='[')push(&st,s[i]);
		else if(s[i]=='*'||s[i]=='/'){
			while(st._size&&(top(&st)=='*')||top(&st)=='/'){
				ans[len++]=top(&st); pop(&st);
			}
			push(&st,s[i]);
		}
		else if(s[i]=='+'||s[i]=='-'){
			while(st._size&&(top(&st)=='+'||top(&st)=='-'||top(&st)=='*'||top(&st)=='/')){
				ans[len++]=top(&st); pop(&st);
			}
			push(&st,s[i]);
		}
	}
	while(st._size){
		ans[len++]=top(&st); pop(&st);
	}
	ans[len]='\0';
	return ans;
}
int calcular(char* s){
	s=change_to_postfix(s);
	int i=0,tmp1,tmp2;
	Stack st;init_stack(&st);
	while(s[i]){
		if(s[i]>='0'&&s[i]<='9'){
			tmp1=0;
			while(s[i]>='0'&&s[i]<='9')tmp1=(tmp1<<3)+(tmp1<<1)+s[i++]-'0';
			push(&st,tmp1);
			if(s[i]==' ')++i;
		}
		else{
			tmp2=top(&st); pop(&st);
			tmp1=top(&st); pop(&st);
			if(s[i]=='+')push(&st,tmp1+tmp2);
			else if(s[i]=='-')push(&st,tmp1-tmp2);
			else if(s[i]=='*')push(&st,tmp1*tmp2);
			else {
				if(tmp2==0){
					puts("ERROR divided by ZERO!!!!");
					exit(1);
				}
				push(&st,tmp1/tmp2);
			}
			++i;
		}
	}
	assert(st._size==1);
	tmp1=top(&st);
	destroy(&st);
	return tmp1;
}
int main(){
	static char s[MAXN];
	while(gets(s)){
		if(check(s)==0)puts("Not valued!!!!!");
		else printf("%d\n",calcular(s));
	}
	return 0;	
}
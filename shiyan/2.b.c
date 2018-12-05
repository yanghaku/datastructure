#include<stdlib.h>
#include<stdio.h>
#define T char
typedef struct{
	int _size,_cap;
	T* _elem;
}Stack;
void init_stack(Stack* st,int n){
	st->_elem=(T*)malloc(sizeof(T)*n);
	st->_size=0;
	st->_cap=n;
}
void expand(Stack* st){
	if(st->_cap!=st->_size)return;
	st->_cap<<=1;
	T* old=st->_elem;
	st->_elem=(T*)malloc(sizeof(T)*st->_cap);
	for(int i=0;i<st->_size;++i)st->_elem[i]=old[i];
	free(old);
}
T top(Stack* st){
	return st->_elem[st->_size-1];
}
void pop(Stack* st){
	if(st->_size>0)--st->_size;
}
void push(Stack* st,T val){
	if(st->_size==st->_cap)expand(st);
	st->_elem[st->_size++]=val;
}

char s[100000];
int judge(){
	Stack st;
	init_stack(&st,1000);
	for(int i=0;s[i];++i){
		if(s[i]=='('||s[i]=='[')push(&st,s[i]);
		else if(s[i]==')'){
			if(!(st._size>0&&top(&st)=='('))return 0;
			pop(&st);
		}
		else if(s[i]==']'){
			if(!(st._size>0&&top(&st)=='['))return 0;
			pop(&st);
		}
	}
	return 1;
}
int main(){
	scanf("%s",s);
	puts(judge()?"YES":"NO");
	return 0;
}
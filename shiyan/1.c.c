#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<assert.h>
typedef struct Node{
	char ch;
	struct Node* next;
}node;
typedef struct{
	int size;
	node* head;
}Set; // 有序链表(升序) 表示集合

void init(Set* st){
	st->size=0;
	st->head=(node*)malloc(sizeof(node));
	st->head->next=NULL;
	st->head->ch=0;
}
void insert(Set* st,char ch){
	if(ch<'a'||ch>'z')return;
	node *pre,*p=st->head;
	while(p->next&&(p->ch<ch)){ pre=p;p=p->next; }
	if(p->ch==ch)return;
	if(p->ch<ch){
		pre=p;
		p=p->next;
	}
	assert(pre->next==p);
	pre->next=(node*)malloc(sizeof(node));
	pre->next->next=p;
	pre->next->ch=ch;
	++st->size;
}
void print(Set* st){
	node* p=st->head->next;
	putchar('"');
	while(p){
		putchar(p->ch);
		p=p->next;
	}
	putchar('"');
	putchar('\n');
}
void destory(Set* st){
	node* p;
	while(st->head){
		p=st->head;
		st->head=st->head->next;
		free(p);
	}
}

Set Bing(Set* st1,Set* st2){
	Set ans;init(&ans);
	node *p=ans.head,*p1=st1->head->next,*p2=st2->head->next;
	while(p1&&p2){
		p->next=(node*)malloc(sizeof(node));
		p=p->next;
		if(p1->ch==p2->ch){
			p->ch=p1->ch;
			p1=p1->next;
			p2=p2->next;
		}
		else if(p1->ch<p2->ch){
			p->ch=p1->ch;
			p1=p1->next;
		}
		else{
			p->ch=p2->ch;
			p2=p2->next;
		}
	}
	if(p1==NULL)p1=p2;
	while(p1){
		p->next=(node*)malloc(sizeof(node));
		p=p->next;
		p->ch=p1->ch;
		p1=p1->next;
	}
	p->next=NULL;
	return ans;
}

Set Jiao(Set* st1,Set* st2){
	Set ans;init(&ans);
	node *p=ans.head,*p1=st1->head->next,*p2=st2->head->next;
	while(p1&&p2){
		while(p1->ch!=p2->ch){
			while(p1&&p1->ch<p2->ch)p1=p1->next;
			if(!p1)break;
			while(p2&&p2->ch<p1->ch)p2=p2->next;
			if(!p2)break;
		}
		if(!(p1&&p2))break;
		p->next=(node*)malloc(sizeof(node));
		p=p->next;
		p->ch=p1->ch;
		p1=p1->next;
		p2=p2->next;
	}
	p->next=NULL;
	return ans;
}

Set Cha(Set* st1,Set* st2){
	Set ans;init(&ans);
	node *p=ans.head,*p1=st1->head->next,*p2=st2->head->next;
	while(p1&&p2){
		while(p2&&p2->ch<p1->ch)p2=p2->next;
		if(p2&&p2->ch==p1->ch){
			p1=p1->next;
			continue;
		}
		p->next=(node*)malloc(sizeof(node));
		p=p->next;
		p->ch=p1->ch;
		p1=p1->next;
	}
	while(p1){
		p->next=(node*)malloc(sizeof(node));
		p=p->next;
		p->ch=p1->ch;
		p1=p1->next;
	}
	p->next=NULL;
	return ans;
}

int main(){
	static char s1[10000],s2[10000];
	Set st1,st2,st3,st4,st5;
	init(&st1);init(&st2);
	init(&st3);init(&st4);init(&st5);
	scanf("%s%s",s1,s2);
	for(int i=0;s1[i];++i)insert(&st1,s1[i]);
	for(int i=0;s2[i];++i)insert(&st2,s2[i]);
	printf("set1: ");print(&st1);
	printf("set2: ");print(&st2);
	st3=Bing(&st1,&st2);
	printf("set1 U set2 = ");print(&st3);
	st4=Jiao(&st1,&st2);
	printf("set1 and set2 = ");print(&st4);
	st5=Cha(&st1,&st2);
	printf("set1 - set2 = ");print(&st5);
	destory(&st1);destory(&st2);destory(&st3);
	destory(&st4);destory(&st5);
	return 0;
}
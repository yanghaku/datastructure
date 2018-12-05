#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<assert.h>
const double eps=1e-6;
#define fabs(x) ((x)<0?(-(x)):(x))
typedef struct Node{
	struct Node* next;
	double c;
	int e;
}node;
typedef struct{
	int n;
	node* head;
}polynomial;
void init(polynomial* f){
	f->n=0;
	f->head=(node*)malloc(sizeof(node));
	f->head->next=NULL;
	f->head->e=INT_MAX;
}
void insert(polynomial* f,double c,int e){ //有序插入(降序)
	node *p=f->head,*pre;
	while(p->next&&(p->e)>e){pre=p; p=p->next;}
	if(p->e==e)p->c+=c;  //不增加节点
	else{		//增加节点
		if(p->e>e){//加在p之后
			pre=p;
			p=p->next;
		}
		assert(pre->next==p);
		pre->next=(node*)malloc(sizeof(node));
		pre->next->next=p;
		pre->next->c=c;
		pre->next->e=e;
		++f->n;
	}
}

void destroy(polynomial* f){
	node* p;
	while(f->head->next){
		p=f->head;
		f->head=f->head->next;
		free(p);
	}free(f->head);
}

void print(polynomial* f){
	printf("n= %d ; f(x)=",f->n);
	node* p=f->head;
	int k=f->n;
	while(p->next){
		printf("%.2f*x^%d",p->next->c,p->next->e);
		if(--k)putchar('+');
		p=p->next;
	}printf("\n");
}

void check(polynomial* f){ //将系数为0的节点去除
	node *pre=f->head,*p=f->head->next;
	while(p){
		if(fabs(p->c)<eps){
			pre->next=p->next;
			free(p);
			p=pre->next;
			--f->n;
		}
		else{
			pre=p;
			p=p->next;
		}
	}
}

polynomial add(polynomial* f1,polynomial* f2){
	polynomial f;
	init(&f);
	node *p=f.head,*pf=f1->head->next;
	f.n=f1->n;
	while(pf){
		p->next=(node*)malloc(sizeof(node));
		p->next->c=pf->c;
		p->next->e=pf->e;
		p=p->next;
		pf=pf->next;
	}p->next=NULL;
	pf=f2->head->next;
	while(pf){
		insert(&f,pf->c,pf->e);
		pf=pf->next;
	}
	check(&f);
	return f;
}
polynomial sub(polynomial* f1,polynomial* f2){
	polynomial f;
	init(&f);
	node *p=f.head,*pf=f1->head->next;
	f.n=f1->n;
	while(pf){
		p->next=(node*)malloc(sizeof(node));
		p->next->c=pf->c;
		p->next->e=pf->e;
		p=p->next;
		pf=pf->next;
	}p->next=NULL;
	pf=f2->head->next;
	while(pf){
		insert(&f,-1*(pf->c),pf->e);
		pf=pf->next;
	}
	check(&f);
	return f;
}


int main(){
	int n,e;
	double c;
	polynomial f1,f2,f3,f4;
	while(scanf("%d",&n)!=EOF){
		init(&f1);init(&f2);
		for(int i=0;i<n;++i){
			scanf("%lf%d",&c,&e);
			insert(&f1,c,e);
		}
		scanf("%d",&n);
		for(int i=0;i<n;++i){
			scanf("%lf%d",&c,&e);
			insert(&f2,c,e);
		}
		print(&f1);print(&f2);
		f3=add(&f1,&f2);
		f4=sub(&f1,&f2);
		printf("add: ");print(&f3);
		printf("sub: ");print(&f4);
		destroy(&f1);destroy(&f2);destroy(&f3);destroy(&f4);
	}
	return 0;
}
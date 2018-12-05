#include<stdio.h>
#include<stdlib.h>
#define DEFAULT_SIZE 2
#define MONEY 1000
typedef struct{int num,time;} car;
typedef struct{int size,cap; car* elem;}Stack;

typedef struct CarNode{
	int num,time;
	struct CarNode* next;
}node;
typedef struct{int size; node *front,*end;}Queue;

void init_stack(Stack* st){
	st->size=0;
	st->cap=DEFAULT_SIZE;
	st->elem=(car*)malloc(sizeof(car)*st->cap);
}
void push_stack(Stack* st,int num,int time){
	st->elem[st->size].num=num;
	st->elem[st->size++].time=time;
}
car top_stack(Stack* st){ return st->elem[st->size-1]; }
void pop_stack(Stack* st){
	if(st->size>0)--st->size;
}
void destroy_stack(Stack* st){
	free(st->elem);
}

void init_queue(Queue* q){
	q->size=0;
	q->front=(node*)malloc(sizeof(node));
	q->front->next=q->front;
	q->end=q->front;
}
void push_queue(Queue* q,int num,int time){
	q->end->num=num;
	q->end->time=time;
	q->end->next=(node*)malloc(sizeof(node));
	q->end=q->end->next;
	q->end->next=q->front;
	++q->size;
}
car front_queue(Queue* q){
	car ans;
	ans.time=q->front->time;
	ans.num=q->front->num;
	return ans;
}
void pop_queue(Queue* q){
	if(q->size<=0)return;
	q->end->next=q->front->next;
	free(q->front);
	q->front=q->end->next;
}
void destroy_queue(Queue* q){
	node* p;
	while(q->end!=q->front){
		p=q->end;
		q->end=q->end->next;
		free(p);
	}
	free(q->front);
}

int main(){
	static char s[100];
	char op;
	int num,time;
	Stack park,tmp_stack;
	init_stack(&park);init_stack(&tmp_stack);
	Queue q;
	init_queue(&q);
	while(scanf("(\'%c\',%d,%d)",&op,&num,&time)!=EOF){
		getchar();
		if(op=='A'){
			if(park.size<DEFAULT_SIZE){
				push_stack(&park,num,time);
				printf("%d号车在停车场内%d号位置\n",num,park.size);
			}
			else{
				push_queue(&q,num,time);
				printf("%d号车在便道上%d号位置\n",num,q.size);
			}
		}
		else if(op=='D'){
			car tmp_car;
			tmp_car.num=-1;
			while(park.size!=0){
				tmp_car=top_stack(&park);
				pop_stack(&park);
				if(tmp_car.num==num)break;
				push_stack(&tmp_stack,tmp_car.num,tmp_car.time);
			}
			if(tmp_car.num!=num)
				puts("没有找到!!!!");
			else{
				printf("花费:%d\n",(time-tmp_car.time)*MONEY);
			}
			while(tmp_stack.size!=0){
				tmp_car=top_stack(&tmp_stack);
				pop_stack(&tmp_stack);
				push_stack(&park,tmp_car.num,tmp_car.time);
			}
			if(park.size<DEFAULT_SIZE&&q.size>0){
				tmp_car=front_queue(&q);
				pop_queue(&q);
				push_stack(&park,tmp_car.num,time);
			}
		}
		else break;
	}
	destroy_queue(&q);
	destroy_stack(&park);destroy_stack(&tmp_stack);
	return 0;
}

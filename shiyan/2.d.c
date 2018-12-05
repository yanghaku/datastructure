#include<stdio.h>
#include<stdlib.h>
/*
 * 节点,模拟dfs时候调用函数的栈帧,
 * 其中维护三个状态: 当前节点,当前步数,已经下一个将要递归的位置(方向)
 */
typedef struct Node{
	struct Node* next;
	struct Node* pre;
	int x,y,step,direction;
}Frame;

typedef struct{
	int _size;
	Frame* head;
	Frame* top;
}Stack;
void init_stack(Stack* st){
	st->head=(Frame*)malloc(sizeof(Frame));
	st->head->next=NULL;
	st->_size=0;
	st->top=st->head;
}
void pop(Stack* st){
	if(st->_size==0)return;
	Frame* tmp=st->top;
	st->top=st->top->pre;
	st->top->next=NULL;
	free(tmp);
	--st->_size;
}
void push(Stack* st,Frame* val){
	st->top->next=val;
	val->pre=st->top;
	st->top=val;
	val->next=NULL;
	++st->_size;
}
void delete_stack(Stack* st){
	while(st->head){
		st->top=st->head;
		st->head=st->head->next;
		free(st->top);
	}
}

struct{ //存储答案的结构体
	int x,y,direction;
}ans[10000];
char s[500][500];
int n,m;
// 定义方向, 1为向右,2为向下,3为向左,4为向上;
int dire[][2]={{0,0},{0,1},{1,0},{0,-1},{-1,0}};

int dfs(int x,int y,int step){
	s[x][y]='1';
	ans[step].x=x;
	ans[step].y=y;
	if(step&&(x==0||x==n-1||y==0||y==m-1))
		return step+1;
	for(int i=1;i<5;++i){
		x=ans[step].x+dire[i][0];
		y=ans[step].y+dire[i][1];
		if(x>0&&x<n&&y>0&&y<m&&s[x][y]=='0'){
			ans[step].direction=i;
			int tmp=dfs(x,y,step+1);
			if(tmp!=0)return tmp;
		}
	}
	s[ans[step].x][ans[step].y]='0';
	return 0;
}

int judge(int x,int y){
	Stack st;init_stack(&st);
	Frame* frame=(Frame*)malloc(sizeof(Frame));
	frame->x=x;
	frame->y=y;
	frame->step=0;
	frame->direction=1;
	push(&st,frame);//先初始化第一个栈帧并入栈
	while(st._size>0){
		frame=st.top;
		ans[frame->step].x=frame->x;
		ans[frame->step].y=frame->y;
		if(frame->step!=0&&(frame->x==0||frame->y==0||frame->x==n-1||frame->y==m-1)){
			delete_stack(&st);
			return frame->step+1; //如果找到通路,就停止搜索,返回路径长度
		}
		s[frame->x][frame->y]='1';
		while(frame->direction<5){// 寻找下一个可以到达的合法位置
			x=st.top->x+dire[frame->direction][0];
			y=st.top->y+dire[frame->direction][1];
			if(x>0&&x<n&&y>0&&y<n&&s[x][y]=='0')break;
			++frame->direction;
		}
		if(frame->direction>4){// 如果找不到下一个位置, 就回溯
			s[frame->x][frame->y]='0';
			pop(&st);
			continue;
		}
		ans[frame->step].direction=frame->direction;
		// 创建下一个栈帧
		frame=(Frame*)malloc(sizeof(Frame));
		frame->x=x;
		frame->y=y;
		frame->direction=1;
		frame->step=st.top->step+1;
		push(&st,frame);
	}
	delete_stack(&st);
	return 0;
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=0;i<n;++i)scanf("%s",s[i]);
	int tot=0;
	for(int i=0;i<n;++i)if(s[i][0]=='0'){
		tot=judge(i,0);
		if(tot!=0)break;
	}
	if(tot==0)for(int i=0;i<n;++i)if(s[i][m-1]=='0'){
		tot=judge(i,m-1);
		if(tot!=0)break;
	}
	if(tot==0)for(int i=0;i<m;++i)if(s[0][i]=='0'){
		tot=judge(0,i);
		if(tot!=0)break;
	}
	if(tot==0)for(int i=0;i<m;++i)if(s[n-1][i]=='0'){
		tot=judge(n-1,i);
		if(tot!=0)break;
	}
	if(tot==0)puts("NO Answer!");
	else{
		for(int i=0;i<tot;++i)printf("(%d,%d,%d) ",ans[i].x,ans[i].y,ans[i].direction);
		printf("\n");			
	}
	return 0;
}
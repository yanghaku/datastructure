#include<stdio.h>
#include<stdlib.h>
// 定义循环链表的节点
typedef struct Node{
	int seqnum,password;
	struct Node* next;
}person;
int main(){
	int n,m;
	scanf("%d%d",&n,&m);
	person* pre=(person*)malloc(sizeof(person));
	person* now=pre;
	scanf("%d",&(pre->password));// pre指向第一个节点
	pre->seqnum=1;
	for(int i=1;i<n;++i){
		now->next=(person*)malloc(sizeof(person));
		now=now->next;
		scanf("%d",&(now->password));
		now->seqnum=i+1;
	}now->next=pre;
	now=pre;
	while(--n){
		while(--m)pre=now,now=now->next;
		printf("out: %d\n",now->seqnum);
		m=now->password;
		pre->next=now->next;
		free(now);
		now=pre->next;
	}
	printf("out: %d\n",now->seqnum);
	printf("the last person's password: %d\n",now->password);
	free(now);
	return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int MAX=10000;
typedef struct Node{
	int num;
	struct Node* next;
	struct Node* pre;
}node;
typedef struct{
	node* head;
	node* end;
}BigNum;

BigNum zero(){
	BigNum ans;
	ans.head=(node*)malloc(sizeof(node));
	ans.end=(node*)malloc(sizeof(node));
	ans.end->next=ans.head->pre=NULL;
	ans.end->pre=ans.head;
	ans.head->next=ans.end;
	ans.end->num=ans.head->num=0;
	return ans;
}
BigNum init(char* s){
	int len=strlen(s),k,i,start;
	if(len==0)return zero(); // 当空串的时候返回零
	BigNum ans;
	ans.head=(node*)malloc(sizeof(node));
	ans.head->pre=NULL;
	ans.head->next=ans.end;
	ans.end=ans.head;
	if(s[0]=='-')ans.head->num=1,start=1;
	else ans.head->num=0,start=0;
	while(s[start]=='0'&&start<len)++start; // 去除前导零
	if(start==len)return zero(); //如果全是零, 就返回零
	for(i=len;i>start;i-=4){
		ans.end->next=(node*)malloc(sizeof(node));
		ans.end->next->pre=ans.end;
		ans.end=ans.end->next;
		ans.end->num=0;
		k=i-4;if(k<start)k=start;
		while(k<i)
			ans.end->num=(ans.end->num<<1)+(ans.end->num<<3)+s[k++]-'0';
	}
	ans.end->next=NULL;
	return ans;
}

// 销毁
void delete_num(BigNum* a){
	while(a->head->next){
		a->head=a->head->next;
		free(a->head->pre);
	}
	free(a->head);
}

// 如果 |a|>|b| 就返回1, 相等返回0, 否则返回-1
int judge(BigNum* a,BigNum* b){
	node* pa=a->head;
	node* pb=b->head;
	while(pa->next&&pb->next)pa=pa->next,pb=pb->next;
	if(pa->next==NULL&&pb->next==NULL){// 长度相等, 就从高位比较
		while(pa!=a->head&&pa->num==pb->num)pa=pa->pre,pb=pb->pre;
		if(pa==a->head)return 0;
		else if(pa->num>pb->num)return 1;
		else return -1;
	}
	else if(pa->next==NULL)return -1;
	else return 1;
}

BigNum add(BigNum* a,BigNum* b){
	BigNum ans;
	ans.head=(node*)malloc(sizeof(node));
	ans.head->pre=NULL;
	ans.end=ans.head;
	if(a->head->num==b->head->num){// 同号相加即可
		ans.head->num=a->head->num;
		node* pa=a->head->next;
		node* pb=b->head->next;
		while(pa||pb){
			ans.end->next=(node*)malloc(sizeof(node));
			ans.end->next->pre=ans.end;
			ans.end->next->num=ans.end->num/MAX;
			ans.end->num%=MAX;
			ans.end=ans.end->next;
			if(pa&&pb){
				ans.end->num+=pa->num+pb->num;
				pa=pa->next;
				pb=pb->next;
			}
			else if(pb){
				ans.end->num+=pb->num;
				pb=pb->next;
			}
			else{
				ans.end->num+=pa->num;
				pa=pa->next;
			}
		}
		if(ans.end->num>=MAX){
			ans.end->next=(node*)malloc(sizeof(node));
			ans.end->next->pre=ans.end;
			ans.end->next->num=ans.end->num/MAX;
			ans.end->num%=MAX;
			ans.end=ans.end->next;
		}
		ans.end->next=NULL;
	}
	else{ //符号相异
		int status=judge(a,b);
		if(status==1){
			BigNum* tmp=a;a=b;b=tmp; // 交换, 保证|b|>|a|
		}
		else if(status==0){
			free(ans.head);
			return zero();	
		}
		ans.head->num=b->head->num;
		node* pa=a->head->next;
		node* pb=b->head->next;
		while(pb||pa){
			ans.end->next=(node*)malloc(sizeof(node));
			ans.end->next->pre=ans.end;
			if(ans.end->num<0){
				ans.end->num+=MAX;
				ans.end->next->num=-1;
			}
			else ans.end->next->num=0;
			ans.end=ans.end->next;
			if(pb&&pa){
				ans.end->num+=pb->num-pa->num;
				pa=pa->next;
				pb=pb->next;
			}
			else{ //由于|b|>|a|, 所以另一种情况只能是pb, pa为NULL
				ans.end->num+=pb->num;
				pb=pb->next;
			}
		}
		// 去除前导零, 但是如果为零的时候不能全部去掉
		while(ans.end!=ans.head->next&&ans.end->num==0){
			ans.end=ans.end->pre;
			free(ans.end->next);
		}
		ans.end->next=NULL;
	}
	return ans;
}
void print(BigNum* number){
	if(number->head->num)putchar('-');
	printf("%d",number->end->num);
	for(node* p=number->end->pre;p!=number->head;p=p->pre)
		printf(",%04d",p->num);
}


int main(){
	static char s1[1000],s2[1000];
	BigNum bnum1,bnum2,bnum3;
	while(scanf("%s%s",s1,s2)!=EOF){
		bnum1=init(s1);bnum2=init(s2);
		bnum3=add(&bnum1,&bnum2);
		print(&bnum1);printf(" + ");print(&bnum2);
		printf(" = ");print(&bnum3);printf("\n");
		delete_num(&bnum1);delete_num(&bnum2);delete_num(&bnum3);
	}
	return 0;
}
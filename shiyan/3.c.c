#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXN 100005
struct{
	int num;
	char* email;
	char* title;
	int open_time;
	int close_time;
}Advertisement[MAXN];
int r[MAXN],tot,tmp[MAXN];

void merge_sort(int array_len,int cmp(int,int)){
	int len=1;
	while(len<array_len){
		int cnt=array_len/(len<<1);
		if(array_len%(len<<1))++cnt;
		int i,j,ll=0,rr=((len<<1)<array_len? (len<<1): array_len);
		while(cnt--){
			for(i=0;i<len;++i)tmp[i]=r[i+ll];
			for(i=0,j=ll+len;ll<rr;++ll){
				if(i>=len||(j<rr&&cmp(r[j],tmp[i])<1))r[ll]=r[j++];
				else r[ll]=tmp[i++];
			}
			rr+=(len<<1);
			if(rr>array_len)rr=array_len;
		}
		len<<=1;
	}
}

int cmp_string(char* x,char* y){
	int i=0;
	while(x[i]&&y[i]&&x[i]==y[i])++i;
	if(x[i]==0&&y[i]==0)return 0;
	else if(x[i]==0&&y[i]!=0)return -1;
	else if(x[i]!=0&&y[i]==0)return 1;
	else return x[i]>y[i]?1:-1;
}
int cmp_title(int x,int y){
	return cmp_string(Advertisement[x].title,Advertisement[y].title);
}
int cmp_email(int x,int y){
	return cmp_string(Advertisement[x].email,Advertisement[y].email);
}
int cmp_num(int x,int y){
	if(Advertisement[x].num==Advertisement[y].num)return 0;
	return Advertisement[x].num>Advertisement[y].num?1:-1;
}
void destroy(){
	for(int i=0;i<tot;++i){
		free(Advertisement[i].title);
		free(Advertisement[i].email);
	}
}
int main(){
	static char buf[1000];
	scanf("%d",&tot);
	for(int i=0;i<tot;++i){
		scanf("%s%d",buf,&Advertisement[i].num);
		Advertisement[i].title=(char*)malloc(sizeof(char)*(strlen(buf)+1));
		strcpy(Advertisement[i].title,buf);
		scanf("%s",buf);
		Advertisement[i].email=(char*)malloc(sizeof(char)*(strlen(buf)+1));
		strcpy(Advertisement[i].email,buf);
		r[i]=i;
	}
	printf(">> ");
	while(scanf("%s",buf)!=EOF){
		if(strcmp(buf,"title")==0)
			merge_sort(tot,cmp_title);
		else if(strcmp(buf,"email")==0)
			merge_sort(tot,cmp_email);
		else if(strcmp(buf,"num")==0)
			merge_sort(tot,cmp_num);
		else if(strcmp(buf,"print")==0){
			for(int i=0;i<tot;++i){
				printf("%d\n\t%s\n\t%d\n\t%s\n",i+1,Advertisement[r[i]].title,
					Advertisement[r[i]].num,Advertisement[r[i]].email);
			}
		}
		else if(strcmp(buf,"save")==0){
			FILE* fp=fopen("1.out","w");
			for(int i=0;i<tot;++i){
				fprintf(fp,"%d\n\t%s\n\t%d\n\t%s\n",i+1,Advertisement[r[i]].title,
					Advertisement[r[i]].num,Advertisement[r[i]].email);				
			}
			fclose(fp);
		}
		else break;
		printf(">> ");
	}
	destroy();
	return 0;
}
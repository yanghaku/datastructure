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

void merge(int L,int mid,int R,int cmp(int,int)){
	int tmplen=mid-L;
	for(int i=0;i<tmplen;++i)tmp[i]=r[L+i];
	for(int k=L,i=0,j=mid;k<R;++k){
		if(i>=tmplen||(j<R&&cmp(r[j],tmp[i])!=1))r[k]=r[j++];
		else r[k]=tmp[i++];
	}
}

void merge_sort(int L,int R,int cmp(int,int)){
	if(R-L<2)return;
	int mid=(L+R)>>1;
	merge_sort(L,mid,cmp);
	merge_sort(mid,R,cmp);
	merge(L,mid,R,cmp);
	return ;
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
			merge_sort(0,tot,cmp_title);
		else if(strcmp(buf,"email")==0)
			merge_sort(0,tot,cmp_email);
		else if(strcmp(buf,"num")==0)
			merge_sort(0,tot,cmp_num);
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
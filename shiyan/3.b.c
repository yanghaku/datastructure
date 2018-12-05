#include<stdio.h>
#define MAXN (1<<16)
int a[MAXN];
void Gray_iterater(int n){
	n=1<<n;
	for(int i=0;i<n;++i)
		a[i]=(i^(i>>1));
}

void Gray(int n,int l,int r){
	if(r-l<2){a[l]=n;return;}
	int mid=(l+r)>>1;
	if(n&1){
		Gray(n<<1|1,l,mid);
		Gray(n<<1,mid,r);
	}
	else{
		Gray(n<<1,l,mid);
		Gray(n<<1|1,mid,r);
	}
}
int main(){
	int n;
	while(scanf("%d",&n)!=EOF){
		Gray(0,0,1<<n);
		for(int i=0;i<(1<<n);++i)printf("%d ",a[i]);printf("\n");
		Gray_iterater(n);
		for(int i=0;i<(1<<n);++i)printf("%d ",a[i]);printf("\n");
	}
	return 0;
}


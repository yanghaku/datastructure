#include<stdio.h>
#define maxn 100005
int X[maxn],Y[maxn],ans[maxn<<1],n;

void merge(){
	int len=n<<1;
	for(int i=0,j=0,k=0;k<len;++k){
		if(j>=n||(i<n&&X[i]<Y[j]))ans[k]=X[i++];
		else ans[k]=Y[j++];
	}
}
int main(){
	scanf("%d",&n);
	for(int i=0;i<n;++i)scanf("%d",&X[i]);
	for(int i=0;i<n;++i)scanf("%d",&Y[i]);
	merge();
	printf("%d\n",(ans[n]+ans[n-1])>>1);
	return 0;
}
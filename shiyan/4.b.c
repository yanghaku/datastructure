#include<stdio.h>
#include<limits.h>
#define maxn 100005
#define min(x,y) ((x)<(y)?(x):(y))
int dp[maxn],r[maxn][maxn],n;
int main(){
	scanf("%d",&n);
	for(int i=0;i<n-1;++i){
		for(int j=i+1;j<n;++j)
			scanf("%d",&r[i][j]);
	}
	for(int i=1;i<n;++i)dp[i]=INT_MAX;
	dp[0]=0;
	for(int i=1;i<n;++i){
		for(int j=0;j<i;++j){
			dp[i]=min(dp[i],dp[j]+r[j][i]);
		}
	}
	printf("%d\n",dp[n-1]);
	return 0;
}
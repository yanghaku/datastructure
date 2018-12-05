#include<stdio.h>
#include<limits.h>
#define maxn 100005
#define min(a,b) (((a)>(b))?(b):(a))
int dp[maxn];
int T[20];

int main(){
	int n;scanf("%d",&n);
	for(int i=0;i<n;++i)scanf("%d",&T[i]);
	dp[0]=0;
	for(int i=1;i<maxn;++i){
		dp[i]=INT_MAX;
		for(int j=0;j<n;++j){
			if(i>=T[j])
				dp[i]=min(dp[i],dp[i-T[j]]);
		}
		++dp[i];
	}
	while(scanf("%d",&n)!=EOF)printf("%d\n",dp[n]);
	return 0;
}
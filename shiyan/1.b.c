#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int score1[][10]={{7,5,3,2,1},{5,3,2}};
struct{
	int man_score,woman_score;
	int item_num[300],tot;
}school[31];
struct{
	char name[21];
	int project_num;
	int rank;
	int score;
}item[300];
int tot;
struct{
	int name_num;
	int score;
	int is_man;
}person[300];
int person_num;
char s[500];
int find(char* name){
	int k=person_num;
	while(k--){
		if(strcmp(item[person[k].name_num].name,name)==0)return k;
	}
	return k;
}
int main(){
	int n,m,w;
	scanf("%d%d%d\n",&n,&m,&w);
	for(int i=0;i<n;++i)school[i].tot=0;
	tot=0;
	for(int i=0;i<m+w;++i){
		while(gets(s)){
			if(s[0]=='0'||s[0]=='1'){
				for(int j=tot-1;j>=0&&item[j].project_num==i+1;--j){
					if(s[0]=='0')item[j].score=item[j].rank>5?0:score1[0][item[j].rank-1];
					if(s[0]=='1')item[j].score=item[j].rank>3?0:score1[1][item[j].rank-1];
				}
				break;
			}
			int school_num;
			sscanf(s,"%s%d%d",item[tot].name,&school_num,&item[tot].rank);
			school[school_num].item_num[school[school_num].tot++]=tot;
			item[tot++].project_num=i+1;
		}
	}
	for(int i=1;i<=n;++i){
		printf("\nSchool %d :\n",i);
		printf("项目号\t名次\t姓名\t得分\n");
		school[i].man_score=school[i].woman_score=0;
		for(int j=0;j<school[i].tot;++j){
			int ind=school[i].item_num[j];
			printf("%d\t%d\t%s\t%d\n",item[ind].project_num,item[ind].rank,item[ind].name,item[ind].score);
			if(item[ind].project_num<=m)school[i].man_score+=item[ind].score;
			else school[i].woman_score+=item[ind].score;
		}
	}
	printf("\n团体报表:\n");
	for(int i=1;i<=n;++i)
		printf("School%d: man: %d  woman: %d\n",i,school[i].man_score,school[i].woman_score);
	person_num=0;
	for(int i=0;i<tot;++i){
		int k=find(item[i].name);
		if(k==-1){
			k=person_num++;
			person[k].name_num=i;
			person[k].is_man= item[i].project_num>m?0:1;
			person[k].score=0;
		}
		person[k].score+=item[i].score;
	}
	int man_name,man_score=-1,woman_name,woman_score=-1;
	for(int i=0;i<person_num;++i){
		if(person[i].is_man){
			if(person[i].score>man_score){
				man_score=person[i].score;
				man_name=person[i].name_num;
			}
		}
		else{
			if(person[i].score>woman_score){
				woman_score=person[i].score;
				woman_name=person[i].name_num;
			}
		}
	}
	printf("\nmost_man: %s  score: %d\n",item[man_name].name,man_score);
	printf("most_woman: %s  score: %d\n",item[woman_name].name,woman_score);
	return 0;
}
/** 输入格式:
	n m w
	然后m组, 
	姓名 学校编号 排名
	每组以 项目的类型符结束(0取前五名, 1取前三名)
	然后w组(与前m组一样)

	3 2 1
	yang 1 2
	bo 2 1
	0
	yang 1 1
	bo 2 3
	1
	girl 1 1
	0
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<direct.h>
#define COL_MAX 321
#define LINE_MAX 40
const char* direct_name="edit_tmp";
char file_name[33],buf[COL_MAX],now_block[LINE_MAX][COL_MAX],tmp[33];
int total_block=0,block_num,line_num;

FILE* open_file(){
	FILE* fp=NULL;
	scanf("%s",file_name);
	while((fp=fopen(file_name,"r"))==NULL){
		fprintf(stderr,"FILE NOT FOUND!\nplease try again: ");
		scanf("%s",file_name);
	}
	getchar(); //清除缓冲区行末换行符
	puts("open file SUCCESS!");
	return fp;
}
void divide_file(FILE* fp){
	int EOF_ERROR=0;
	mkdir(direct_name);
	for(total_block=0;!EOF_ERROR;total_block++){
		sprintf(tmp,"%s/%s%d.swap",direct_name,file_name,total_block);
		FILE* fp_tmp=fopen(tmp,"w");
		for(int i=0;i<30;++i){
			if(fgets(buf,COL_MAX,fp)){
				fprintf(fp_tmp,"%s",buf);
			}
			else{
				line_num=i;
				EOF_ERROR=1;
				break;
			}
		}
		fclose(fp_tmp);
	}
	block_num=total_block-1;
	fclose(fp);
	printf("total %d blocks; now block is %d\n",total_block,block_num+1);
	// 将当前载入活区
	fp=fopen(tmp,"r");
	for(int i=0;i<line_num;++i)
		fgets(now_block[i],COL_MAX,fp);
	fclose(fp);
}

void save_block(){
	sprintf(tmp,"%s/%s%d.swap",direct_name,file_name,block_num);
	FILE* fp=fopen(tmp,"w");
	for(int i=0;i<line_num;++i){
		fprintf(fp,"%s",now_block[i]);
	}
	fclose(fp);
}

void merge_file(){
	save_block();
	FILE *fp=fopen(file_name,"w"),*fp_tmp;
	for(int i=0;i<total_block;++i){
		sprintf(tmp,"%s/%s%d.swap",direct_name,file_name,i);
		fp_tmp=fopen(tmp,"r");
		while(fgets(buf,COL_MAX,fp_tmp)){
			fprintf(fp,"%s",buf);
		}
		fclose(fp_tmp);
		remove(tmp);
	}
	fclose(fp);
	rmdir(direct_name);
}

void change_block(int num){
	save_block();
	if(num<0||num>=total_block){
		fprintf(stderr,"block num error\n");
		return;
	}
	sprintf(tmp,"%s/%s%d.swap",direct_name,file_name,num);
	FILE* fp=fopen(tmp,"r");
	for(int i=0;i<LINE_MAX;++i){
		if(fgets(now_block[i],COL_MAX,fp)==NULL){
			line_num=i;
			break;
		}
	}
	fclose(fp);
	block_num=num;
	printf("change success! now block is %d; has %d line\n",block_num+1,line_num);
}

void insert_line(int num){
	if(num<0||num>line_num){
		fprintf(stderr,"line num error\n");
		return;
	}
	int i=line_num++;
	while(i>num){
		strcpy(now_block[i],now_block[i-1]);
		--i;
	}
	strcpy(now_block[i],buf);
	strcat(now_block[i],"\n");
}

void delete_line(int num){
	if(num<0||num>=line_num){
		fprintf(stderr,"line num error\n");
		return;
	}
	--line_num;
	while(num<line_num){
		strcpy(now_block[num],now_block[num+1]);
		++num;
	}
}

void print_block(){
	int now_line=0;
	printf("q: exit; n:next page; p:previous\n");
	while(1){
		for(int i=0;i<20&&now_line<line_num;++i){
			printf("%4d %s",now_line+1,now_block[now_line]);
			++now_line;
		}
		printf("(print)>> ");
		scanf("%s",tmp);getchar();
		if(tmp[0]=='n'){
			if(now_line>=line_num)
				printf("already last\n");
		}
		else if(tmp[0]=='p'){
			if(now_line<=20)
				printf("already first\n");
			now_line-=20;
			if(now_line<0)now_line=0;
		}
		else return;
	}
}

int main(){
	/*FILE *fp=fopen("1.in","w");
	for(int i=0;i<100;++i)fprintf(fp,"line%d\n",i+1);
	fclose(fp);*/
	static char op[100];
	int num;
	printf("welcome!\nplease input filename: ");
	divide_file(open_file());
	
	printf("the instructions: \n\tchange [line]\n\t"
		"insert [line] [text]\n\tdelete [line]\n\t"
		"print\n\texit\n>> ");

	while(scanf("%s",op)!=EOF){
		if(strcmp(op,"exit")==0){
			merge_file();
			return 0;
		}
		else if(strcmp(op,"print")==0){
			print_block();
		}
		else if(strcmp(op,"change")==0){
			scanf("%d",&num);
			change_block(num-1);
		}
		else if(strcmp(op,"insert")==0){
			scanf("%d%s",&num,buf);
			insert_line(num);
		}
		else if(strcmp(op,"delete")==0){
			scanf("%d",&num);
			delete_line(num-1);
		}
		printf(">> ");
	}
	return 0;
}

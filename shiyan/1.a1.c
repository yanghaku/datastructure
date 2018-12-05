#include<stdio.h>
#include<stdlib.h>
#define T person
typedef struct{
	int password;
	int seqnum;
}person;
typedef struct{
	int _cap;//容量
	int _size;//元素数量
	T *_elem; //数组头指针
}Vector;

/* 初始化顺序表*/
void init_vector(Vector* ve,int n){
	ve->_cap=n;
	ve->_size=0;
	ve->_elem=(T*)malloc(sizeof(T)*n);
}
// 当容量不足时扩容
void expand_vector(Vector* ve){
	if(ve->_cap>ve->_size)return;
	T* old=ve->_elem;
	ve->_cap<<=1;	//容量加倍
	ve->_elem=(T*)malloc(sizeof(T)*(ve->_cap));
	for(int i=0;i<ve->_size;++i)ve->_elem[i]=old[i];
	free(old); //释放原先的
}
// 当容量大于元素的四倍时收缩
void shrink_vector(Vector* ve){
	if(ve->_cap<(ve->_size<<2))return;
	T* old=ve->_elem;
	ve->_cap>>=1; // 容量减半
	ve->_elem=(T*)malloc(sizeof(T)*(ve->_cap));
	for(int i=0;i<ve->_size;++i)ve->_elem[i]=old[i];
	free(old);
}
// 在index上插入val
void insert_elem(Vector* ve,int index,T val){
	if(ve->_size==ve->_cap)expand_vector(ve);
	for(int i=ve->_size;i>index;--i)ve->_elem[i]=ve->_elem[i-1];
	++ve->_size;
	ve->_elem[index]=val;
}
// 将index位置上的元素删除
void delete_elem(Vector* ve,int index){
	--ve->_size;
	for(int i=index;i<ve->_size;++i)ve->_elem[i]=ve->_elem[i+1];
	//if((ve->_size<<2)<ve-_cap)shrink_vector(ve);
}
// 将数组销毁
void delete_vector(Vector* ve){
	free(ve->_elem);
}
// 将数组清空
void clear_vector(Vector* ve){
	ve->_size=0;
}
/** ========================================*/

// 模拟约瑟夫环, 返回最后一个人的密码
int joseph(Vector* ve,int m){
	int index=0;
	while(ve->_size>1){
		index=(index+m-1)%ve->_size;
		printf("out: %d\n",ve->_elem[index].seqnum);
		m=ve->_elem[index].password;
		delete_elem(ve,index);
	}
	printf("out: %d \n",ve->_elem[0].seqnum);
	return ve->_elem[0].password;
}
int main(){
	Vector seq;
	person tmp;
	int n,m;
	scanf("%d%d",&n,&m);
	init_vector(&seq,n);
	for(int i=0;i<n;++i){
		scanf("%d",&tmp.password);
		tmp.seqnum=i+1;
		insert_elem(&seq,seq._size,tmp);
	}
	printf("the last person's password: %d\n",joseph(&seq,m));
	delete_vector(&seq);
	return 0;
}

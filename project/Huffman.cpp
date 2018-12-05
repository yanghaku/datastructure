#include<iostream>
#include<cstdio>
#include<cstring>
#include<bitset>
#include<map>
#include<queue>
#include"BinTree.cpp"
using namespace std;

#define N_CHAR (0x80 - 0x20)

struct HuffChar{
	char ch;int weight;
	HuffChar(char c='^',int w=0):ch(c),weight(w){};
	bool operator<(HuffChar const& hc){return weight>hc.weight;}
	bool operator==(HuffChar const& hc){return weight==hc.weight;}
};
typedef BinTree<HuffChar> HuffTree;
struct HuffCmp{
	bool operator()(const HuffTree* t1,const HuffTree* t2)const{
		return (t1->root()->data)<(t2->root()->data);
	}
};
typedef priority_queue<HuffTree*,vector<HuffTree*>,HuffCmp> HuffForest;
typedef map<char,char*> HuffTable;
typedef bitset<1000> bitmap;

int* statistics(char* file){
	FILE* fp=fopen(file,"r");
	int* ans=new int[N_CHAR];
	for(int i=0;i<N_CHAR;++i)ans[i]=0;
	char ch;
	while(fscanf(fp,"%c",&ch)!=EOF){
		if(ch>=0)++ans[ch-0x20];
	}
	fclose(fp);
	return ans;
}

HuffForest* initForest(int* freq){
	HuffForest* res=new HuffForest;
	for(int i=0;i<N_CHAR;++i){
		HuffTree* tree=new HuffTree;
		tree->insertAsRoot(HuffChar(0x20+i,freq[i]));
		res->push(tree);
	}
	return res;
}

HuffTree* generateTree(HuffForest* forest){
	HuffTree* tree=new HuffTree;
	while(forest->size()>1){
		HuffTree* t1=forest->top();forest->pop();
		HuffTree* t2=forest->top();forest->pop();
		HuffTree* s=new HuffTree;
		s->insertAsRoot(HuffChar('^',(t1->root()->data).weight+(t2->root()->data).weight));
		s->attachAsLC(s->root(),t1);
		s->attachAsRC(s->root(),t2);
		forest->push(s);
	}
	return forest->top();
}

void DFS(BinNode<HuffChar>* x,HuffTable* table,bitmap* bitString,int len){
	if(IsLeaf(*x)){
		char *p=new char[len+1];
		for(int i=0;i<len;++i)p[i]=bitString->test(i)?'1':'0';
		p[len]=0;
		table->insert(make_pair((x->data).ch,p));
		return;
	}
	if(HasLChild(*x)){//1
		bitString->set(len);
		DFS(x->lc,table,bitString,len+1);
	}
	if(HasRChild(*x)){//0
		bitString->reset(len);
		DFS(x->rc,table,bitString,len+1);
	}
}

HuffTable* generateTable(HuffTree* tree){
	bitmap bitString;
	HuffTable* table=new HuffTable;
	DFS(tree->root(),table,&bitString,0);
	return table;
}

int encode(HuffTable* table,bitmap& bitString,char* s){
	int n=0;
	for(size_t i=0,m=strlen(s);i<m;++i){
		char* p=(*table)[s[i]];
		for(size_t j=0,k=strlen(p);j<k;++j){
			p[j]=='1'?bitString.set(n++):bitString.reset(n++);
		}
		printf("%s",p);
	}puts("");
	return n;
}
void decode(HuffTree* tree,bitmap& codeString,int n){
	BinNode<HuffChar>* x=tree->root();
	for(int i=0;i<n;++i){
		x=codeString.test(i)?x->lc:x->rc;
		if(IsLeaf(*x)){
			printf("%c",(x->data).ch);
			x=tree->root();
		}
	}
	puts("");
	return;
}
int main(int argc,char* argv[]){
	int* freq=statistics(argv[1]);
	HuffForest* forest=initForest(freq);
	HuffTree* tree=generateTree(forest);
	delete forest;
	HuffTable* table=generateTable(tree);
	for(int i='a';i<='z';++i)cerr<<char(i)<<' '<<freq[i-0x20]<<' '<<(*table)[char(i)]<<endl;
	bitmap codeString;
	for(int i=2;i<argc;++i){
		int n=encode(table,codeString,argv[i]);
		decode(tree,codeString,n);
	}
	delete freq;
	delete table;
	delete tree;
	return 0;
}

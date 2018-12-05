#include"BinTree.cpp"
#include<iostream>
#include<cstring>
#include<vector>
#include<ctime>
#include<map>
using namespace std;

typedef BinTree<char>PFCTree;
typedef vector<PFCTree*>PFCForest;
typedef map<char,char*>PFCTable;

#define N_CHAR (0x80 - 0x20)
PFCForest* initForest(){
	PFCForest* res=new PFCForest;
	for(int i=0;i<N_CHAR;++i){
		res->push_back(new PFCTree());
		(*res)[i]->insertAsRoot(i+0x20);
	}
	return res;
}
PFCTree* generateTree(PFCForest* forest){
	srand(unsigned (time(nullptr)));
	while(forest->size()>1){
		PFCTree* s=new PFCTree;
		s->insertAsRoot('#');
		size_t r=rand()%forest->size();
		s->attachAsLC(s->root(),(*forest)[r]);
		forest->erase(forest->begin()+r);
		r=rand()%forest->size();
		s->attachAsRC(s->root(),(*forest)[r]);
		forest->erase(forest->begin()+r);
		forest->push_back(s);
	}
	return (*forest)[0];
}

void dfs(PFCTable* tree,vector<bool>& bitString,BinNodePosi(char) x,int len){
	if(IsLeaf(*x)){
		char* s=new char[len+1];
		for(int i=0;i<len;++i){
			s[i]=bitString[i]?'1':'0';
		}s[len]='\0';
		tree->insert(make_pair(x->data,s));
		return;
	}
	if(HasLChild(*x)){//0
		bitString[len]=false;
		dfs(tree,bitString,x->lc,len+1);
	}
	if(HasRChild(*x)){//1
		bitString[len]=true;
		dfs(tree,bitString,x->rc,len+1);
	}
}

PFCTable* generateTable(PFCTree* tree){
	PFCTable* res=new PFCTable;
	vector<bool> ve(500,false);
	dfs(res,ve,tree->root(),0);
	return res;
}

int encode(PFCTable* table,vector<bool>& bitString,char* str){
	bitString.clear();
	int n=0;
	for(size_t i=0,m=strlen(str);i<m;++i){
		char* code=(*table)[str[i]];
		if(!code)code=(*table)['*'];
		printf("%s",code);
		size_t k=strlen(code);
		n+=k;
		for(size_t j=0;j<k;++j)
			bitString.push_back(code[j]=='1');
	}puts("");
	return n;
}
void decode(PFCTree* tree,vector<bool>& bitString,int n){
	BinNodePosi(char) x=tree->root();
	for(int i=0;i<n;++i){
		x=bitString[i]? x->rc : x->lc;
		if(IsLeaf(*x)){
			printf("%c",x->data);
			x=tree->root();
		}
	}
	puts("");
	return;
}
int main(int argc,char* argv[]){
	/**
	  PFC 编码树类似字典树的结构
	  PFC森林是初始化PFCTree的叶节点，然后随机生成一个PFCTree
	  PFCTable 负责将生成的编码树每一个字符对应的编码保存起来（编码的时候用）
	  */
	PFCForest* forest=initForest();//初始化PFC森林
	PFCTree* tree=generateTree(forest);//生成PFC编码树
	delete forest;
	PFCTable* table=generateTable(tree);//将PFC编码树转换为编码表
	/*for(auto it=table->begin();it!=table->end();++it){
		cerr<<it->first<<' ';
		char* k=it->second;
		for(size_t i=0,m=strlen(k);i<m;++i)printf("%c",k[i]);
		cerr<<endl;
	}*/
	for(int i=1;i<argc;++i){//对于命令行传入的每一个明文串
		vector<bool> codeString;//二进制编码串
		int n=encode(table,codeString,argv[i]);//将根据编码表生成
		decode(tree,codeString,n);//利用编码树，对长度为n的二进制串解码（直接输出）
	}
	delete tree;
	delete table;
	return 0;
}


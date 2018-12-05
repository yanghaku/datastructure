#ifndef BIN_NODE_H
#define BIN_NODE_H
#include<stack>
#include<queue>

/* ====  *BinNode 的状态与性质判断 */
#define IsRoot(x) (!((x).parent ))
#define IsLChild(x) (! (IsRoot(x) && ( &(x) ==(x).parent->lc ) ) )
#define IsRChild(x) (! (IsRoot(x) && ( &(x) ==(x).parent->rc ) ) )
#define HasParent(x) (! IsRoot(x) )
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) ( HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x) )
#define IsLeaf(x) (! HasChild(x) )
/*  --- 与BinNode 具有特定关系的节点与指针 */
#define sibling(p) /*兄弟*/ \
    ( IsLChild( *(p) )? (p)->parent->rc : (p)->parent->lc )
#define uncle(x) /*叔叔 */  \
    ( IsLChild( *( (x)->parent ) )? (x)->parent->parent->rc : (x)->parent->parent->lc )
#define FromParentTo(x) /* 来自父亲的引用 */ \
    ( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )
typedef enum{RB_RED,RB_BLACK} RBColor;//节点颜色
#define stature(p) ((p)? (p)->height : -1) //节点高度(空树为-1)

#define BinNodePosi(T) BinNode<T>* 

template<typename T>struct BinNode//二叉树节点模版类
{
    T data;//数值
    BinNode<T>* parent;//父节点
    BinNode<T> *lc,*rc;//左右孩子;
    int height;//高度
    int npl;//nullptr path lehght(左式堆)
    RBColor color;//颜色(红黑树)
//构造函数
    BinNode():parent(nullptr),lc(nullptr),rc(nullptr),height(0),npl(1),color(RB_RED){}
    BinNode(T e,BinNode<T>* p=nullptr,BinNode<T>* lc=nullptr,BinNode<T>* rc=nullptr,
            int h=0,int l=1,RBColor c=RB_RED):
        data(e),parent(p),lc(lc),rc(rc),height(h),npl(l),color(c){}
//接口
    int size();//统计当前节点的后代总数(以其为根的子树的规模)
    BinNode<T>* insertAsLC(T const& e);//作为当前节点的左孩子插入
    BinNode<T>* insertAsRC(T const& e);//              右
    BinNode<T>* succ();//取当前节点的直接后继
    template<typename VST> void travLevel(VST& );//子树层次遍历
    template<typename VST> void travPre(VST& vst){TravPre_I1(this,vst);};//子树先序遍历
    template<typename VST> void travIn(VST& );//子树中序遍历
    template<typename VST> void travPost(VST& vst){travPost_R(this,vst);}//子树后序遍历
//比较器,判等器
    bool operator<(BinNode const& bn){return data<bn.data;}//小于
    bool operator<=(BinNode const& bn){return data<=bn.data;}//小于等于
    bool operator>(BinNode const& bn){return data>bn.data;}//大于
    bool operator>=(BinNode const& bn){return data>=bn.data;}//小于等于
    bool operator==(BinNode const& bn){return data==bn.data;}//等于
};

template<typename T>BinNodePosi(T) BinNode<T>::insertAsLC(T const& e){
	return lc=new BinNode(e,this) ;
}
template<typename T>BinNodePosi(T) BinNode<T>::insertAsRC(T const& e){
	return rc=new BinNode(e,this) ;
}

template<typename T>BinNodePosi(T) BinNode<T>::succ(){//定位节点的直接后继（中序遍历时访问此节点后的下一个节点）
	BinNodePosi(T) ans=this;
	if(ans->rc){  //如果有右子树，那就是右子树一直找左节点
		while(ans->lc)ans=ans->lc;
	}
	else{
		while(IsRChild(*ans))ans=ans->parent;//只有左孩子的后继是父节点，如果是右孩子，就向上走
		ans=ans->parent;//此时一定是左孩子，父节点就是后继，当最后一个节点的时候，就是nullptrptr
	}
	return ans;
}

template<typename T>template<typename VST>
void BinNode<T>::travIn(VST& visit){
	int kase=1;
	switch(kase){
		case 1: travIn_I1(this,visit);break;
		case 2: travIn_I2(this,visit);break;
		//case 3: travIn_I3(this,visit);break;
		//case 4: travIn_I4(this,visit);break;
		default: travIn_R(this,visit);break;
	}
}
template<typename T>template<typename VST>void BinNode<T>::travLevel(VST& vst){
    std::queue<BinNodePosi(T)>q;
    q.push(this);
    BinNodePosi(T) tmp;
    while(!q.empty()){
        tmp=q.front();q.pop();
        vst(tmp->data);
        if(tmp->lc)q.push(tmp->lc);
        if(tmp->rc)q.push(tmp->rc);
    }
}

template<typename T,typename VST>void travPre_R(BinNodePosi(T) x,VST& vst){//递归先序遍历
	if(!x)return;
	vst(x->data);
	travPre(x->lc,vst);
	travPre(x->rc,vst);
}
template<typename T,typename VST>void TravPre_I1(BinNodePosi(T) root,VST& vst){//迭代先序遍历1，尾递归消除
	std::stack<BinNodePosi(T)> st;
	if(root)st.push(root);
	while(!st.empty()){
		root=st.top();st.pop();
		vst(root);
		if(root->rc)st.push(root->rc);//先将右子树压栈，再将左子树压栈，才能先访问左子树
		if(root->lc)st.push(root->lc);
	}
}
template<typename T,typename VST>void TravPre_I2(BinNodePosi(T) root,VST& vst){//迭代先序遍历2,将右子树压栈
	std::stack<BinNodePosi(T)> st;
	if(root)st.push(root);
	while(!st.empty()){
		root=st.top();st.pop();
		while(root){
			visit(root);
			if(HasRChild(root))st.push(root);
			root=root->lc;
		}
	}
}


template<typename T,typename VST>void travIn_R(BinNodePosi(T) x,VST& vst){//递归版中序遍历
	if(!x)return;
	travIn_R(x->lc,vst);
	vst(x->data);
	travIn_R(x->rc,vst);
}
template<typename T,typename VST>void travIn_I1(BinNodePosi(T) x,VST& vst){//迭代版中序遍历1
	std::stack<BinNodePosi(T)> st;
	while(x){			//先将左节点依次入栈
		st.push(x);
		x=x->lc;
	}
	while(!st.empty()){
		x=st.top();st.pop();  //访问此节点，如果有右节点，右节点的左节点依次入栈
		vst(x->data);
		if(x->rc){
			x=x->rc;
			while(x){
				st.push(x);
				x=x->lc;
			}
		}
	}
}
template<typename T,typename VST>void travIn_I2(BinNodePosi(T) x,VST& vst){
	std::stack<BinNodePosi(T)> st;
	while(true){
		if(x){
			st.push(x);
			x=x->lc;
		}
		else if(!st.empty()){
			x=st.pop();
			vst(x->data);
			x=x->rc;
		}
		else break;
	}
}
template<typename T,typename VST>void travIn_I3(BinNodePosi(T) x,VST& vst){//迭代版中序遍历3，利用直接后继
	bool backtrack=false;
	while(true){
		if(!backtrack&&HasLChild(*x)){
			x=x->lc;//如果有左子树并且刚才没有回溯，就去左子树
		}
		else{
			visit(x->data);
			if(HasRChild(*x)){
				x=x->rc;
				backtrack=false;
			}else{
				if(!(x=x->succ()))break;
				backtrack=true;
			}
		}
	}
}


template<typename T,typename VST>void travPost_R(BinNodePosi(T) x,VST& vst){//递归版后序遍历
	if(!x)return;
	travPost_R(x->lc,vst);
	travPost_R(x->rc,vst);
	vst(x->data);
}


#endif

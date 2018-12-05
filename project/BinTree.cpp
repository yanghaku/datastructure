#ifndef BIN_TREE_H
#define BIN_TREE_H
#include "BinNode.cpp"

/*  ----===== ---- BinTree 模版类------======*/

template<typename T>class BinTree{//二叉树模版类,封装BinNode
protected:
    int _size;BinNode<T>* _root;//规模, 根节点
    virtual int updateHeight(BinNode<T>* x);//更新节点x的高度
    void updateHeightAbove(BinNode<T>* x);//更新节点及其祖先的高度
public:
    BinTree():_size(0),_root(NULL){}//构造函数
    ~BinTree(){if(_size>0) remove(_root);}//析构函数
    int size()const{return _size;}//规模
    bool empty()const{return !_root;}//判空
    BinNode<T>* root()const{return _root;}//树根
    BinNode<T>* insertAsRoot(T const& e);//插入根节点
    BinNode<T>* insertAsLC(BinNode<T>* x,T const& e);//e作为左孩子插入x下
    BinNode<T>* insertAsRC(BinNode<T>* x,T const& e);//e作为右孩子插入
    BinNode<T>* attachAsLC(BinNode<T>* x,BinTree<T>* &t);//T作为左子树插入x
    BinNode<T>* attachAsRC(BinNode<T>* x,BinTree<T>* &t);//T作为右子树插入
    int remove(BinNode<T>* x);//删除x为根的子树,返回子树原先的规模
    BinTree<T>* secede (BinNode<T>* x);//将x从树上摘除并转化为一棵独立的子树

    template<typename VST> void travLevel(VST& visit){if(_root)_root->travLevel(visit);}//层次遍历
    template<typename VST> void travPre(VST& visit){if(_root)_root->travPre(visit);}//先序遍历
    template<typename VST> void travIn(VST& visit){if(_root)_root->travIn(visit);}//中序遍历
    template<typename VST> void travPost(VST& visit){if(_root)_root->travPost(visit);}//后序遍历

    bool operator <(BinTree<T> const& t)//比较器
    {return _root && t._root && lt(_root,t._root);}
    bool operator ==(BinTree<T> const& t)
    {return _root && t._root && (_root,t._root);}
};//BinTree

template<typename T>int BinTree<T>::updateHeight(BinNode<T>* x)//更新x节点的高度
{return x->height=1+ (stature(x->lc)>stature(x->rc)? stature(x->lc):stature(x->rc));}//规则因树而异

template<typename T>void BinTree<T>::updateHeightAbove(BinNode<T>* x)//更新高度
{while(x){updateHeight(x);x=x->parent;}}//覆盖历代祖先, 每当子节点变化的时候，就一直更新到祖先

template<typename T>BinNode<T>* BinTree<T>::insertAsRoot(T const& e)
{//当做根节点插入
	if(_size>0){ remove(_root); }
    _size=1;
	return _root=new BinNode<T>(e);
}

template<typename T>BinNode<T>* BinTree<T>::insertAsLC(BinNode<T>* x,T const &e)
{//  规模+1   x上添加左孩子     更新祖先的高度    返回新生成的左孩子
    ++_size;x->insertAsLC(e);updateHeightAbove(x);return x->lc;
}

template<typename T>BinNode<T>* BinTree<T>::insertAsRC(BinNode<T>* x,T const& e)
{// 规模+1    x上添加右孩子     更新祖先高度    返回生成的右孩子
    ++_size;x->insertAsRC(e);updateHeightAbove(x);return x->rc;
}

template<typename T>//子树接入,将S作为x左节点接入,S置空
BinNode<T>* BinTree<T>::attachAsLC(BinNode<T>* x,BinTree<T>* &S)
{
    if(x->lc = S->_root)x->lc->parent=x;//接入,x左孩子指向s根,s根的父亲指向x
    _size+=S->_size;updateHeightAbove(x);//更新全树的规模和祖先的高度
    S->_root=NULL;S->_size=0;
	delete S;
    S=NULL;return x;//释放原树,返回x
}
template<typename T>BinNode<T>* BinTree<T>::attachAsRC(BinNode<T>* x,BinTree<T>* &S)
{
    if(x->rc = S->_root)x->rc->parent=x;
    _size+=S->_size;updateHeightAbove(x);
    S->_root=NULL;S->_size=0;
	delete S;
    S=NULL;return x;
}

template<typename T>// 删除二叉树中位置x处的节点及其后代
int BinTree<T>::remove(BinNode<T>* x)
{
    FromParentTo( *x ) = NULL;//切断来自父节点的指针
    updateHeightAbove(x->parent);//跟新祖先高度
    int n=removeAt(x);_size-=n;return n;//删除子树x,更新规模,返回删除节点总数
}
template<typename T>static int removeAt(BinNode<T>* x)
{
    if(!x)return 0;//递归基,空树
    int n=1+removeAt(x->lc)+removeAt(x->rc);//递归释放左右子树
    delete x;
	return n;//释放被摘除的节点,返回删除数;
}

template<typename T>//子树分离,并封装为一棵新树
BinTree<T>* BinTree<T>::secede(BinNode<T>* x)
{
    FromParentTo((*x))=NULL;//切断来自父节点的指针
    updateHeightAbove(x->parent);
    BinNode<T>* S=new BinTree<T>;
    S->_root=x;x->parent=NULL;//新树以x为根
    S->_size=x->size();_size-=S->_size;return S;
}

#endif

#include"BinTree.cpp"
/**====================binary search tree ======================*/

template<typename K,typename V>struct Entry{// 词条模板类
	K key; V value;//关键码，键值
	Entry(K k=K(),V v=V()):key(k),value(v){};//构造函数
	Entry(Entry<K,V>const& e):key(e.key),value(e.value){};//复制构造函数
	//比较器
	bool operator< (Entry<K,V>const& e){return key <e.key; }
	bool operator> (Entry<K,V>const& e){return key >e.key; }
	bool operator>=(Entry<K,V>const& e){return key>=e.key; }
	bool operator<=(Entry<K,V>const& e){return key<=e.key; }
	bool operator==(Entry<K,V>const& e){return key==e.key; }
	bool operator!=(Entry<K,V>const& e){return key!=e.key; }
};


template<typename T>class BST : public BinTree<T> {
protected:
	BinNodePosi(T) _hot;  //命中节点的父亲
	BinNodePosi(T) connect34(// 3+4结构， 联接3个节点及四课子树
		BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),
		BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),BinNodePosi(T) );
	BinNodePosi(T) rotateAt(BinNodePosi(T) x); // 对x及其父亲，祖父作统一的旋转
public:
	virtual BinNodePosi(T) & search(const T& e); // 查找
	virtual BinNodePosi(T) insert (const T& e); // 插入
	virtual bool remove ( const T& e);         // 删除
};

template<typename T>
static BinNodePosi(T) & searchIn(BinNodePosi(T)& v,const T& e,BinNodePosi(T)& hot){
	if(!v || (e==v->data) )return v;// 递归基
	hot=v;
	return searchIn( ((e<v->data)? v->lc: v->rc), e, hot);
}

template<typename T>BinNodePosi(T) & BST<T>::search(const T& e){
	return searchIn( _root, e, _hot=nullptr);
}







#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef int Rank;
#define ListNodePosi(T) ListNode<T>* //列表节点的位置

template <typename T> struct ListNode{ //列表节点模版类(双向链表实现)
//成员
    T data;//数值
    ListNodePosi(T) pred=nullptr;//前驱
    ListNodePosi(T) succ=nullptr;//后继
//构造函数
    ListNode(){};//针对header 和 trailer
    ListNode (T e,ListNodePosi(T) p=nullptr,ListNodePosi(T) s=nullptr)
        : data(e),pred(p),succ(s){}//默认构造器
//接口
    ListNodePosi(T) insertAsPred(T const& e);//紧靠节点之前插入,返回新节点
    ListNodePosi(T) insertAsSucc(T const& e);//紧靠当前节点之后插入,
};//ListNode
template<typename T>ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
    ListNodePosi(T) x=new ListNode(e,pred, this );//创建新节点
    pred->succ=x;
    pred=x;
    return x;
}
template<typename T>ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
    ListNodePosi(T) x=new ListNode(e,this,succ);
    succ->pred=x;
    succ=x;
    return x;
}





template<typename T>class list //列表
{
private:
    int _size=0;//规模
    ListNodePosi(T) header=nullptr;//头哨兵
    ListNodePosi(T) trailer=nullptr;//尾哨兵
protected:
    void init();//列表创建时初始化
    int clear();//清除所有节点
    void copyNodes(ListNodePosi(T),int ); //复制列表中自位置p起的n项
    void merge(ListNodePosi(T)&,int ,list<T>&,ListNodePosi(T),int );//归并
    void mergeSort(ListNodePosi(T)&,int);//对从p节点开始连续的n个节点归并排序
    void selectSort(ListNodePosi(T),int);//对从p节点开始连续的n个节点选择排序
    void insertionSort(ListNodePosi(T),int);//对从p节点开始连续的n个节点插入排序
public:
//构造函数
    list(){init();}//默认
    list(list<T>const&L);//整体复制列表L
    list(list<T>const&L,Rank r,int n);//复制列表从第r项起的n项
    list(ListNodePosi(T) p,int n);//复制列表中自位置p起的n项
//析构函数
    ~list();//释放所有节点,包括头,尾哨兵
//只读访问接口
    Rank size()const{return _size;}//规模
    bool empty()const{return _size<=0;}//判空
    T &operator[](Rank r)const;//重载,支持循秩访问,O(n)
    ListNodePosi(T) first()const{return header->succ;}//首节点
    ListNodePosi(T) last()const{return trailer->pred;}//末节点位置

    bool valid(ListNodePosi(T) p)//判断位置p是否对外合法
    { return p&&(trailer!=p)&&(header!=p);}//头尾哨兵等同于null

    int disordered() const;//判断列表是否已排好序
    ListNodePosi(T) find(T const& e,int n,ListNodePosi(T) p)const;//无序区间查找
    ListNodePosi(T) find(T const& e)const   {return find(e,_size,trailer);}//无序查找
    ListNodePosi(T) search(T const& e,int n,ListNodePosi(T) p)const;//有序区间查找
    ListNodePosi(T) search(T const& e)const   {return search(e,_size,trailer);}//有序查找
    ListNodePosi(T) selectMax(ListNodePosi(T) p,int n)const;//在p及其后n-1个找出最大的
    ListNodePosi(T) selectMax()const   {return selectMax(header->succ,_size);}//整体最大的
//可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e);//将e作为首节点插入
    ListNodePosi(T) insertAsLast(T const& e);//将e作为末节点插入
    ListNodePosi(T) insertA (ListNodePosi(T) p,T const& e);//将e当做p的后继插入
    ListNodePosi(T) insertB (ListNodePosi(T) p,T const& e);//将e当做p的前驱插入
    T remove( ListNodePosi(T) p);//删除合法位置p处的节点,返回被删除的节点
    void merge(list<T>& L){merge(first(),size,L,L.first(),L._size);}//全列表归并
    void sort(ListNodePosi(T) p,int n);//列表区间排序
    void sort(){sort(first(),_size);}//列表整体排序
    int deduplicate();//无序去重
    int uniquify();//有序去重
    void reverse();//前后倒置
//遍历
    void traverse(void (*)(T& ));//遍历,依次实施visit操作(函数指针)
    template<typename VST>
    void traverse( VST& );//遍历,依次执行visit操作(函数对象)
};//list





//初始化函数
template<typename T>void list<T>::init()
{
    header=new ListNode<T>;//创建头哨兵
    trailer=new ListNode<T>;//创建尾哨兵节点
    header->succ=trailer;header->pred=nullptr;
    trailer->pred=header;trailer->succ=nullptr;
    _size=0;//记录规模
}

//重载[]
template<typename T>T& list<T>::operator[](Rank r)const
{
    ListNodePosi(T) p=first();//从首节点出发
    while(0<r--)p=p->succ;
    return p->data;
}

//无序查找 O(n)
template<typename T>ListNodePosi(T) list<T>::find(T const&e,int n,ListNodePosi(T) p)const
{
    while(0<n--)
    {
        p=p->pred;
        if(e==p->data)return p;//逐个比对
    }
    return nullptr;//失败时返回null
}

//插入
template<typename T>ListNodePosi(T) list<T>::insertAsFirst(T const& e)
{
    ++_size;
    return header->insertAsSucc(e);//当做首节点
}
template<typename T>ListNodePosi(T) list<T>::insertAsLast(T const& e)
{
    ++_size;
    return trailer->insertAsPred(e);
}
template<typename T>ListNodePosi(T) list<T>::insertA(ListNodePosi(T) p,T const& e)
{
    ++_size;
    return p->insertAsSucc(e);
}
template<typename T>ListNodePosi(T) list<T>::insertB(ListNodePosi(T) p,T const& e)
{
    ++_size;
    return p->insertAsPred(e);
}



//基于复制的构造
//内部方法 复制
template<typename T>void list<T>::copyNodes(ListNodePosi(T) p,int n)
{//p合法且至少n-1个真后继节点
    init();//创建头/尾哨兵节点并初始化
    while(n--){insertAsLast(p->data);p=p->succ;}
}

template<typename T>list<T>::list(ListNodePosi(T) p,int n){copyNodes(p,n);}
template<typename T>list<T>::list(list<T> const& L){copyNodes(L.first(),L._size);}
template<typename T>list<T>::list(list<T> const&L,int r,int n){copyNodes(L[r],n);}


//删除
template<typename T>T list<T>::remove(ListNodePosi(T) p)
{
    T e=p->data;//备份
    p->pred->succ=p->succ;
    p->succ->pred=p->pred;
    delete p;//释放节点
    --_size;//更新规模
    return e;
}

//清除
template<typename T>int list<T>::clear()
{
    int oldsize=_size;
  /**
    while(0<_size)remove(header->succ);
        一直remove太慢,操作太多                   */
    ListNodePosi(T) x=header->succ;
    while(_size--)
    {
        header->succ=x->succ;
        delete x;
        x=header->succ;
    }
    return oldsize;
}

template<typename T>list<T>::~list()
{
    clear();//清空列表
    delete header;//删除哨兵
    delete trailer;
}


//唯一化  无序列表去重
template<typename T>int list<T>::deduplicate()
{
    if(_size<2)return 0;
    int oldSize=_size;//记录原规模
    ListNodePosi(T) p=header;
    Rank r=0;
    while(trailer!=( p=p->succ))
    {
        ListNodePosi(T) q=find(p->data,r,p);//从头查到p
        q!=nullptr?remove(q):r++;//如果存在,删除!!前面出现的
    }
    return oldSize-_size;//返回规模变化量
}

//遍历
template<typename T>void list<T>::traverse(void (*visit)(T&))
{
    for(ListNodePosi(T) p=header->succ;p!=trailer;p=p->succ)visit(p->data);
}
template<typename T> template<typename VST>
void list<T>::traverse(VST& visit)
{
    for(ListNodePosi(T) p=header->succ;p!=trailer;p=p->succ)visit(p->data);
}


//有序列表的唯一化
template<typename T>int list<T>::uniquify()
{
    if(_size<2)return 0;
    int oldSize=_size;
    ListNodePosi(T) p=first(),q;
    while(trailer!=(q=p->succ))
    {
        if(p->data !=q->data)p=q;
        else remove(q);//相同就删除
    }
    return _size-oldSize;
}

//有序查找   查找p前面的n个节点
template<typename T>ListNodePosi(T) list<T>::search(T const&e,int n,ListNodePosi(T) p)const
{
    while(0<n--)
    { //从右向左比较,直至命中
        if(((p=p->pred)->data)<=e)break;
    }
    return p;//返回查找终止的位置
}//失败时,返回的是左边界的前驱


//插入排序
template<typename T>void list<T>::insertionSort(ListNodePosi(T) p,int n)
{
    for(int i=0;i<n;++i)
    {
        insertB(search(p->data,i,p),p->data);
        p=p->succ;
        remove(p->pred);
    }//有插入就有删除,若T是一个类,效率不高
}

//选择排序
template<typename T>ListNodePosi(T) list<T>::selectMax(ListNodePosi(T) p,int n)const
{
    ListNodePosi(T) x=p;
    while(n--)
    {
        p=p->succ;
        if((p->data)>(x->data))x=p;
    }
    return x;
}
template<typename T>void list<T>::selectSort(ListNodePosi(T) p,int n)
{
    ListNodePosi(T) head=p->pred;
    while(n--)
    {
        ListNodePosi(T) x=selectMax(p,n);
        insertA(head,x->data);
        remove(x);
        p=p->succ;
    }
}
#include <iostream>
//merge  当前的p后的n个与 L中q后的m个归并
template<typename T>void list<T>::merge(ListNodePosi(T)& p,int n,list<T>& L,ListNodePosi(T) q,int m)
{
    //假定这两个不重叠,归并排序中,*this就是L
    ListNodePosi(T) pp=p->pred;
    while(m>0)
    {
        if((n<=0)||((p->data)>(q->data)))
        {
            insertB(p,q->data);
            q=q->succ;
            L.remove(q->pred);
            --m;
            std::cerr<<"aaa  "<<p->data<<std::endl;
        }
        else
        {
            std::cerr<<"nnnn  "<<p->data<<std::endl;
            p=p->succ;
            --n;
        }
    }
    p=pp->succ;
}
/*!!!!!! ----归并和归并排序都是引用传递,使得p一直都是指向起点*/
template<typename T>void list<T>::mergeSort(ListNodePosi(T) &p,int n)
{
    if(n<2)return;
    int m=n>>1;
    ListNodePosi(T) q=p;
    for(int i=0;i<m;++i)q=q->succ;//均分列表 !!!!
    mergeSort(p,m);mergeSort(q,n-m);
    merge(p,m,*this,q,n-m);
}//p会指向起点
//排序接口
template<typename T>void list<T>::sort(ListNodePosi(T) p,int n)
{
    mergeSort(p,n);
}

#endif // LIST_H_INCLUDED




#include <iostream>
using std::cout;
using std::endl;
struct VST
{
    void operator()(const int &e)
    {cout<<e<<endl;}
};
int main()
{
    VST print;
    cout<<"hello world"<<endl;
    list<int> a;
    for(int i=0;i<30;++i)a.insertAsFirst(i);
    a.traverse(print);
    a.sort();
    a.traverse(print);
    return 0;
}

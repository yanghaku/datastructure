#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <algorithm>

typedef int Rank;//秩
#define DEFAULT_CAPACITY 3
template <typename T>class vector//向量模版类
{
protected:
    Rank _size=0; int _capacity=0; T* _elem=nullptr;//规模,容量,数据区

    void copyFrom(T const *A,Rank lo,Rank hi);//复制数组区间A[lo,hi)
    void expand();//空间不足时扩容
    void shrink();//装填因子过小时压缩
    bool bubble(Rank lo,Rank hi);//扫描交换
    void bubbleSort(Rank lo,Rank hi);//起泡排序算法
    Rank max(Rank lo,Rank hi);//选取最大元素
    void selectionSort(Rank lo,Rank hi);//选择排序算法
    void merge(Rank lo,Rank mi,Rank hi);//归并算法
    void mergeSort(Rank lo,Rank hi);//归并排序算法
    Rank partition(Rank lo,Rank high);//轴点构造算法
    void quickSort(Rank lo,Rank hi);//快速排序算法
    void heapSort(Rank lo,Rank hi);//堆排序
public:
//构造函数:
    vector(int c=DEFAULT_CAPACITY,int s=0,T v=0)//容量为c,规模为s,所有元素初始为v
    {_elem=new T[_capacity=c];for(_size=0;_size<s;_elem[_size++]=v);} //s<=c
    vector(T const *A,Rank n){copyFrom(A,0,n);}//数组整体复制
    vector(T const *A,Rank lo,Rank hi){copyFrom(A,lo,hi);}//区间复制
    vector(vector<T> const &V){copyFrom(V._elem,0,V._size);}//向量整体复制
    vector(vector<T> const &V,Rank lo,Rank hi){copyFrom(V._elem,lo,hi);}//向量区间复制
//析构函数:
    ~vector(){delete [] _elem;}//释放内存空间
//只读访问接口:
    Rank size() const {return _size;}//规模,内联函数
    bool empty() const {return !_size;}//判空
    int disordered() const;//判断向量是否已排好序
    Rank find(T const &e)const{return find(e,0,_size);}//无序向量整体查找
    Rank find(T const &e,Rank lo,Rank hi)const;//无序向量区间查找
    Rank search(T const &e)const//有序向量整体查找
    {return (0>=_size)?-1:search(e,0,_size);}
    Rank search(T const &e,Rank lo,Rank hi)const;//有序向量区间查找

//可写访问接口:
    T& operator[] (Rank r)const;//重载下标操作符[],可以类似数组形式
    vector<T> &operator = (vector<T> const&);//重载赋值运算符.便于直接克隆
    void push_back(T const &e){insert(e);}
    T remove(Rank r);//删除秩为r的元素
    int remove(Rank lo,Rank hi);//删除秩在区间[lo,hi)的元素
    Rank insert(Rank r,T const &e);//插入元素
    Rank insert(T const &e){return insert(_size,e);}//默认插入末尾
    void sort(Rank lo,Rank hi);//对[lo,hi)排序
    void sort(){sort(0,_size);}//整体排序
    void unsort(Rank lo,Rank hi);//[lo,hi)置乱
    void unsort(){unsort(0,_size);}//整体置乱
    int deduplicate();//无序去重
    int uniquify();//有序去重
    void uniquify_slow();

//遍历:
    void traverse(void (*)(T& ));//遍历,使用函数指针,,只读或局部修改
    template<typename VST> void traverse(VST&);//遍历,使用函数对象,可全局修改
};//vector







template<typename T>void vector<T>::copyFrom(T const *A,Rank lo,Rank hi)
//以数组 A[lo,hi) 为蓝本复制
{
    _elem=new T[_capacity=2*(hi-lo)];//分配内存,2倍
    _size=0;
    while(lo<hi)//逐一复制
    {
        _elem[_size++]=A[lo++];
    }
    return;
}

//重载赋值运算符
template<typename T>vector<T>& vector<T>::operator=(vector<T>const &V)
{
    if(_elem!=nullptr)delete[]_elem;//若不为空,要先释放内存
    copyFrom(V._elem,0,V._size);
    return *this;//返回自己,以便链式赋值
}

//动态管理内存::扩容
template<typename T>void vector<T>::expand()
{
    if(_size<_capacity)return;//当没有满员的时候返回,不扩容
    if(_capacity<DEFAULT_CAPACITY)_capacity=DEFAULT_CAPACITY;//容量不可以小于最小容量
    T* oldelem=_elem;//保存老地址
    _elem=new T[_capacity<<=1];//分配新容量(加倍)
    for(int i=0;i<_size;++i)_elem[i]=oldelem[i];//复制
    delete[] oldelem;//释放内存
    return;
}

//缩容:
template<typename T>void vector<T>::shrink()
{
    if(_capacity<(DEFAULT_CAPACITY<<1))return;//不能缩到默认值以下
    if((_size<<2)>_capacity)return;//以25%为界,大于25%不缩容
    T *oldelem=_elem;
    _elem=new T[_capacity>>=1];//容量减半
    for(int i=0;i<_size;++i)_elem[i]=oldelem[i];
    delete[] oldelem;
    return;
}
//置乱算法
template<typename T>void vector<T>::unsort(Rank lo,Rank hi)
{
    T *V=_elem+lo;//子向量lo为起点
    for(Rank i=hi=lo;i>0;--i)
    {//V[i-1] 与 [0,i)交换
        std::swap(V[i-1],V[std::rand()%i]);//直接通过下标
    }
}

//重载[]
template<typename T> T& vector<T>::operator[](Rank r)const
{
    return _elem[r];
}

//无序查找的顺序查找: 返回最后一个元素e的位置,没有找到返回 lo-1
template<typename T> Rank vector<T>::find(T const &e,Rank lo,Rank hi)const
{//assert(0<=lo<hi<=_size
    while((lo<hi--)&&(e!=_elem[hi]));//从后向前
    return hi;
}

//插入
template<typename T> Rank vector<T>::insert(Rank r,T const &e)
{//assert(0<=r<=_size)
    expand();//先检查是否需要扩容
    for(int i=_size;i>r;--i)_elem[i]=_elem[i-1];//每个向后移一位
    _elem[r]=e;
    ++_size;//!!!!!更新容量
    return r;
}

//区间删除
template<typename T>int vector<T>::remove(Rank lo,Rank hi)
{
    if(lo>=hi)return 0;//考虑退化的情况
    while(hi<_size)_elem[lo++]=_elem[hi++];//后面覆盖前面 复杂度O(_size-hi)
    _size=lo;//!!!更新容量
    shrink();// 若有必要缩容
    return hi-_size;//返回删除个数
}

//单元素删除是区间删除的特例
template<typename T>T vector<T>::remove(Rank r)
{
    T e=_elem[r];//备份
    remove(r,r+1);
    return e;//返回被删除元素
}

//无序唯一化
template<typename T>int vector<T>::deduplicate()
{
    int oldsize=_size;//记录原规模
    Rank i=1;
    while(i<_size)//从前向后逐一
    {
        if(find(_elem[i],0,i)<0)++i;
        else remove(i);
    }
    return oldsize-_size;//返回删除的个数
}

//借助函数指针遍历,
template<typename T>void vector<T>::traverse(void (*visit)(T& ))
{
    for(int i=0;i<_size;++i)visit(_elem[i]);//遍历
}

//借助函数对象遍历
template<typename T>template<typename VST>//元素类型,,操作器
void vector<T>::traverse(VST& visit)
{
    for(int i=0;i<_size;++i)visit(_elem[i]);
}

//有序性甄别
template<typename T>int vector<T>::disordered()const
{
    int n=0;//计数器
    for(int i=1;i<_size;++i)//遍历,,鉴别
    {
        if(_elem[i-1]>_elem[i])++n;//逆序就计数
    }
    return n;//当且仅当n==0时有序
}

//唯一化低效版 复杂度O(n^2)
template<typename T>void vector<T>::uniquify_slow()
{
    for(int i=1;i<_size;)
    {
        (_elem[i]==_elem[i-1])?remove(i):++i;
    }
}

//唯一化高效版 复杂度O(n)
template<typename T>int vector<T>::uniquify()
{
    /*int nowsize=1,cha;
    for(int i=1;i<_size;++i)
    {
        if(_elem[i]!=_elem[i-1])_elem[nowsize++]=_elem[i];
    }
    cha=_size-nowsize;
    _size=nowsize;
    shrink();
    return cha;*/
    Rank i=0,j=0;
    while(++j<_size)
    {
        if(_elem[i]!=_elem[j])//跳过雷同者
        _elem[++i]=_elem[j];
    }
    _size=++i;
    shrink();
    return j-i;
}
/*--------------------------查找---------------------------*/
//fibonacci查找
class Fib
{
private:
    int _n=2;
    int _sizee=1;
    int fibb[100];
public:
    Fib(int n=5)
    {
        if(this->fibb[_sizee]<n)
        {
            for(++_sizee;this->fibb[_sizee]<=n;++_sizee)
            {
                this->fibb[_sizee]=this->fibb[_sizee-1]+this->fibb[_sizee-2];
            }
        }
        _n=_sizee;
    }
    void prev()
    {
        --_n;
    }
    int get()
    {
        return fibb[_n];
    }
};
template<typename T>static Rank fibsearch(T *A,T e,int lo,int hi)
{
    Fib fib(hi-lo);//用 O(hi-lo)的时间创建fib数列
    while(lo<hi)
    {
        while(hi-lo<fib.get())fib.prev();//通过向前顺序查找,至多迭代几次
        Rank mi=lo+fib.get()-1;//确定形如fib(k)-1的轴点
        if(e<A[mi])hi=mi;//在前半段寻找
        else if(A[mi]<e)lo=mi+1;
        else return mi;
    }//命中就返回
    return -1;
}//有多个时,不保证返回秩最大的,,失败时,返回-1;

//二分查找版本A
template<typename T>Rank binsearch(T *A,T e,int lo,int hi)
{
    int mid;
    while(lo<hi)
    {
        mid=(lo+hi)>>1;
        if(A[mid]>e)hi=mid;
        else if(A[mid]<e)lo=mid+1;
        else return mid;
    }
    return -1;
}
//二分查找版本B 高效版 二分支
template<typename T>Rank binsearch2(T *A,T e,int lo,int hi)
{
    while(1<hi-lo)//出口时只剩一个元素 [lo,hi)
    {
        Rank mid=(hi+lo)>>1;
        e<A[mid]?
          hi=mid : lo=mid;  //不能提前终止
    }
    return A[lo]==e?lo:-1;
}
//二分查找版本C 返回最大秩
template<typename T>Rank binsearch3(T *A,T e,int lo,int hi)
{
    while(lo<hi)
    {
        Rank mid=(lo+hi)>>1;
        e<A[mid]? hi=mid : lo=mid+1;
    }
    return --lo;
//结束时,lo为大于e的最小秩,lo-1是e的最大秩
}
//有序查找接口
template<typename T>Rank vector<T>::search(T const &e,Rank lo,Rank hi)const
{//返回不大于e的最后一个节点的秩
    return binsearch3(_elem,e,lo,hi);
}
/*-----------------------------------------------------------*/

/*------------------------sort-------------------------------*/
//起泡排序
template<typename T>void vector<T>::bubbleSort(Rank lo,Rank hi)
{ while( !bubble(lo,hi--)); }//逐趟扫描

template<typename T>bool vector<T>::bubble(Rank lo,Rank hi)
{
    bool sorted=true;//有序标志
    while(++lo<hi)//从左向右扫描
    {
        if(_elem[lo-1]>_elem[lo])
        {
            sorted=false;
            std::swap(_elem[lo-1],_elem[lo]);
        }
    }
    return sorted;
}

//选排
template<typename T>void vector<T>::selectionSort(Rank lo,Rank hi)
{
    for(int i=lo;i<hi;++i)
    {
        int ma=i;
        for(int j=i+1;j<hi;++j)
        {
            if(_elem[j]<_elem[ma])ma=j;
        }
        if(ma!=i)std::swap(_elem[ma],_elem[i]);
    }
    return;
}

//关键 归并的接口
template<typename T>void vector<T>::merge(Rank lo,Rank mid,Rank hi)
{//合并各自有序的向量[lo,mid) [mid,hi)
    T *A=_elem+lo;
    int lb=mid-lo;T *B=new T[lb];
    for(Rank i=0;i<lb;++i)B[i]=A[i];
    int lc=hi-mid;T *C=_elem+mid;
    for(Rank i=0,j=0,k=0;(j<lb)||(k<lc);)
    {
        if( (j<lb)&& (!(k<lc)||(B[j]<=C[k])) )A[i++]=B[j++];
        if( (k<lc)&& (!(j<lb)||(C[k]< B[j])) )A[i++]=C[k++];
    }
    delete []B;//释放空间
}//归并后得到完整的有序[lo,hi)

//归并排序
template<typename T>void vector<T>::mergeSort(Rank lo,Rank hi)
{
    if(2>hi-lo)return;//递归基,单区间的时候返回;
    int mid=(hi+lo)>>1;//以中点为界返回
    mergeSort(lo,mid);mergeSort(mid,hi); //继续分开排序
    merge(lo,mid,hi); //归并
}

//排序接口
template<typename T>void vector<T>::sort(Rank lo,Rank hi)
{
    mergeSort(lo,hi);
    /*switch(rand()%3)
    {
        case 1:bubbleSort(lo,hi);break;
        case 2:selectionSort(lo,hi);break;
        //case 3:quickSort(lo,hi);break;
       // case 4:heapSort(lo,hi);break;
        default: mergeSort(lo,hi);break;
    }*/
}
/*-----------------------------------------------------------*/

#endif // VECTOR_H_INCLUDED















#include <iostream>
#include "vector.h"


class print
{
public:
    void operator()(const int &t)
    {
        std::cout<<t<<std::endl;
    }
};
int main()
{
    print s;
    vector<int> a;
    for(int i=1000;i>0;--i)a.push_back(i);
    std::cout<<"sdfgbhjgrfedwergthfd"<<std::endl;
    for(int i=0;i<1000;++i)a.search(i);
    a.sort();
    a.traverse(s);
    return 0;
}

#ifndef P_QUEUE_H_INCLUDED
#define P_QUEUE_H_INCLUDED

#include <vector>

#define left(i) (i)<<1
#define right(i) (i)<<1|1
#define parent(i) (i)>>1

using std::swap;

template<typename T>class p_queue{
private:
    std::vector<T> a;           //用堆维护一个数组
    void build_heap();          //建堆
    void max_heap(int now);        //调整最大堆
    void max_heap_in_sort(std::vector<T>& s,const int& now,const int& n);
                                //堆排序专用的 最大堆维护
    void erase();               //删除堆顶元素
    void insert(const T& key);    //插入
    void judge(int now);          //插入后交换恢复最大堆
public:
    p_queue(){ a.clear();a.resize(1); }
    p_queue(const std::vector<T>& ve){ a=std::vector<T>(ve); build_heap();}
    inline void pop(){ erase(); }
    inline T top(){ return a[1]; }
    inline void push(const T& key){  insert(key); }
    inline int size(){return a.size()-1;}
    inline bool empty(){return a.size()<1;}
    inline std::vector<T> to_vector();
};

template<typename T>void p_queue<T>::build_heap(){
    for(int i=( a.size()-1)>>1;i>0;--i)max_heap(i);
}
template<typename T>void p_queue<T>::max_heap(int now)
{
    if(now>=a.size())return;
    int l=left(now),r=right(now);
    int largest=now;
    if(l<a.size()&&a[largest]<a[l])largest=l;
    if(r<a.size()&&a[largest]<a[r])largest=r;
    if(largest!=now)
    {
        std::swap(a[largest],a[now]);
        max_heap(largest);
    }
}
template<typename T>void p_queue<T>::judge(int now)
{
    if(now<=1)return;
    int p=parent(now);
    if(a[p]<a[now])
    {
        std::swap(a[p],a[now]);
        judge(p);
    }
}
template<typename T>void p_queue<T>::insert(const T& key)
{
    a.push_back(key);
    judge(a.size()-1);
}
template<typename T>void p_queue<T>::erase()
{
    a[1]=a[a.size()-1];a.erase(a.begin()+a.size()-1);
    max_heap(1);
}

template<typename T>void p_queue<T>::max_heap_in_sort(std::vector<T>& s,const int& now,const int& n)
{
    if(now>=n)return;
    int l=left(now),r=right(now);
    int largest=now;
    if(l<n&&s[largest]<s[l])largest=l;
    if(r<n&&s[largest]<s[r])largest=r;
    if(largest!=now)
    {
        std::swap(s[largest],s[now]);
        max_heap_in_sort(s,largest,n);
    }
}
template<typename T>std::vector<T> p_queue<T>::to_vector()
{
    std::vector<T> ans=std::vector<T>(a);
    int n=ans.size();
    while(n>2)
    {
        --n;
        swap(ans[1],ans[n]);
        max_heap_in_sort(ans,1,n);
    }
    return ans;
}

#endif // P_QUEUE_H_INCLUDED




#include <iostream>
#include <vector>

using namespace std;

p_queue<int> pq;
int main()
{
    cerr << "Hello world!" << endl;

    vector<int> ve;
    for(int i=0;i<20;++i)ve.push_back(rand()%30);
    for(int i=0;i<ve.size();++i){cerr<<ve[i]<<"  ";}cerr<<endl<<endl;

    pq=p_queue<int>(ve);

    auto m=pq.to_vector();

    for(auto it=m.begin()+1;it!=m.end();++it)cerr<<*it<<"  ";cerr<<endl;

    while(!pq.empty()){ cerr<<pq.top()<<endl; pq.pop();}

    return 0;
}

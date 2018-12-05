#include<iostream>
using std::cerr;
using std::endl;

template<typename T>class stack{
private:
    T *_elem;
    int _size;
    int _cap;
    void expand(){
        T* old_elem=_elem;
        _cap<<=1;
        _elem=new T[_cap];
        for(int i=0;i<_size;++i)_elem[i]=old_elem[i];
        delete[] old_elem;
    }
public:
    stack(int n=100){
        _cap=n;
        _elem=new T[_cap];
        _size=0;
    }
    ~stack(){
        delete[] _elem;
    }
    inline bool empty(){return _size==0;}
    inline int size(){return _size;}
    inline void pop(){
        if(_size==0)return;
        --_size;
        return;
    }
    inline T top(){
        return _elem[_size-1];
    }
    inline void push(T const& e){
        if(_size==_cap)expand();
        _elem[_size++]=e;
    }
};

int main(){
    stack<int> st(10);
    for(int i=0;i<20;++i)st.push(i);
    cerr<<st.size()<<endl;
    while(!st.empty()){
        cerr<<st.top()<<endl;
        st.pop();
    }
    return 0;
}

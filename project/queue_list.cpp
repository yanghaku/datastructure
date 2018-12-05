#include<iostream>
#include<cstdlib>
using std::cerr;
using std::endl;
#define posi(T) Node<T>*
template<typename T>struct Node{
    T data;
    Node<T>* next;
};
template<typename T>class queue{
private:
    posi(T) _head;
    posi(T) _end;
    int _size;
public:
    queue(){
        _size=0;
        _head=nullptr;
        _end=nullptr;
    }
    ~queue(){
        posi(T) tmp;
        while(_head){
            tmp=_head;
            _head=_head->next;
            free(tmp);
        }
    }
    inline bool empty(){return _size==0;}
    inline int size(){return _size;}
    inline T front(){return _head->data;}
    inline void pop(){
        if(_size==0)return;
        --_size;
        posi(T) tmp=_head;
        _head=_head->next;
        free(tmp);
    }
    inline void push(const T& e){
        if(_size==0){
            _end=(posi(T))malloc(sizeof(Node<T>));
            _head=_end;
        }else{
            _end->next=(posi(T))malloc(sizeof(Node<T>));
            _end=_end->next;
        }
        _end->data=e;
        ++_size;
    }
};

int main(){
    queue<int> q;
    for(int i=0;i<10;++i)q.push(i);
    while(!q.empty()){
        cerr<<q.front()<<endl;
        q.pop();
    }
    return 0;
}

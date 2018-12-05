#include<iostream>
#include<assert.h>
using std::cerr;
using std::endl;


template<typename T>class List{
private:
	T *_elem;   // 存储数据
	int *_next;
	int _size,_cap;
	int unused_head;// 没有分配的内存,   这两个默认尾部 -1 结尾
	int head; // 分配过的内存(也就是维护的链表)

	int malloc(){//分配函数
		if(unused_head==-1)expand();//如果数组的所有都分配完了,就扩容数组
		int tmp=unused_head;
		unused_head=_next[unused_head];
		return tmp;
	}
	void free(int node){
		_next[node]=unused_head;
		unused_head=node;
	}
	void expand(){ // 当内存不够分配的时候扩充
		T* old_elem=_elem;
		int* old_next=_next;
		unused_head=_cap;
		_cap<<=1;
		_elem=new T[_cap];
		_next=new int[_cap];
		for(int i=head;~i;i=old_next[i])_elem[i]=old_elem[i];
		for(int i=0;i<unused_head;++i)_next[i]=old_next[i];
		for(int i=unused_head;i<_cap;++i)_next[i]=i+1;
		_next[_cap-1]=-1;
		delete[] old_elem;
		delete[] old_next;
	}
public:
	List(int n=100){
		this->_cap=n;
		this->_elem=new T[_cap];
		this->_next=new int[_cap];
		this->_size=0;
		--n;
		for(int i=0;i<n;++i)_next[i]=i+1;
		_next[n]=-1;
		unused_head=0;
		head=-1;
	}
	~List(){
		delete[] _elem;
		delete[] _next;
	}
	inline int size(){return _size;}
	inline bool empty(){return _size==0;}
	void clear(){
        _size=0;
        unused_head=0;
        for(int i=0;i<_cap;++i)_next[i]=i+1;
        _next[_cap-1]=head=-1;
	}
	void push_back(const T& e){
	    ++_size;
		int index=head;
		if(head==-1){
            head=malloc();
            _elem[head]=e;
            _next[head]=-1;
            return;
		}
		while(~_next[index])index=_next[index];
		_next[index]=malloc();
		index=_next[index];
		_elem[index]=e;
		_next[index]=-1;
	}
	void push_front(const T& e){
		int h=malloc();
		_elem[h]=e;
		_next[h]=head;
		head=h;
		++_size;
	}
	void insert_as_pre(int num,const T& e){
		assert(-1<num&&num<_size);
		++_size;
		if(num==0){push_front(e);return;}
		int index=head;
		--num;
		while(num--)index=_next[index];
        int k=malloc();
        _elem[k]=e;
        _next[k]=_next[index];
        _next[index]=k;
	}
	void insert_as_next(int num,const T& e){
        assert(-1<num&&num<_size);
        int index=head;
        while(num--)index=_next[index];
        int k=malloc();
        _elem[k]=e;
        _next[k]=_next[index];
        _next[index]=k;
        ++_size;
	}
	void erase(int num){
        assert(-1<num&&num<_size);
        --_size;
        if(num==0){
            int tmp=head;
            head=_next[head];
            free(tmp);
            return;
        }
        int index=head;--num;
        while(num--)index=_next[index];
        int tmp=_next[index];
        _next[index]=_next[tmp];
        free(tmp);
	}

	T operator [](int num){
		assert(-1<num&&num<_size);
		int index=head;
		while(num--)index=_next[index];
		return _elem[index];
	}

	void debug(){
		cerr<<"debug"<<endl<<"unused:  ";
		for(int i=unused_head;~i;i=_next[i])cerr<<i<<"->";
		cerr<<-1<<endl<<"head:  ";
		for(int i=head;~i;i=_next[i])cerr<<i<<"->";
		cerr<<-1<<endl;
	}
};

int main(){
	List<int> l(20);
	for(int i=0;i<10;++i){
        l.push_back(i);
	}
	l.erase(4);
	l.push_back(5);
	l.erase(0);
	for(int i=0;i<l.size();++i)cerr<<l[i]<<' ';cerr<<endl;
	l.debug();
    return 0;
}

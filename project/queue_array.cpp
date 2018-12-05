
template<typename T>class queue{
private:
	T* _elem;
	int _cap;
	T*  _front;
	T*  _end;
	void expand(){
		T* old_elem=_elem;
		int _size=_cap;
		_elem=new T[_cap];
		_end=_elem;
		while(_size--){
			*_end=*_front;
			++_end;
			++_front;
			if(_front-old_elem>=_cap)_front=old_elem;
		}
		_cap<<=1;
		_front=_elem;
		delete[] old_elem;
	}

public:
	queue(int n=100){
		_cap=n;
		_elem=new T[_cap];
		_front=_elem;
		_end=_elem;
	}
	~queue(){
		delete[] _elem;
	}
	inline bool empty(){ return _front==_end;}
	inline int size(){
		int ans=_end-_front;
		if(ans<0)
			ans+=_cap;
		return ans;
	}
	inline T front(){ return *_front ; }
	inline void push(const T& e){
		*_end=e;++_end;
		if(_end-_elem>=_cap)_end=_elem; // 如果大于了最大位置，就变成初始位置
		if(_end==_front)expand(); // 如果相等就是满了，扩容
	}
	inline void pop(){
		if(_end==_front)return;
		++_front;
		if(_front-_elem>=_cap)_front=_elem;
	}
};
/*
#include<iostream>
#include<string>
using namespace std;

int main(){
	queue<int> q(10);
	string s;
	while(cin>>s){
		if(s=="pop")q.pop();
		else if(s=="front")cerr<<"front  "<<q.front()<<endl;
		else if(s=="push"){
			int tmp;
			cin>>tmp;
			q.push(tmp);
		}
		else{
			cerr<<q._elem<<' '<<q._front<<' '<<(q._end)<<endl;
			for(int* i=q._front;i!=q._end;++i)
			{
				if(i-q._elem>=q._cap)i=q._elem;
				cerr<<*i<<' ';cerr<<endl;
			}
			cerr<<"size: "<<q.size()<<endl;	
			cerr<<"empty: "<<q.empty()<<endl;
		}
	}
	return 0;
}*/

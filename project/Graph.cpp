#include <climits>
#include <vector>
#include <stack>
#include <cstdlib>
#include <queue>
#include <algorithm>
using namespace std;
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;//顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;//边在遍历树中所属的类型

template<typename Tv,typename Te>//顶点类型,边类型
class Graph{//图的模版类
private:
    void reset()//所有顶点,边,辅助信息复位
    {
        for(int i=0;i<n;++i)//所有顶点的
        {
            status(i)=UNDISCOVERED; dTime(i)=fTime(i)=-1;//状态,时间标签
            parent(i)=-1; priority(i)=INT_MAX;//父节点,优先级数;
            for(int j=0;j<n;++j)//所有边的
                if(exists(i,j))type(i,j)=UNDETERMINED;//类型
        }
    }
    void BFS(int,int&);
    void DFS(int,int&);
    void BCC(int,int&,stack<int>&);//连通域内基于DFS的双连通分量分解算法
    bool TSort(int,int&,stack<Tv>*);//基于DFS的 拓扑排序算法
    template<typename PU> void PFS(int,PU);//优先级搜索框架
public:
//顶点:
    int n=0;//顶点总数
    virtual int insert(Tv const&)=0;//插入顶点,返回编号
    virtual Tv remove(int)=0;//删除顶点及其关联边
    virtual Tv& vertex(int)=0;//顶点v的数据(该定点确实存在)
    virtual int inDegree(int)=0;//顶点v的入度
    virtual int outDegree(int)=0;//顶点v的出度
    virtual int firstNbr(int)=0;//顶点v的首个邻接顶点
    virtual int nextNbr(int,int)=0;//顶点v的下一个邻接点
    virtual VStatus& status(int)=0;//顶点v的状态
    virtual int& dTime(int)=0;//顶点v的时间标签dTime
    virtual int& fTime(int)=0;//顶点v的时间标签fTime
    virtual int& parent(int)=0;//顶点v在遍历树的父亲
    virtual int& priority(int)=0;//顶点在遍历树中的优先级数
//边 (约定:无向边是互逆的一对有向边,
    int e=0;//边总数
    virtual bool exists(int,int)=0;//边(u,v)是否存在
    virtual void insert(Te const&,int,int,int)=0;//在v,u之间插入权重为w的边
    virtual Te remove(int,int)=0;//删除v,u之间的边,返回改边的信息
    virtual EType& type(int,int)=0;//边(u,v)的类型
    virtual Te& edge(int,int)=0;//边(u,v)的数据
    virtual int& weight(int,int)=0;//边u,v的权重
//算法
    void bfs(int);
    void dfs(int);
    void bcc(int);
    stack<Tv>* tSort(int);
    void prim(int);//最小支撑树
    void dijkstra(int);//最短路径dijkstra算法
    template<typename PU> void pfs(int,PU);
};

template<typename Tv>struct Vertex{//z顶点对象
    Tv data;int inDegree,outDegree;VStatus status;//数据,出入度,状态
    int parent;int priority;//父节点,优先度
    int dTime,fTime;//时间标签
    Vertex(Tv const& d=(Tv)0)://构造新顶点
        data(d),inDegree(0),outDegree(0),status(UNDISCOVERED),
        dTime(-1),fTime(-1),parent(-1),priority(INT_MAX){}
};

template<typename Te>struct Edge{//边对象
    Te data;int weight;EType type;//数据,权重,类型
    Edge(Te const& d,int w):data(d),weight(w),type(UNDETERMINED){}
};

template<typename Tv,typename Te>class GraphMatrix: public Graph<Tv,Te>{//基于向量,以矩阵形式实现的图
private:
    vector< Vertex<Tv> > V;//顶点集
    vector< vector<Edge<Te>*> >E;//边集,邻接矩阵
public:
    GraphMatrix(){this->n=this->e=0;}
    ~GraphMatrix(){
        for(int j=0;j<this->n;++j)
            for(int k=0;k<this->n;++k)
            delete E[j][k];
    }
//顶点的基本操作,查询第i个顶点
    virtual Tv& vertex(int i){return V[i].data;}
    virtual int inDegree(int i){return V[i].inDegree;}
    virtual int outDegree(int i){return V[i].outDegree;}
    virtual int firstNbr(int i){return nextNbr(i,this->n);}
    virtual int nextNbr(int i,int j)//相对于顶点j的下一个邻接顶点
    {
        while( (-1<j) && (!exists(i,--j)) );return j;//逆向线性试探
    }
    virtual VStatus& status(int i){return V[i].status;}
    virtual int& dTime(int i){return V[i].dTime;}
    virtual int& fTime(int i){return V[i].fTime;}
    virtual int& parent(int i){return V[i].parent;}
    virtual int& priority(int i){return V[i].priority;}
//顶点的动态操作
    virtual int insert(Tv const& vertex)
    {//插入顶点,返回编号
        for(int j=0;j<this->n;++j)E[j].push_back(NULL);++this->n;//各顶点预留一条潜在的关联边
        E.push_back( vector<Edge<Te>*>(this->n,(Edge<Te>*)NULL )  );//创建新的顶点对应的边向量
        V.push_back( Vertex<Tv>(vertex) );//顶点向量增加一个顶点
        return V.size()-1;
    }
    virtual Tv remove(int i)
    {//删除第i个顶点及其关联边
        for(int j=0;j<this->n;++j)//所有出边
            if(exists(i,j)){delete E[i][j];--V[j].inDegree;}
        E.erase(i+E.begin());--this->n;//删除行
        Tv vbak=vertex(i);V.erase(i+V.begin());//删除顶点i
        for(int j=0;j<this->n;++j)//所有入边(即删除列)
            if(E[j][i]!=NULL){
                delete E[j][i];E[j].erase(i+E[j].begin());--V[j].outDegree;
            }
        return vbak;//返回被删除的顶点信息
    }
//边的确认操作
    virtual bool exists(int i,int j)//边(i,j)是否存在
    {return (0<=i) && (i<this->n) && (0<=j) && (j<this->n) && E[i][j] != NULL;}
//边的基本操作
    virtual EType& type(int i,int j){return E[i][j]->type;}
    virtual Te& edge(int i,int j){return E[i][j]->data;}
    virtual int& weight(int i,int j){return E[i][j]->weight;}
//边的动态操作
    virtual void insert(Te const& edge,int w,int i,int j)//插入权重为w的边e=(i,j)
    {
        if(exists(i,j))return;//如果存在就返回
        E[i][j]=new Edge<Te>(edge,w);
        ++(this->e);++V[i].outDegree;++V[j].inDegree;
    }
    virtual Te remove(int i,int j)
    {
        Te eBak = edge(i,j);delete E[i][j];E[i][j]=NULL;//备份后删除
        --(this->e);--V[i].outDegree;--V[j].inDegree;
        return eBak;//返回被删除边的信息
    }
};
template<typename Tv,typename Te>void Graph<Tv,Te>::bfs(int s)
{
    reset();int clock=0,v=s;//初始化
    do if(status(v) == UNDISCOVERED)BFS(v,clock);
    while(s!= (v=(++v%n)));//按序号检查
}
template<typename Tv,typename Te>void Graph<Tv,Te>::BFS(int v,int& clock)
{
    queue<int> Q;
    status(v)=DISCOVERED;Q.push(v);//初始化起点
    while(!Q.empty())
    {
        int v=Q.front();dTime(v)= ++clock;//取出队首v
        Q.pop();
        for(int u=firstNbr(v);u>-1;u=nextNbr(v,u))//枚举所有的邻居
        {
            if(status(u)==UNDISCOVERED)
            {
                status(u)=DISCOVERED;Q.push(u);//发现改顶点
                type(v,u)=TREE;parent(u)=v;//引入树边拓展支撑树
            }else{
                type(v,u)=CROSS;
            }
        }
        status(v)=VISITED;
    }
}
template<typename Tv,typename Te>void Graph<Tv,Te>::dfs(int s)
{
    reset();int clock=0,v=s;//初始化
    do if(status(v)==UNDISCOVERED)DFS(v,clock);
    while(s != ( v= (++v%n)));
}
template<typename Tv,typename Te>void Graph<Tv,Te>::DFS(int v,int& clock)
{
    dTime(v)=++clock;status(v)=DISCOVERED;
    for(int u=firstNbr(v);u>-1;u=nextNbr(v,u))
    {
        switch(status(u))
        {
        case UNDISCOVERED:
            type(v,u)=TREE;parent(u)=v;DFS(u,clock);break;
        case DISCOVERED:
            type(v,u)=BACKWARD;break;
        default:
            type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;break;
        }
    }
    status(v)=VISITED;fTime(v)=++clock;
}

template<typename Tv,typename Te> stack<Tv>* Graph<Tv,Te>::tSort(int s)
{
    reset();int clock=0;int v=s;
    stack<Tv> ss;
    stack<Tv>* S=&ss;//用栈记录排序点
    do{ if(status(v)==UNDISCOVERED)
        if(!TSort(v,clock,S)){
            while(!S->empty())S->pop();//任意连通域不是DAG时全部pop,返回空
            break;
        }
    }while(s!= (v=(++v%n)));
    return S;//若为DAG,S内各顶点自顶向底排序, 否则返回空
}
template<typename Tv,typename Te>bool Graph<Tv,Te>::TSort(int v,int& clock,stack<Tv>* S)
{
    dTime(v)=++clock;status(v)=DISCOVERED;
    for(int u=firstNbr(v);u>-1;u=nextNbr(v,u))
    {
        switch(status(u)){
        case UNDISCOVERED:
            parent(u)=v;type(v,u)=TREE;
            if(!TSort(u,clock,S))
                return false;
            break;
        case DISCOVERED:
            type(v,u)=BACKWARD;
            return false;//一旦发现后向边,返回并报告
        default:
            type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;
            break;
        }
    }
    status(v)=VISITED;S->push(vertex(v));
    return true;
}
template<typename Tv,typename Te>void Graph<Tv,Te>::bcc(int s)
{
    reset();int clock=0;int v=s;
    stack<int> S;
    do if(status(v)==UNDISCOVERED){
        BCC(v,clock,S);
        S.pop();//遍历返回时,弹出栈最后一个顶点(当前连通域的起点)
    }
    while(s !=(v = (++v%n)));
}
#define hca(x) (fTime(x))
template<typename Tv,typename Te>void Graph<Tv,Te>::BCC(int v,int& clock,stack<int>& S)
{
    hca(v)=dTime(v)=++clock;status(v)=DISCOVERED;
    S.push(v);
    for(int u=firstNbr(v);-1<u;u=nextNbr(v,u))
    {
        switch(status(u))
        {
        case UNDISCOVERED:
            parent(u)=v;type(v,u)=TREE;
            BCC(u,clock,S);
            if(hca(u)<dTime(v))//遍历返回后,若发现u可指向v的真祖先
                fTime(v)=min( fTime(v), fTime(u) );
            else{
                while(v != S.top())S.pop();
            }
            break;
        case DISCOVERED:
            type(v,u)=BACKWARD;
            if(u != parent(v))hca(v)=min(hca(v),dTime(u));
            break;
        default:
            type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;
        }
    }
    status(v)=VISITED;
}
#undef hca

template<typename Tv,typename Te>template<typename PU>void Graph<Tv,Te>::pfs(int s,PU prioUpdater)
{
    reset();int v=s;
    do if(status(v)==UNDISCOVERED)PFS(v,prioUpdater);
    while(s != ( v= (++v%n)));
}
template<typename Tv,typename Te>template<typename PU>void Graph<Tv,Te>::PFS(int s,PU prioUpdater)
{
    priority(s)=0;status(s)=VISITED;parent(s)=-1;
    while(1){
        for(int w=firstNbr(s);-1<w;w=nextNbr(s,w))
            prioUpdater(this,s,w);//更新顶点w的优先级及父节点
        for(int shortest = INT_MAX,w=0;w<n;++w)
        {
            if(status(w)==UNDISCOVERED)
                if(shortest>priority(w)){shortest=priority(w);s=w;}
        }
        if(status(s)==VISITED)break;
        status(s)=VISITED;type(parent(s),s)=TREE;
    }
}
int main()
{
    GraphMatrix<int,int> g;
    return 0;
}



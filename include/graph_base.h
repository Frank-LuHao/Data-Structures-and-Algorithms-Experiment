#if !defined(_GRAPH_BASE_H_)
#define _GRAPH_BASE_H_
#include <stdlib.h>
#include <memory.h>
#include<iostream>
#include "lk_stack.h"
#include "FindUnionSet.h"
#include "min_priority_heap_queue.h"
#include "SqList.h"
#include "lk_queue.h"
using namespace std;
template <class EdgeWType>
struct GraphEdge {			// 树的边结点类型
	int vex1, vex2;
	EdgeWType weight;		// 边的三元组(始点，终点，权值)
	bool operator<(const  GraphEdge& e) const {  						// 使用优先级队列需要重载<
		return weight < e.weight;
	}
	bool operator<=(const  GraphEdge& e) const {  						// 使用优先级队列需要重载<
		return weight <= e.weight;
	}
	bool operator>(const  GraphEdge& e) const {  						// 使用优先级队列需要重载<
		return weight > e.weight;
	}
	bool operator>=(const  GraphEdge& e) const {  						// 使用优先级队列需要重载<
		return weight >= e.weight;
	}

};
enum GRAPH_TYPE {//定义图的种类
	UNDIR_GRAPH = 0,//无向图
	DIR_GRAPH,//有向图
	UNDIR_NETWORK,//无向网
	DIR_NETWORK,//有向网
};
template <class ElemType,class NodeType,class EdgeWType>
class GraphBase
{

public:
	GraphBase(ElemType e[],int nVexNum, GRAPH_TYPE tGraph);
	~GraphBase();
	virtual int FirstAdjVex(int v, EdgeWType& weight) const = 0;			// 返回顶点v的第一个邻接点，并获得该边的权重
	virtual int NextAdjVex(int v1, int v2, EdgeWType &weight) const = 0;	// 返回顶点v1的相对于v2的下一个邻接点，并获得该边权重	 
	virtual void InsertEdge(int v1, int v2,EdgeWType weight) =0;			// 插入顶点为v1和v2,权为weight的边			 
	virtual void DeleteEdge(int v1, int v2) =0;			// 删除顶点为v1和v2的边		
	virtual EdgeWType InfniteWeight();				// 边的无穷大权值
	bool GetTag(int v) const;							// 返回顶点v的标志		 
	void SetTag(int v, bool val) ;						// 设置顶点v的标志为val	
	int GetVexNum() const;								// 返回顶点个数		
	int GetEdgeNum() const;								// 返回边数个数			 
	void DFSTraverse(void (*visit)(const ElemType& e)) const;	// 对图进行深度优先遍历
	void BFSTraverse(void (*visit)(const ElemType&)) const;	// 对图进行广度优先遍历
	bool GetElem(int v, ElemType& e) const;				// 用e返回顶点v的元素值	
	bool SetElem(int v, const ElemType& e);				// 设置顶点v的元素值为e
	bool Prim();										//用普里姆算法求最小生成树
	bool Kruskal();										//用克鲁斯卡尔算法生成最小生成树
	GraphEdge <EdgeWType>* GetMstEdge(int& nMsgEdgeNum) const;	//输出最小生成树
	bool TopSort(SqList<int>& lstSortedVex) const;				//拓扑排序
	bool CriticalPath(SqList<GraphEdge<EdgeWType>>& lstCritalPathEdge) const;		//求网的关键路径
	bool Dijkstra(int start, EdgeWType D[], int pre[], int nBufSize); //用迪杰斯特拉算法求最短路径
	GraphBase<ElemType, NodeType, EdgeWType>& operator =(const GraphBase<ElemType, NodeType, EdgeWType>& source);//重载赋值运算符
	int InsertEdges(const GraphEdge<EdgeWType> edge[], int nEdgeNums);		//依据边序列插入多条边
protected:
	GraphEdge <EdgeWType>* m_pMstEdge;				// 最小生成树的边集
	// 辅助函数模板:
	void DFSAux(int v, void (*visit)(const ElemType& e)) const;		// 从顶点v出发进行深度优先搜索图
	void BFSAux(int v, void (*visit)(const ElemType&)) const;		// 从第顶点v出发进行广度优先搜索图
	virtual void GetNodeElem(const NodeType& node,ElemType &e) const =0;//根据节点类型获取原子数据
	virtual void SetNodeElem(NodeType& node,const  ElemType& e)  = 0;	//把原子数据存入到节点类型数据中
	void ClearTag();//清空顶点访问标记
	void ReSize(int nVexNum);//重新分配存储空间
	void StatInDegree(int inDegree[]) const;							//统计顶点入度
	bool* m_pbVistedTabel;//节点访问标志表
	NodeType* m_pNodeTabel;//节点表
	int m_nVexNum;//节点数目
	int m_nEdgeNum;//边数
	int m_nMstEdgeNum;//最小生成树边个数
	GRAPH_TYPE m_type;//图的属性
};
template <class ElemType, class NodeType, class EdgeWType>
//操作结果：由原子类型数据序列构造图
GraphBase<ElemType,NodeType, EdgeWType>::GraphBase(ElemType e[],int nVexNum, GRAPH_TYPE tGraph)
{
	m_pNodeTabel = new NodeType[nVexNum];		//创建顶点数组
	m_pbVistedTabel = new bool[nVexNum];		//创建访问标志数组
	m_pMstEdge = NULL;							//最小生成树初始化为空
	for (int i = 0; i < nVexNum; i++)				
		SetElem(i, e[i]);						//拷贝节点原子数据
	memset(m_pbVistedTabel, 0, sizeof(bool) * nVexNum);	//清空访问标志
	m_nVexNum = nVexNum;						//赋值顶点数
	m_nEdgeNum = 0;								//边数初始化为0
	m_nMstEdgeNum = 0;							//最小生成树边数初始化为0
	m_type = tGraph;							//设置图的属性（有向、无向、网等）
}

template <class ElemType, class NodeType, class EdgeWType>
GraphBase<ElemType, NodeType, EdgeWType>::~GraphBase()
{
	if (m_pNodeTabel)
	{
		delete[]m_pNodeTabel;
		m_pNodeTabel = NULL;
	}
	if (m_pbVistedTabel)
	{
		delete[]m_pbVistedTabel;
		m_pbVistedTabel = NULL;
	}
	if (m_pMstEdge)
	{
		delete []m_pMstEdge;
		m_pMstEdge = NULL;

	}
	m_nVexNum = 0;
	m_nEdgeNum = 0;
	m_nMstEdgeNum = 0;
}

template <class ElemType, class NodeType, class EdgeWType>
//操作结果：重新分配顶点存储空间
void GraphBase<ElemType, NodeType, EdgeWType>::ReSize(int nVexNum)
{
	if (m_pNodeTabel)
	{
		delete[]m_pNodeTabel;
	}
	if (m_pbVistedTabel)
	{
		delete[]m_pbVistedTabel;
	}
	if (m_pMstEdge)
	{
		delete[]m_pMstEdge;
		m_pMstEdge = NULL;

	}
	m_pNodeTabel = new NodeType[nVexNum];
	m_pbVistedTabel = new bool[nVexNum];
	memset(m_pbVistedTabel, 0, sizeof(bool) * nVexNum);//清空访问标志
	m_nVexNum = nVexNum;
	m_nEdgeNum = 0;
	m_nMstEdgeNum = 0;
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果：依据边序列插入多条边
// 参数：edge-边数组
//		nEdge-边数组大小
//返回：当前边数
int GraphBase<ElemType, NodeType, EdgeWType>::InsertEdges(const GraphEdge<EdgeWType> edge[], int nEdgeNums)
{
	int i;
	for (i = 0; i < nEdgeNums; i++)
		InsertEdge(edge[i].vex1, edge[i].vex2, edge[i].weight);
	return m_nEdgeNum;
}
	
template <class ElemType, class NodeType, class EdgeWType>
//操作结果： 用e返回顶点v的元素值
bool GraphBase<ElemType, NodeType, EdgeWType>::GetElem(int v, ElemType& e) const
{
	if (v < 0 || v >= m_nVexNum || !m_pNodeTabel)
		return false;
	GetNodeElem(m_pNodeTabel[v],e);
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果： 顶点v的元素值e
bool GraphBase<ElemType, NodeType, EdgeWType>::SetElem(int v, const ElemType& e)
{
	if (v < 0 || v >= m_nVexNum || !m_pNodeTabel)
		return false;
	SetNodeElem(m_pNodeTabel[v], e);
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::ClearTag()
{
	if (!m_pbVistedTabel)
		return;
	memset(m_pbVistedTabel, 0, sizeof(bool) * m_nVexNum);//顶点访问表所有内存空间清零
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::DFSAux(int v, void (*visit)(const ElemType& e)) const
// 初始条件：存在图
// 操作结果：从顶点v出发进行深度优先搜索图，递归结束时完成联通子图的访问
{
	SetTag(v, true);							// 设置访问标志
	ElemType e;									// 临时变量
	GetElem(v, e);								// 取顶点v的数据元素
	(*visit)(e);								// 访问顶点v的数据元素
	EdgeWType weight;							// 边的权值临时变量
	for (int w = FirstAdjVex(v,weight); w != -1; w = NextAdjVex(v, w, weight))
	{	// 对v的尚未访问过的邻接顶点w递归调用DFSAux
		if (!GetTag(w))	DFSAux(w, visit);
	}
}
template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::DFSTraverse(void (*visit)(const ElemType& e)) const
// 初始条件：存在图
// 操作结果：对图进行深度优先遍历
{
	int v;
	ClearTag();//清空顶点访问标记
	
	for (v = 0; v < m_nVexNum; v++)
	{	// 对尚未访问的顶点按DFS进行深度优先搜索
		if (!GetTag(v))	DFSAux(v, visit);
	}
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::BFSAux(int v, void (*visit)(const ElemType&)) const
// 初始条件：存在图
// 操作结果：从第顶点v出发进行广度优先搜索图
{
	SetTag(v, true);						// 设置访问标志
	ElemType e;								// 临时变量
	GetElem(v, e);							// 取顶点v的数据元素
	(*visit)(e);							// 访问顶点v的数据元素
	LinkQueue<int> q;						// 定义队列
	q.InQueue(v);							// v入队
	EdgeWType weight ;
	while (!q.IsEmpty())
	{	// 队列q非空, 进行循环
		int u, w;							// 临时顶点
		q.OutQueue(u);						// 出队
		for (w = FirstAdjVex(u, weight); w >= 0; w = NextAdjVex(u, w, weight))
		{	// 对u尚未访问过的邻接顶点w进行访问
			if (!GetTag(w))
			{	// 对w进行访问
				SetTag(w, true);			// 设置访问标志
				GetElem(w, e);				// 取顶点w的数据元素
				(*visit)(e);				// 访问顶点w的数据元素
				q.InQueue(w);				// w入队
			}
		}
	}
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::BFSTraverse(void (*visit)(const ElemType&)) const
// 初始条件：存在图
// 操作结果：对图进行广度优先遍历
{
	ClearTag();//清空顶点访问标记
	int v;
	for (v = 0; v < m_nVexNum; v++)
	{	// 对尚未访问的顶点按BFS进行深度优先搜索
		if (!GetTag(v)) BFSAux(v, visit);
	}
}


template <class ElemType, class NodeType, class EdgeWType>
int GraphBase<ElemType, NodeType, EdgeWType>::GetVexNum() const
// 操作结果：返回顶点个数			 
{
	return m_nVexNum;
}
template <class ElemType, class NodeType, class EdgeWType>
// 操作结果：返回边数	
int GraphBase<ElemType, NodeType, EdgeWType>::GetEdgeNum() const
{
	return m_nEdgeNum;
}
template <class ElemType, class NodeType, class EdgeWType>
bool GraphBase<ElemType, NodeType, EdgeWType>::GetTag(int v) const
// 操作结果：返回顶点v的标志		 
{
	if (v < 0 || v >= m_nVexNum)
	{	// 出现异常
		cout << "v不合法!" << endl;					// 提示信息
		exit(12);									// 退出程序
	}
	
	return m_pbVistedTabel[v];// 返回顶点v的标志
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::SetTag(int v, bool val)
// 操作结果：设置顶点v的标志为val		 
{
	if (v < 0 || v >= m_nVexNum)
	{	// 出现异常
		cout << "v不合法!" << endl;					// 提示信息
		exit(13);									// 退出程序
	}

	m_pbVistedTabel[v]=val;									// 设置顶点v的标志为val
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果:用Prim算法生成最小生成树
bool GraphBase<ElemType, NodeType, EdgeWType>::Prim() 
{
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 || m_type != UNDIR_NETWORK)
		return false;//不存在顶点或边，以及不是无向网，返回失败
	struct Dist {
		int  adjVex;		// 最小代价边依附的顶点编号(<数组下标,adjVex>组成的边,其中adjVex已在Vnew中)
		EdgeWType  lowCost;	// 最小代价
	}*D = new Dist[m_nVexNum];//D为Vnew的候选顶点集合，如果顶点lowCost为无穷大，则该顶点为非候选顶点
	
	ClearTag();//清空顶点访问标记
	if (m_pMstEdge)
		delete[]m_pMstEdge;//曾经存在最小生成树边，删除树边
	m_pMstEdge = new GraphEdge<EdgeWType>[m_nVexNum];
	EdgeWType minCost;	//最小代价临时变量
	int u,w, i, j, count = 0;
	EdgeWType InfniteW = InfniteWeight();//获取边权值的无穷大值
	for (i = 0; i < m_nVexNum; ++i) {
		
		D[i].lowCost = InfniteW;//初始化时所有最小代价都赋值为无穷大
	}
	// 初始化Vnew集合
	u = 0; 
	SetTag(u,true) ;  	
	EdgeWType weight;
	for (i = 1; i < m_nVexNum; ++i) 
	{	// 选中一个点u入Vnew集合后,进入迭代过程
		for (w = FirstAdjVex(u, weight); w>=0; w=NextAdjVex(u, w, weight))  // 更新和u关联的（V-Vnew）集合邻接点的权值
			if (!GetTag(w) && D[w].lowCost > weight)
			{//w在(V-Vnew)集合，且边(w,D[W].AdjVex)权>边(u,w)权
					D[w].lowCost = weight;		// 更新lowcost
					D[w].adjVex = u; 			// 更新adjVex 
			}
		minCost = InfniteW;
		for (j = 0; j < m_nVexNum; ++j)			// 在候选顶点集合中找lowCost最小顶点u
			if (D[j].lowCost < minCost)
			{
				minCost = D[j].lowCost;  u = j;
			}
		m_pMstEdge[count].vex1 = D[u].adjVex; 		// 保存最小生成树的一条边
		m_pMstEdge[count].vex2 = u;
		m_pMstEdge[count++].weight = D[u].lowCost;
		D[u].lowCost = InfniteW;					// 顶点u已并入Vnew集合
		SetTag(u, true);//设置u已经访问
	}
	delete[] D;
	m_nMstEdgeNum = count;
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果:用Kruskal算法生成最小生成树
bool GraphBase<ElemType, NodeType, EdgeWType>::Kruskal()
{
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 || m_type != UNDIR_NETWORK)
		return false;//不存在顶点或边，以及不是无向网，返回失败
	if (m_pMstEdge)
	{
		delete[]m_pMstEdge;
	}
	m_pMstEdge = new GraphEdge<EdgeWType>[m_nVexNum];

	int count = 0;
	GraphEdge<EdgeWType> e;
	
	FindUnionSet S(m_nVexNum);    		// 并查集S
	MinPriorityHeapQueue <GraphEdge<EdgeWType>> Q; 		// 优先级队列Q
	int i,w;
	EdgeWType weight;
	for (int i = 0; i < m_nVexNum; ++i) 
	{	// 边入优先级队列
		for (w = FirstAdjVex(i, weight); w >= 0;w= NextAdjVex(i, w, weight))  // 更新u关联的顶点的D值
			if (i < w) 
			{			// 防止重复入队
				e.vex1 = i;
				e.vex2 = w;
				e.weight = weight;
				Q.InQueue(e); 		// 边e入队
			}
	}
	while (count < m_nVexNum - 1) 
	{	// 选出m_nVexNum-1条边
		 if(!Q.OutQueue(e)) break;		// 从优先级队列出队一条边
		if (!S.IsSameTree(e.vex1,e.vex2)) 
		{	// 边上的两个顶点不属于同一连通分量
			S.Union(e.vex1, e.vex2); 		// 合并e.vex1, e.vex2所属子集（连通分量）
			m_pMstEdge[count++] = e;		// 保存最小生成树上的一条边
		}
	}
	m_nMstEdgeNum = count;
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果:输出最小生成树
GraphEdge <EdgeWType>* GraphBase<ElemType, NodeType, EdgeWType>::GetMstEdge(int& nMsgEdgeNum) const
{
	nMsgEdgeNum = m_nMstEdgeNum;
	return m_pMstEdge;
}
template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::StatInDegree(int inDegree[]) const
//操作结果:统计入度
{
	int i, w;//临时变量
	EdgeWType weight;//权值临时变量
	for (i = 0; i < m_nVexNum; i++)
		inDegree[i] = 0;
	for (i = 0; i < m_nVexNum; i++)
	{	 // 遍历边表，求顶点入度
		for (w = FirstAdjVex(i, weight); w >= 0; w = NextAdjVex(i, w, weight))
			++inDegree[w];
	}
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果:拓扑排序,排序好的顶点下标输出到lstSortedVex线性表中，返回是否排序成功
bool GraphBase<ElemType, NodeType, EdgeWType>::TopSort(SqList<int>& lstSortedVex) const
{
	if (m_type == UNDIR_NETWORK || m_type == UNDIR_GRAPH)
		return false;//有向图才能进行拓扑排序
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 )
		return false;//不存在顶点或边，返回失败
	LinkStack <int> topSortS;			// 用于拓扑排序存储入度为0的顶点栈
	
	int i, curNode, count = 0,w;
	EdgeWType weight;					//权值临时变量
	int *inDegree = new int[m_nVexNum];	//入度数组
	StatInDegree(inDegree);				//统计顶点入度
	for (i = 0; i < m_nVexNum; i++) 	// 入度为0的顶点入栈
		if (inDegree[i] == 0) topSortS.Push(i);
	while (!topSortS.IsEmpty())
	{
		if (!topSortS.Pop(curNode))		// 出栈一个为0的顶点，失败时退出循环
			break;
		lstSortedVex.AddTail(curNode);	//顶点序号输入到线性表中
		count++;				// 计数器+1
		for (w = FirstAdjVex(curNode, weight); w >= 0; w = NextAdjVex(curNode, w, weight))
			if (--inDegree[w] == 0) 	// 邻接点入度减1
				topSortS.Push(w); 			// 入度为0的顶点入栈
	}
	delete []inDegree;
	if (count == m_nVexNum)
		return true; 			// 输出全部顶点，拓扑排序成功
	return false; 			// 该有向图有环，拓扑排序失败
}
template <class ElemType, class NodeType, class EdgeWType>
bool GraphBase<ElemType, NodeType, EdgeWType>::CriticalPath(SqList<GraphEdge<EdgeWType>>& lstCritalPathEdge) const
//操作结果：求网的关键路径
//参数：lstCritalPathEdge - 输出关键路径的网边
//返回：操作是否成功
{
	if (m_type != DIR_NETWORK )		return false;//有向网才能求关键路径
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 )	return false;//不存在顶点或边，返回失败
	int* inDegree = new int[m_nVexNum];	// 顶点入度数组
	StatInDegree(inDegree);				//统计顶点入度
	int* ve = new int[m_nVexNum];		// 事件最早发生时刻数组
	LinkStack<int> topSortS;			// 用于拓扑排序存储入度为0的顶点栈
	LinkStack<int> reverseS;			// 用于实现逆扑序序列的栈
	int ee, el, u, v, count = 0;
	EdgeWType weight;					//权值
	for (v = 0; v < m_nVexNum; v++)
	{	// 初始化事件最早发生时刻
		ve[v] = 0;
	}

	for (v = 0; v < m_nVexNum; v++)
	{	// 遍历顶点
		if (inDegree[v] == 0)
		{	// 建立入度为0的顶点栈
			topSortS.Push(v);					// 入栈
		}
	}

	while (!topSortS.IsEmpty())
	{	// 栈非空
		topSortS.Pop(u);						// 取出一个入度为0的顶点
		reverseS.Push(u);						// 顶点u入栈，以便得逆拓扑排序序列
		count++;								// 对顶点进行记数
		for (v = FirstAdjVex(u, weight); v != -1; v = NextAdjVex(u, v, weight))
		{	// v2为v1的一个邻接点,对v1的每个邻接点入度减1
			if (--inDegree[v] == 0)
			{	// v入度为0，将v入栈
				topSortS.Push(v);				// 入栈 
			};
			if (ve[u] + weight > ve[v])
			{	// 修改ve[v]
				ve[v] = ve[u] + weight;
			}
		}
	}
	delete[]inDegree;							// 释放inDegree所占用的存储空间

	if (count < m_nVexNum)
	{
		delete[]ve;							// 释放ve所占用的存储空间
		return false;							// 网g有回路
	}

	int* vl = new int[m_nVexNum];			// 事件最迟发生时刻数组
	reverseS.Top(u);							// 取出栈顶，栈顶为汇点
	for (v = 0; v < m_nVexNum; v++)
	{	// 初始化事件最迟发生时刻
		vl[v] = ve[u];
	}

	while (!reverseS.IsEmpty())
	{	// reverseS非空
		reverseS.Pop(u);						// 出栈
		for (v = FirstAdjVex(u,weight); v != -1; v = NextAdjVex(u, v,weight))
		{	// v为u的一个邻接点
			if (vl[v] - weight < vl[u])
			{	// 修改vl[u]
				vl[u] = vl[v] - weight;
			}
		}
	}
	GraphEdge<EdgeWType> edge;			//边临时变量
	for (u = 0; u < m_nVexNum; u++)
	{	// 求ee, el和关键路径
		for (v = FirstAdjVex(u, weight); v != -1; v =NextAdjVex(u, v, weight))
		{	// v为u的一个邻接点
			ee = ve[u]; el = vl[v] - weight;
			if (ee == el)
			{	// <u, v>为关键活动,输出关键活动
				edge.vex1 = u;
				edge.vex2 = v;
				edge.weight = weight;
				lstCritalPathEdge.AddTail(edge);
				//cout << "<" << u << "," << v << "> ";	// 输出关键活动
			}
		}
	}

	delete[]ve;							// 释放ve所占用的存储空间
	delete[]vl;							// 释放vl所占用的存储空间
	return true;							// 操作成功
}

template <class ElemType, class NodeType, class EdgeWType>
//操作结果：用dijkstra方法求最端路径
//参数: start-起始顶点序号
//		 D[] - 记录最短路径长度的数组
//		 pre[] -记录前驱的数组
//		 nBufSize - D数组和pre数组的长度
//返回:  操作是否成功
bool GraphBase<ElemType, NodeType, EdgeWType>::Dijkstra(int start, EdgeWType D[],int pre[],int nBufSize) 
{
	if (start < 0 || start > m_nVexNum - 1)	//源点下标越界
		return false;
	if (!D || !pre || nBufSize < m_nVexNum)
		return false;						//缓冲区错误
	if (m_type != DIR_NETWORK)
		return false;						//只有有向网才能求最短路径
	ClearTag();//清空顶点访问标志
	EdgeWType min;   
	int i, j, k, w;
	EdgeWType weight;
	EdgeWType InfniteW = InfniteWeight();//获取边权值的无穷大值
	for (i = 0; i < m_nVexNum; ++i) 
	{	// 初始化
		D[i] = InfniteW;  pre[i] = -1;
	}
	D[start] = 0;
	pre[start] = start;	// start到自身的路径长度置0
	min = D[start];  k = start;

	for (i = 1; i < m_nVexNum; ++i) 
	{
		SetTag(k, true);//设置顶点k的访问标志
		// 刷新start到k的邻接点的最短路径长度
		for (w = FirstAdjVex(k, weight); w >= 0; w = NextAdjVex(k, w, weight))
			if (!GetTag(w) && D[w] > min + weight) 
			{//顶点w还没有访问，且start到w的边距大于min+weight
				D[w] = min + weight;
				pre[w] = k;
			}
		min = InfniteW; k = start;
		for (j = 0; j < m_nVexNum; ++j) // 重新找当前路径长度最短且未被访问过的顶点k
			if (!GetTag(j) && D[j] < min) 
			{
				k = j;
				min = D[k];
			}
	}
	return  true;
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果：重载赋值运算符
GraphBase<ElemType, NodeType, EdgeWType>& GraphBase<ElemType, NodeType, EdgeWType>::operator =(const GraphBase<ElemType, NodeType, EdgeWType>& source)
{
	if (m_nVexNum != source.m_nVexNum)
	{
		ReSize(source.m_nVexNum);
	}
	m_nEdgeNum = source.m_nEdgeNum;
	m_nVexNum = source.m_nVexNum;
	m_nMstEdgeNum = source.m_nMstEdgeNum;
	if (source.m_pMstEdge)
	{
		m_pMstEdge = new GraphEdge<EdgeWType>[source.m_nMstEdgeNum];
		memcpy(m_pMstEdge, source.m_pMstEdge, sizeof(GraphEdge<EdgeWType>) * source.m_nMstEdgeNum);//拷贝最小生成树
	}
	int i;
	NodeType* pSrcNodeTabel = source.m_pNodeTabel;
	for (i = 0; i < m_nVexNum; i++)
	{
		m_pNodeTabel[i] = pSrcNodeTabel[i];
	}
	memcpy(m_pbVistedTabel, source.m_pbVistedTabel, sizeof(bool) * this->m_nVexNum);//拷贝顶点访问标记数组
	m_type = source.m_type;//复制图的种类
	return *this;
}
template <class ElemType, class NodeType, class EdgeWType>
//操作结果：返回无穷大
EdgeWType GraphBase<ElemType, NodeType, EdgeWType>::InfniteWeight()
{
	int typeSize = sizeof(EdgeWType);
	if (typeSize == 1)
		return 127;//可能是8位整数
	else if (typeSize == 2)
		return 32767;//可能是16位整数
	else
		return 2147483647;//最大的32位整数
}
#endif
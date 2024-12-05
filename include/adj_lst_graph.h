#if !defined(_ADJ_LST_GRAPH_H_)
#define _ADJ_LST_GRAPH_H_
#include "adj_lst_graph_node.h"
#include "graph_base.h"
template <class ElemType,class EdgeWType>
class AdjListGraph :public GraphBase<ElemType, AdjListGraphVexNode<ElemType, EdgeWType>, EdgeWType>
{
public:
	AdjListGraph(ElemType e[], int nVexNum, GRAPH_TYPE tGraph);//构造函数
	~AdjListGraph();//析构函数
	//重载基类中定义的纯虚函数
	virtual int FirstAdjVex(int v, EdgeWType& weight) const;			// 返回顶点v的第一个邻接点，并获得该边的权重
	virtual int NextAdjVex(int v1, int v2, EdgeWType& weight) const;	// 返回顶点v1的相对于v2的下一个邻接点，并获得该边权重	 
	virtual void InsertEdge(int v1, int v2, EdgeWType weight);			// 插入顶点为v1和v2,权为weight的边			 
	virtual void DeleteEdge(int v1, int v2);							// 删除顶点为v1和v2的边		
	AdjListGraph(const AdjListGraph<ElemType, EdgeWType>& source);		// 复制构造函数
private:
	//重载基类中定义的私有纯虚函数
	virtual void GetNodeElem(const AdjListGraphVexNode<ElemType, EdgeWType>& node, ElemType& e) const;//根据节点类型获取原子数据
	virtual void SetNodeElem(AdjListGraphVexNode<ElemType, EdgeWType>& node, const  ElemType& e);	//把原子数据存入到节点类型数据中
};
template <class ElemType, class EdgeWType>
//操作结果：构造邻近链表类
AdjListGraph<ElemType, EdgeWType>::AdjListGraph(ElemType e[], int nVexNum, GRAPH_TYPE tGraph)
	:GraphBase<ElemType, AdjListGraphVexNode<ElemType,EdgeWType>, EdgeWType>(e, nVexNum, tGraph)
{

}
template <class ElemType, class EdgeWType>
//操作结果：析构邻近链表类
AdjListGraph<ElemType, EdgeWType>::~AdjListGraph()
{

}
template <class ElemType, class EdgeWType>
AdjListGraph<ElemType, EdgeWType>::AdjListGraph(const AdjListGraph<ElemType, EdgeWType>& source)
{
	*this = source;//由于重载了'='运算符
}

template <class ElemType, class EdgeWType>
void AdjListGraph<ElemType, EdgeWType>::GetNodeElem(const AdjListGraphVexNode<ElemType, EdgeWType>& node, ElemType& e) const
{
	e = node.data;
}
template <class ElemType, class EdgeWType>
void AdjListGraph<ElemType, EdgeWType>::SetNodeElem(AdjListGraphVexNode<ElemType, EdgeWType>& node, const  ElemType& e)
{
	node.data = e;
}

template <class ElemType, class EdgeWType>
//操作结果：插入顶点为v1和v2,权为weight的边
void AdjListGraph<ElemType, EdgeWType>::InsertEdge(int v1, int v2, EdgeWType weight)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//顶点不正确，返回
	if (this->m_type == UNDIR_GRAPH || this->m_type == DIR_GRAPH)
		weight = 1;//对于图，权为1
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//是无向网或无向图
		if (this->m_pNodeTabel[v1].InsertEdge(v2, weight) &&
			this->m_pNodeTabel[v2].InsertEdge(v1, weight))//以v1为顶点和以v2为顶点都同时插入
			this->m_nEdgeNum++;
	}
	else
	{//有向图或网
		if (this->m_pNodeTabel[v1].InsertEdge(v2, weight))
			this->m_nEdgeNum++;
	}
}
template <class ElemType, class EdgeWType>
// 操作结果：删除顶点为v1和v2的边
void AdjListGraph<ElemType, EdgeWType>::DeleteEdge(int v1, int v2)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//顶点不正确，返回
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//是无向网或无向图
		if(this->m_pNodeTabel[v1].DeleteEdge(v2) &&
			this->m_pNodeTabel[v2].DeleteEdge(v1))
			this->m_nEdgeNum--;

	}
	else
	{
		if (this->m_pNodeTabel[v1].DeleteEdge(v2))
			this->m_nEdgeNum--;
	}
}
template <class ElemType, class EdgeWType>
// 操作结果：返回顶点v的第一个邻接点，并获得该边的权重
int AdjListGraph<ElemType, EdgeWType>::FirstAdjVex(int v, EdgeWType& weight) const
{
	if (v < 0 || v >= this->m_nVexNum)
		return -1;//顶点不正确，返回失败（顶点序号为-1）
	
	return 	this->m_pNodeTabel[v].FirstEdge(weight);
}
template <class ElemType, class EdgeWType>
// 操作结果：返回顶点v1的相对于v2的下一个邻接点，并获得该边权重
int AdjListGraph<ElemType, EdgeWType>::NextAdjVex(int v1, int v2, EdgeWType& weight) const
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return -1;//顶点不正确，返回
	if (v1 == v2)
		return -1;//v1不能为v2
	return 	this->m_pNodeTabel[v1].NextEdge(v2, weight);
}
#endif
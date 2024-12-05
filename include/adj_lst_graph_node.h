#if !defined(_ADJ_LST_GRAPH_NODE_H_)
#define _ADJ_LST_GRAPH_NODE_H_

// 邻接表图顶点结点类模板
#include "SimpleLkList.h"
template <class EdgeWType>
struct AdjLstGraphEdge
{
	int nVex;//顶点编号
	EdgeWType weight;
};

template <class ElemType,class EdgeWType>
class AdjListGraphVexNode
{
public:
	AdjListGraphVexNode();
	~AdjListGraphVexNode();
	bool InsertEdge(int nVex, EdgeWType weight);	//插入边
	bool DeleteEdge(int nVex);					//删除边
	int FirstEdge(EdgeWType &weight) const;			//获取第一条边
	int NextEdge(int nVexPre, EdgeWType& weight) const; //获取前驱为nVexPre的下一条边
	ElemType data;							// 数据元素值
	SimpleLkList<AdjLstGraphEdge<EdgeWType>> adjLink;					// 邻接点链表
	AdjListGraphVexNode<ElemType, EdgeWType>& operator =(const AdjListGraphVexNode<ElemType, EdgeWType>& source);//重载赋值运算符
};
template <class ElemType,class EdgeWType>
AdjListGraphVexNode<ElemType, EdgeWType>::AdjListGraphVexNode()
{

}
template <class ElemType, class EdgeWType>
AdjListGraphVexNode<ElemType, EdgeWType>::~AdjListGraphVexNode()
{
}
template <class ElemType, class EdgeWType>
AdjListGraphVexNode<ElemType, EdgeWType>& AdjListGraphVexNode<ElemType, EdgeWType>::operator =(const AdjListGraphVexNode<ElemType, EdgeWType>& source)
{
	data = source.data;
	adjLink = source.adjLink;
}

template <class ElemType, class EdgeWType>
//操作结果：插入边
bool AdjListGraphVexNode<ElemType, EdgeWType>::InsertEdge(int nVex, EdgeWType weight)
{
	//先查找链表中有要插入的顶点，遍历完成后再插入边
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	pLst->Head();			//链表当前节点回归头节点
	AdjLstGraphEdge<EdgeWType> edge;	//定义边的变量
	while (pLst->Next(edge))
	{//循环调用链表Next方法，当前节点指向后继，并获得后继的原子数据
		if (edge.nVex == nVex)
		{//顶点已经存在
			return false;		//边已经存在，返回失败
		}
	}
	edge.nVex = nVex;			//赋值边顶点
	edge.weight = weight;		//赋值边的权
	pLst->AddTail(edge);		//把边加入到链表中
	return true;			//返回成功
}


template <class ElemType, class EdgeWType>
//操作结果：删除边
bool AdjListGraphVexNode<ElemType, EdgeWType>::DeleteEdge(int nVex)
{
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	pLst->Head();
	AdjLstGraphEdge<EdgeWType> edge;
	while (pLst->Next(edge))
	{
		if (edge.nVex == nVex)
		{//顶点存在
			pLst->DeleteCur(edge);//删除当前节点
			return true;
		}
	}
	return false;//遍历完成都没有找到nVex边，返回失败
}
template <class ElemType, class EdgeWType>
//操作结果：获取第一条边的顶点
int AdjListGraphVexNode<ElemType, EdgeWType>::FirstEdge(EdgeWType& weight) const
{
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	if (pLst->IsEmpty())
		return -1;
	pLst->Head();
	AdjLstGraphEdge<EdgeWType> edge;
	if (!pLst->Next(edge))
		return -1;//获取首元节点失败
	weight = edge.weight;
	return edge.nVex;
}
template <class ElemType, class EdgeWType>
//操作结果：获取前驱为nVexPre的下一条边
int AdjListGraphVexNode<ElemType, EdgeWType>::NextEdge(int nVexPre, EdgeWType& weight) const
{
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	if (pLst->IsEmpty())
		return -1;
	AdjLstGraphEdge<EdgeWType> edge;
	if (pLst->GetCurElem(edge))
	{//如果是连续调用NextEdge,很大可能上一次得到的顶点就是nVexPre,此时不需要遍历就可以直接得到结果
		if (nVexPre == edge.nVex)
		{
			if (!pLst->Next(edge))
				return -1;//上一次操作已经是尾节点，返回失败
			weight = edge.weight;
			return edge.nVex;
		}
	}
	//采用遍历操作查找nVexPre
	pLst->Head();
	while (pLst->Next(edge))
	{
		if (edge.nVex == nVexPre)
		{
			if (!pLst->Next(edge))
				return -1;//已经是尾节点，返回失败
			weight = edge.weight;
			return edge.nVex;

		}
	}
	//没有找到前节点
	return -1;
}
#endif
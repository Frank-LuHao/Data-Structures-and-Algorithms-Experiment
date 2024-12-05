#if !defined(_ADJ_LST_GRAPH_H_)
#define _ADJ_LST_GRAPH_H_
#include "adj_lst_graph_node.h"
#include "graph_base.h"
template <class ElemType,class EdgeWType>
class AdjListGraph :public GraphBase<ElemType, AdjListGraphVexNode<ElemType, EdgeWType>, EdgeWType>
{
public:
	AdjListGraph(ElemType e[], int nVexNum, GRAPH_TYPE tGraph);//���캯��
	~AdjListGraph();//��������
	//���ػ����ж���Ĵ��麯��
	virtual int FirstAdjVex(int v, EdgeWType& weight) const;			// ���ض���v�ĵ�һ���ڽӵ㣬����øñߵ�Ȩ��
	virtual int NextAdjVex(int v1, int v2, EdgeWType& weight) const;	// ���ض���v1�������v2����һ���ڽӵ㣬����øñ�Ȩ��	 
	virtual void InsertEdge(int v1, int v2, EdgeWType weight);			// ���붥��Ϊv1��v2,ȨΪweight�ı�			 
	virtual void DeleteEdge(int v1, int v2);							// ɾ������Ϊv1��v2�ı�		
	AdjListGraph(const AdjListGraph<ElemType, EdgeWType>& source);		// ���ƹ��캯��
private:
	//���ػ����ж����˽�д��麯��
	virtual void GetNodeElem(const AdjListGraphVexNode<ElemType, EdgeWType>& node, ElemType& e) const;//���ݽڵ����ͻ�ȡԭ������
	virtual void SetNodeElem(AdjListGraphVexNode<ElemType, EdgeWType>& node, const  ElemType& e);	//��ԭ�����ݴ��뵽�ڵ�����������
};
template <class ElemType, class EdgeWType>
//��������������ڽ�������
AdjListGraph<ElemType, EdgeWType>::AdjListGraph(ElemType e[], int nVexNum, GRAPH_TYPE tGraph)
	:GraphBase<ElemType, AdjListGraphVexNode<ElemType,EdgeWType>, EdgeWType>(e, nVexNum, tGraph)
{

}
template <class ElemType, class EdgeWType>
//��������������ڽ�������
AdjListGraph<ElemType, EdgeWType>::~AdjListGraph()
{

}
template <class ElemType, class EdgeWType>
AdjListGraph<ElemType, EdgeWType>::AdjListGraph(const AdjListGraph<ElemType, EdgeWType>& source)
{
	*this = source;//����������'='�����
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
//������������붥��Ϊv1��v2,ȨΪweight�ı�
void AdjListGraph<ElemType, EdgeWType>::InsertEdge(int v1, int v2, EdgeWType weight)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//���㲻��ȷ������
	if (this->m_type == UNDIR_GRAPH || this->m_type == DIR_GRAPH)
		weight = 1;//����ͼ��ȨΪ1
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//��������������ͼ
		if (this->m_pNodeTabel[v1].InsertEdge(v2, weight) &&
			this->m_pNodeTabel[v2].InsertEdge(v1, weight))//��v1Ϊ�������v2Ϊ���㶼ͬʱ����
			this->m_nEdgeNum++;
	}
	else
	{//����ͼ����
		if (this->m_pNodeTabel[v1].InsertEdge(v2, weight))
			this->m_nEdgeNum++;
	}
}
template <class ElemType, class EdgeWType>
// ���������ɾ������Ϊv1��v2�ı�
void AdjListGraph<ElemType, EdgeWType>::DeleteEdge(int v1, int v2)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//���㲻��ȷ������
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//��������������ͼ
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
// ������������ض���v�ĵ�һ���ڽӵ㣬����øñߵ�Ȩ��
int AdjListGraph<ElemType, EdgeWType>::FirstAdjVex(int v, EdgeWType& weight) const
{
	if (v < 0 || v >= this->m_nVexNum)
		return -1;//���㲻��ȷ������ʧ�ܣ��������Ϊ-1��
	
	return 	this->m_pNodeTabel[v].FirstEdge(weight);
}
template <class ElemType, class EdgeWType>
// ������������ض���v1�������v2����һ���ڽӵ㣬����øñ�Ȩ��
int AdjListGraph<ElemType, EdgeWType>::NextAdjVex(int v1, int v2, EdgeWType& weight) const
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return -1;//���㲻��ȷ������
	if (v1 == v2)
		return -1;//v1����Ϊv2
	return 	this->m_pNodeTabel[v1].NextEdge(v2, weight);
}
#endif
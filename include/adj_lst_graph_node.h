#if !defined(_ADJ_LST_GRAPH_NODE_H_)
#define _ADJ_LST_GRAPH_NODE_H_

// �ڽӱ�ͼ��������ģ��
#include "SimpleLkList.h"
template <class EdgeWType>
struct AdjLstGraphEdge
{
	int nVex;//������
	EdgeWType weight;
};

template <class ElemType,class EdgeWType>
class AdjListGraphVexNode
{
public:
	AdjListGraphVexNode();
	~AdjListGraphVexNode();
	bool InsertEdge(int nVex, EdgeWType weight);	//�����
	bool DeleteEdge(int nVex);					//ɾ����
	int FirstEdge(EdgeWType &weight) const;			//��ȡ��һ����
	int NextEdge(int nVexPre, EdgeWType& weight) const; //��ȡǰ��ΪnVexPre����һ����
	ElemType data;							// ����Ԫ��ֵ
	SimpleLkList<AdjLstGraphEdge<EdgeWType>> adjLink;					// �ڽӵ�����
	AdjListGraphVexNode<ElemType, EdgeWType>& operator =(const AdjListGraphVexNode<ElemType, EdgeWType>& source);//���ظ�ֵ�����
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
//��������������
bool AdjListGraphVexNode<ElemType, EdgeWType>::InsertEdge(int nVex, EdgeWType weight)
{
	//�Ȳ�����������Ҫ����Ķ��㣬������ɺ��ٲ����
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	pLst->Head();			//����ǰ�ڵ�ع�ͷ�ڵ�
	AdjLstGraphEdge<EdgeWType> edge;	//����ߵı���
	while (pLst->Next(edge))
	{//ѭ����������Next��������ǰ�ڵ�ָ���̣�����ú�̵�ԭ������
		if (edge.nVex == nVex)
		{//�����Ѿ�����
			return false;		//���Ѿ����ڣ�����ʧ��
		}
	}
	edge.nVex = nVex;			//��ֵ�߶���
	edge.weight = weight;		//��ֵ�ߵ�Ȩ
	pLst->AddTail(edge);		//�ѱ߼��뵽������
	return true;			//���سɹ�
}


template <class ElemType, class EdgeWType>
//���������ɾ����
bool AdjListGraphVexNode<ElemType, EdgeWType>::DeleteEdge(int nVex)
{
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	pLst->Head();
	AdjLstGraphEdge<EdgeWType> edge;
	while (pLst->Next(edge))
	{
		if (edge.nVex == nVex)
		{//�������
			pLst->DeleteCur(edge);//ɾ����ǰ�ڵ�
			return true;
		}
	}
	return false;//������ɶ�û���ҵ�nVex�ߣ�����ʧ��
}
template <class ElemType, class EdgeWType>
//�����������ȡ��һ���ߵĶ���
int AdjListGraphVexNode<ElemType, EdgeWType>::FirstEdge(EdgeWType& weight) const
{
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	if (pLst->IsEmpty())
		return -1;
	pLst->Head();
	AdjLstGraphEdge<EdgeWType> edge;
	if (!pLst->Next(edge))
		return -1;//��ȡ��Ԫ�ڵ�ʧ��
	weight = edge.weight;
	return edge.nVex;
}
template <class ElemType, class EdgeWType>
//�����������ȡǰ��ΪnVexPre����һ����
int AdjListGraphVexNode<ElemType, EdgeWType>::NextEdge(int nVexPre, EdgeWType& weight) const
{
	SimpleLkList<AdjLstGraphEdge<EdgeWType>>* pLst = &adjLink;
	if (pLst->IsEmpty())
		return -1;
	AdjLstGraphEdge<EdgeWType> edge;
	if (pLst->GetCurElem(edge))
	{//�������������NextEdge,�ܴ������һ�εõ��Ķ������nVexPre,��ʱ����Ҫ�����Ϳ���ֱ�ӵõ����
		if (nVexPre == edge.nVex)
		{
			if (!pLst->Next(edge))
				return -1;//��һ�β����Ѿ���β�ڵ㣬����ʧ��
			weight = edge.weight;
			return edge.nVex;
		}
	}
	//���ñ�����������nVexPre
	pLst->Head();
	while (pLst->Next(edge))
	{
		if (edge.nVex == nVexPre)
		{
			if (!pLst->Next(edge))
				return -1;//�Ѿ���β�ڵ㣬����ʧ��
			weight = edge.weight;
			return edge.nVex;

		}
	}
	//û���ҵ�ǰ�ڵ�
	return -1;
}
#endif
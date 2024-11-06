#if !defined(_DOUBLE_CIRCLE_LIST_H_)
#define _DOUBLE_CIRCLE_LIST_H_
#include "node.h"
#include "circle_list_base.h"
template <class ElemType>
class DblCircleList :public CircleListBase< ElemType, DbNode<ElemType>>
{
public:
	DblCircleList();//���캯��
	~DblCircleList();//��������
	DblCircleList(const DblCircleList& source);//�������캯��
	void Tail();//��ǰ�ڵ�ָ��ָ��β�ڵ�
	bool Prev(ElemType& e);//��ǰ�ڵ���Ϊǰ�� 
	bool Reverse();//���������е�Ԫ��λ�÷�ת
protected:
	virtual bool Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode);//���ش��麯��Link	

};

template <class ElemType>
DblCircleList< ElemType>::DblCircleList()
{
	this->LinkTailToHead();//���ø��෽��������β����
}
template <class ElemType>
DblCircleList< ElemType>::~DblCircleList()
{

}
template <class ElemType>
DblCircleList< ElemType>::DblCircleList(const DblCircleList& source)
{
	*this = source;//������������'='�����ø�ֵ���㣬�������
	this->LinkTailToHead();//���ø��෽��������β����
}
template <class ElemType>
bool DblCircleList< ElemType>::Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//ǰ����m_pNextָ��pCurNode
	if (pCurNode)
	{
		pCurNode->m_pPre = pPreNode;//���pCurNode��Ϊ�գ���m_pPre����ǰ���ڵ�
	}
	return true;
}
template <class ElemType>
//�������:��ǰ�ڵ�ָ��ָ��β�ڵ�
void DblCircleList< ElemType>::Tail()
{
	this->m_pCurNode = this->m_pNodeTail;//��ǰ�ڵ�ָ��β�ڵ�
}
template <class ElemType>
//�������:��ǰ�ڵ���Ϊǰ��,ͬʱ��ȡǰ���Ĵ���ֵ
bool DblCircleList< ElemType>::Prev(ElemType& e)
{
	if (this->IsEmpty())
		return false;//����Ϊ�գ����ܻ�ȡ��ǰ�ڵ��ǰ��
	if (!this->m_pNodeCur || !this->m_pNodeCur->m_pPre)
		return false;//��ǰ�ڵ�Ϊ�ջ�ǰ�ڵ��ǰ��Ϊ�գ������ܼ���ǰ�Ƶ�ǰ�ڵ�
	this->m_pNodeCur = this->m_pNodeCur->m_pPre;//��ǰ�ڵ���Ϊǰ��
	//��������Ľڵ�Ϊͷ�ڵ㣬�������Ϊǰ��
	if (this->m_pNodeCur == this->m_pNodeHead)
		this->m_pNodeCur = this->m_pNodeCur->m_pPre;
	e = this->m_pNodeCur->m_TData;
	return true;
}

template <class ElemType>
//�������:���������е�Ԫ��λ�÷�ת
bool DblCircleList< ElemType>::Reverse()
{
	if (this->IsEmpty())
		return false;//�������÷�ת
	DbNode<ElemType>* pFirstNode = this->GetFirstNode();//��ȡ��Ԫ�ڵ�
	DbNode<ElemType>* pNode = this->m_pNodeHead;//��ͷ�ڵ㿪ʼ
	//��ͷ�ڵ㿪ʼ��ת���ڵ��ǰ���ͺ�̻�������������
	do
	{
		DbNode<ElemType>* pNext = pNode->m_pNext;//�ݴ�pNode�ĺ��
		//����pNode��ǰ���ͺ��
		pNode->m_pNext = pNode->m_pPre;
		pNode->m_pPre = pNext;
		pNode = pNext;//���Ϊ���
	} while (pNode !=this->m_pNodeHead);
	//��ת��β�ڵ�Ϊ��תǰ����Ԫ�ڵ�
	this->m_pNodeTail = pFirstNode;
	//Ҳ�����ǣ�this->m_pNodeTail = this->m_pNodeHead->m_pPre;
	
}
#endif
 

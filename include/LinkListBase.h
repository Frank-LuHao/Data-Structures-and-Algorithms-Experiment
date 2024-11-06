#if !defined(_LINK_LIST_BASE_H_)
#define  _LINK_LIST_BASE_H_
#include "ListBase.h"
#include "node.h"

template <class ElemType, class NodeType>  
class LinkListBase:public ListBase<ElemType>
{
public:
	LinkListBase();//���캯��
	virtual ~LinkListBase();//��������
	virtual int Length() const;//��ȡ���Ա���Ԫ�ظ���
	virtual bool IsEmpty() const;//�ж����Ա��Ƿ�Ϊ��
	virtual void Clear();//������Ա�
	virtual bool GetElem(int position, ElemType& e) const;//��ȡָ��λ��Ԫ��
	virtual bool SetElem(int position, const ElemType e);//����ָ��λ��Ԫ��
	virtual bool Delete(int position, ElemType& e);//ɾ��ָ��λ��Ԫ��
	virtual bool Insert(int position, const ElemType e) ;//��ָ��λ�ò���Ԫ��
	virtual void Traverse(bool (*visit)(const ElemType&))const ;//�������Ա�
	virtual int AddTail(const ElemType e);	//������ݵ����Ա�ĩβ
	virtual LinkListBase& operator = (const LinkListBase<ElemType, NodeType>& source); // ���ظ�ֵ�����
	virtual void Head();//��ǰ�ڵ�ָ��ͷ�ڵ�
	virtual bool Next(ElemType& e);//��ǰ�ڵ����,ͬʱ��ȡ�������
	virtual bool DeleteCur(ElemType& e);//ɾ����ǰ�ڵ�Ԫ��
protected:
	virtual bool Link(NodeType* pPreNode,NodeType* pNextNode) =0;//���������ڵ�
	virtual NodeType* FindNode(int position, NodeType*&  pPreNode) const;//����λ�ú�Ѱ�ҽڵ�
	virtual NodeType* GetFirstNode() const;//��ȡ��Ԫ���
	virtual bool IsEndWhile(NodeType* pNode) const;//�Ƿ����ѭ�����
	NodeType* m_pNodeHead;//ͷ�ڵ�ָ��
	NodeType* m_pNodeTail;//β�ڵ�ָ��
	NodeType* m_pNodeCur;//��ǰ�ڵ�ָ��
	int m_nNodeCounts;//��ǰ�ڵ���
	bool m_bSpecialDetrcutiom;//����������ʽ��������
	
};
//LinkListBaseʵ��
template <class ElemType,class NodeType>
LinkListBase<ElemType, NodeType>::LinkListBase()
{
	m_bSpecialDetrcutiom = false;
	m_pNodeHead = new NodeType;
	m_nNodeCounts = 0;
	m_pNodeTail = m_pNodeHead;//��ʼ��ʱͷ�ڵ����β�ڵ�
	m_pNodeCur = m_pNodeHead;//��ǰ�ڵ�Ϊͷ�ڵ�
}

template <class ElemType, class NodeType>
LinkListBase<ElemType, NodeType>& LinkListBase<ElemType, NodeType>::operator = (const LinkListBase<ElemType, NodeType>& source)
{

	Clear();//��սڵ�
	m_bSpecialDetrcutiom = false;
	NodeType* pSrcNodeHead = source.m_pNodeHead;//��ȡԴ����ͷ�ڵ�
	NodeType* pSrcNode = source.GetFirstNode();//pSrcNodeΪsource����Ԫ���
	NodeType* pPreNode = m_pNodeHead;
	while (!source.IsEndWhile(pSrcNode))
	{
		NodeType* pNewNode = new NodeType;
		pNewNode->m_TData = pSrcNode->m_TData;//�½ڵ㸳ֵ
		Link(pPreNode, pNewNode);//����ǰ�ڵ���½ڵ�
		pSrcNode = pSrcNode->m_pNext;//Դ����ĵ�ǰ�ڵ����
		pPreNode = pNewNode;//ǰ�ڵ���Ϊ�½ڵ�
		m_nNodeCounts++;
	}
	m_pNodeTail = pPreNode;
	m_pNodeCur = m_pNodeHead;
	return *this;
}

template <class ElemType, class NodeType>
LinkListBase<ElemType, NodeType>::~LinkListBase()
{
	if (!m_bSpecialDetrcutiom)
	{

		Clear();//��սڵ�
		if (m_pNodeHead)
		{
			delete m_pNodeHead;
			m_pNodeHead = NULL;
		}
	}
}


template <class ElemType, class NodeType>
int LinkListBase<ElemType, NodeType>::Length() const
{
	int nNodeCounts = 0;//�ڵ���Ŀ������

/*
	���û��m_nNodeCounts��Ϊ�ڵ���Ŀ����������Ҫ�����ڵ�
	NodeType* pNode = GetFirstNode();
	while (!IsEndWhile(pNode))
	{
		nNodeCounts++;
		pNode = pNode->m_pNext;
	}
*/
	nNodeCounts = m_nNodeCounts;
	return nNodeCounts;
}

template <class ElemType, class NodeType>
NodeType* LinkListBase<ElemType, NodeType>::GetFirstNode() const
{
	NodeType* pNode = m_pNodeHead->m_pNext;
	return pNode;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::IsEndWhile(NodeType* pNode) const
{
	if (!pNode || pNode == m_pNodeHead)
		return true;
	return false;
}
template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Head()
{
	m_pNodeCur = m_pNodeHead;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Next(ElemType &e)
{
	if (!m_pNodeCur || !m_pNodeCur->m_pNext)
		return false;//��ǰ�ڵ�Ϊ�ջ�ǰ�ڵ�ĺ��Ϊ�գ������ܼ������Ƶ�ǰ�ڵ�
	m_pNodeCur = m_pNodeCur->m_pNext;
	e = m_pNodeCur->m_TData;
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::DeleteCur(ElemType& e)
{
	if (!m_pNodeCur || m_pNodeCur == m_pNodeHead)
		return false;//��ǰ�ڵ�Ϊ�գ���ǰ�ڵ�Ϊͷ�ڵ㣬������ɾ��
	NodeType* pNextNode = m_pNodeCur->m_pNext;//��̿���ֱ�ӵõ�
	NodeType* pPreNode = m_pNodeHead;//ǰ����ʼ��Ϊͷ�ڵ�
	NodeType* pNode = GetFirstNode();
	bool bFindPre = false;
	while (!IsEndWhile(pNode))
	{
		if (pNode == m_pNodeCur)
		{
			bFindPre = true;
			break;//�ҵ�ǰ�����˳�ѭ��
		}
		pNode = pNode->m_pNext;
		pPreNode = pPreNode->m_pNext;
	}
	if (!bFindPre)
		return false;
	Link(pPreNode, pNode->m_pNext);//����ǰ���ͺ�̽ڵ�
	e = pNode->m_TData;

	if (pNode == m_pNodeTail)
	{//���ɾ������β�ڵ�
		m_pNodeTail = pPreNode; //���β�ڵ�Ϊɾ���ڵ��ǰ��
	}
	m_pNodeCur = pPreNode;//��ǰ�ڵ���Ϊǰ��
	delete pNode;//ɾ���ڵ�
	m_nNodeCounts--;//�ܽڵ�����һ
	return true;
}
template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Clear()
{
	if (!m_pNodeHead)
		return;
	NodeType* pNode = GetFirstNode();
	//�����ѭ����սڵ�
	while (!IsEndWhile(pNode))
	{
		NodeType* pNodeNext = pNode->m_pNext;
		delete pNode;
		pNode = pNodeNext;
	}
	m_pNodeHead->ClearLink();//���ͷ�ڵ������״̬
	m_pNodeTail = m_pNodeHead;//��պ�ͷ�ڵ�Ϊβ�ڵ�
	m_pNodeCur = m_pNodeHead;//��ǰ�ڵ�Ϊͷ�ڵ�
	m_nNodeCounts = 0;//�ڵ���ĿΪ0
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::IsEmpty() const
{
	if (m_pNodeHead->m_pNext)
		return false;
	return true;
}
template <class ElemType, class NodeType>
NodeType* LinkListBase<ElemType, NodeType>::FindNode(int position, NodeType*& pPreNode) const
{
	if (position < 0)
		return NULL;
	NodeType* pNode = GetFirstNode();//��ȡ��Ԫ���
	pPreNode = m_pNodeHead;//��Ԫ����ǰ����ͷ�ڵ�
	NodeType* pFindNode = NULL;//�����ҵ��Ľڵ㣬��ʼֵΪ��
	int nIndex = 0;//����ڵ�λ�úű�������ʼֵΪ0
	while (!IsEndWhile(pNode))
	{
		if (nIndex == position)
		{//��ǰλ�ú�Ϊ����λ�ú�
			pFindNode = pNode;//�ҵ��ڵ�
			break;//�˳�ѭ��
		}
		pPreNode = pNode;//ǰ�����Ϊ��ǰ�ڵ�
		pNode = pNode->m_pNext;//��ǰ�ڵ���Ϊ����
		nIndex++;//λ�úű�����1
	}
	return pFindNode;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::SetElem(int position, const ElemType e) 
{
	NodeType* pPreNode;
	NodeType* pNode = FindNode(position, pPreNode);//����λ�úŲ��ҽڵ�
	if (!pNode)
		return false;//û���ҵ��ڵ㣬����false;
	pNode->m_TData = e;
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::GetElem(int position, ElemType& e) const
{
	NodeType* pPreNode;
	NodeType* pNode =  FindNode(position, pPreNode);//����λ�úŲ��ҽڵ�
	if (!pNode)
		return false;//û���ҵ��ڵ㣬����false;
	e = pNode->m_TData;
	return true;
}

template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Delete(int position, ElemType& e)
{
	NodeType* pPreNode;
	NodeType* pNode = FindNode(position, pPreNode);//����λ�úŲ��ҽڵ�
	if (!pNode)
		return false;//û���ҵ��ڵ㣬����false;
	
	Link(pPreNode, pNode->m_pNext);//����ǰ���ͺ�̽ڵ�
	e = pNode->m_TData;
	
	if (pNode == m_pNodeTail)
	{//���ɾ������β�ڵ�
		m_pNodeTail = pPreNode; //���β�ڵ�Ϊɾ���ڵ��ǰ��
	}
	m_pNodeCur = pPreNode;//��ǰ�ڵ���Ϊǰ��
	delete pNode;//ɾ���ڵ�
	m_nNodeCounts--;//�ܽڵ�����һ
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Insert(int position, const ElemType e)
{
	NodeType* pPreNode;//������λ�õ�ǰ���ڵ�ָ��
	NodeType* pNode = FindNode(position, pPreNode);//����λ�úŲ��ҽڵ�
	if (!pNode)
	{ //û���ҵ��ڵ�
		if (position == m_nNodeCounts)//β�������ж�
			pPreNode = m_pNodeTail;//β������ʱ��ǰ��Ϊβ�ڵ�
		else
			return false;//λ�úŴ��󣬷���false
	}
	NodeType* pNewNode = new NodeType;//�����½ڵ�
	pNewNode->m_TData = e;//���½ڵ�����ݸ�ֵ
	Link(pPreNode, pNewNode);//����ǰ�ڵ���½ڵ�
	Link(pNewNode, pNode);//�����½ڵ�Ͳ���λ�õĽڵ�
	if (position == m_nNodeCounts)//β�������ж�
		m_pNodeTail = pNewNode;//β������ʱ��β�ڵ�Ϊ�½ڵ�
	m_pNodeCur = pNewNode;//�����ǰ�ڵ�Ϊ�½ڵ�
	m_nNodeCounts++;
	return true;
}


template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Traverse(bool (*visit)(const ElemType&))const
{
	if (!visit)
		return;
	NodeType* pNode =GetFirstNode();
	while (!IsEndWhile(pNode))
	{
		if (!(*visit)(pNode->m_TData))
			break;//����ָ����ʺ�������Ϊfalse�˳�ѭ��
		pNode = pNode->m_pNext;
	}
}
template <class ElemType, class NodeType>
int LinkListBase<ElemType, NodeType>::AddTail(const ElemType e)
{
	NodeType* pNewNode = new NodeType;
	pNewNode->m_TData = e;//���½ڵ�����ݸ�ֵ
	Link(m_pNodeTail, pNewNode);//����β�ڵ���½ڵ�
	m_pNodeTail = pNewNode;//β�ڵ���Ϊ�½ڵ�
	m_pNodeCur = pNewNode;//�����ǰ�ڵ�Ϊ�½ڵ�
	m_nNodeCounts++;//�ڵ���Ŀ��һ
	return m_nNodeCounts - 1;// �����¼ӽڵ��λ�ú�
}

#endif

#if !defined(_REF_GEN_LIST_H_)
#define _REF_GEN_LIST_H_
#include "ref_gen_node.h"
#include "LinkListBase.h"
template <class ElemType>
class RefGenList :private LinkListBase< ElemType, RefGenListNode< ElemType>>
{
public:
	RefGenList();//�޲������캯��
	RefGenList(const RefGenList<ElemType>& source);	// ���ƹ��캯��ģ��	
	~RefGenList();//��������
	RefGenListNode<ElemType>* First() const;	// ����ָ���������������ĵ�һ��Ԫ�ص�ָ��
	bool IsEmpty() const;//�Ƿ�Ϊ�չ����
	void Push(const ElemType& e);				// ��ԭ��Ԫ��e��Ϊ��ͷ���뵽���������������ǰ��
	void Push(RefGenList<ElemType>& subList);// ���ӱ�subList��Ϊ��ͷ���뵽���������������ǰ��
	void Show() const;							// ��ʾ�����	
	int Depth() const;							//���������
	void Input();								// ��������	
	RefGenListNode<ElemType>* Next(RefGenListNode<ElemType>* elemPtr) const;// ����ָ��elemPtrָ����������������Ԫ�صĺ�̵�ָ��
	RefGenList<ElemType>& operator =(const RefGenList<ElemType>& source);// ���ظ�ֵ�����
private:
	virtual bool Link(RefGenListNode< ElemType>* pPreNode, RefGenListNode< ElemType>* pNextNode);//���ش��麯��
	void ClearAux(RefGenListNode< ElemType>* &pHeadNode);//�������������
	void CopyAux(const RefGenListNode<ElemType>* sourceHead, RefGenListNode<ElemType>*& targetHead);
	void ShowAux(RefGenListNode<ElemType>* pHeader) const;
	int DepthAux(const RefGenListNode<ElemType>* pHeader) const;// ������pHeaderΪ��ͷ�������������������
	void CreateAux(RefGenListNode<ElemType>*& first);// ������firstΪ��Ԫ�ؽ����������������
};
template <class ElemType>
//����չ����
RefGenList<ElemType>::RefGenList()
{
	this->m_pNodeHead->m_tag = _HEAD;//ͷ�ڵ�����Ϊ_HEAD
	this->m_pNodeHead->m_nRef = 1;//������Ϊ1
	//����������ʽ�������������ʱ�������еĽڵ㲻һ������
	this->m_bSpecialDetrcutiom = true;
}
template <class ElemType>
// ���ƹ��캯��ģ��
RefGenList<ElemType>::RefGenList(const RefGenList<ElemType>& source)
{
	this->m_bSpecialDetrcutiom = true;
	CopyAux(source.m_pNodeHead, this->m_pNodeHead);
}
template <class ElemType>
// ������pHeaderΪ��ͷ�������������������
int RefGenList<ElemType>::DepthAux(const RefGenListNode<ElemType>* pHeader) const
{
	if (pHeader->m_pNext == NULL) return 1;	// �ձ����������Ϊ1
	int subMaxDepth = 0;			// �ӱ�������
	for (RefGenListNode<ElemType>* temPtr = pHeader->m_pNext; temPtr != NULL;
		temPtr = temPtr->m_pNext)//������ڵ�
	{// ���ӱ��������
		if (temPtr->m_tag == _LIST)
		{// �ڵ����ӱ�
			int curSubDepth = DepthAux(temPtr->m_pSubLink);	// ���ӱ���ȣ��ݹ����
			if (subMaxDepth < curSubDepth) subMaxDepth = curSubDepth;
		}
	}
	return subMaxDepth + 1;		// ����������������Ϊ�ӱ������ȼ�1
}

template <class ElemType>
int RefGenList<ElemType>::Depth() const
// �����������������������������
{
	return DepthAux(this->m_pNodeHead);
}
template <class ElemType>
//��ʾ�����������
void RefGenList<ElemType>::ShowAux(RefGenListNode<ElemType>* pHeader) const
{
	bool frist = true;
	cout << "(";								// ���������������(��ʼ
	for (RefGenListNode<ElemType>* temPtr = pHeader->m_pNext; temPtr != NULL; temPtr = temPtr->m_pNext)
	{	// ���δ�����������������Ԫ�� 
		if (frist) frist = false;				// ��һ��Ԫ��
		else cout << ",";						// ��ͬԪ������ö��Ÿ���
		if (temPtr->m_tag == _ATOM)
		{	// ԭ�ӽ��
			cout << temPtr->m_TData;
		}
		else
		{	// ����
			ShowAux(temPtr->m_pSubLink);
		}
	}
	cout << ")";								// ���������������)����

}
template<class ElemType>
RefGenList<ElemType>& RefGenList<ElemType>::operator =(const RefGenList<ElemType>& source)
// ������������������������source��ֵ����ǰ������������������ظ�ֵ�����
{
	if (&source != this)
	{
		ClearAux(this->m_pNodeHead);						// ��յ�ǰ�������������
		CopyAux(source.m_pNodeHead, this->m_pNodeHead);			// �����������������
	}
	return *this;
}

// ��ʾ�����	
template <class ElemType>
void RefGenList<ElemType>::Show() const
{
	ShowAux(this->m_pNodeHead);
}
template <class ElemType>
RefGenList<ElemType>::~RefGenList()
{
	ClearAux(this->m_pNodeHead);
}
template <class ElemType>
void RefGenList<ElemType>::CopyAux(const RefGenListNode<ElemType>* sourceHead,
	RefGenListNode<ElemType>*& targetHead)
	// ��ʼ����: ��sourceHeadΪͷ�����������������Ϊ�ǵݹ��������������
	// �������: ����sourceHeadΪͷ������������������Ƴ���targetHeadΪͷ������������
	//	�����
{
	targetHead = new RefGenListNode<ElemType>(_HEAD);		// ����ͷ���
	RefGenListNode<ElemType>* targetPtr = targetHead;		// targetHead�ĵ�ǰ���
	targetHead->m_nRef = 1;									// ������Ϊ1
	for (RefGenListNode<ElemType>* temPtr = sourceHead->m_pNext; temPtr != NULL;
		temPtr = temPtr->m_pNext)
	{	// ɨ���������������sourceHead�Ķ���
		targetPtr = targetPtr->m_pNext = new RefGenListNode<ElemType>(temPtr->tag);
		// �����½��
		if (temPtr->m_tag == _LIST)
		{	// �ӱ�
			CopyAux(temPtr->m_pSubLink, targetPtr->m_pSubLink);// �����ӱ�
		}
		else
		{	// ԭ�ӽ��
			targetPtr->m_TData = temPtr->m_TData;		// ����ԭ�ӽ��
		}
	}
}
template <class ElemType>
RefGenListNode<ElemType>* RefGenList<ElemType>::Next(RefGenListNode<ElemType>* elemPtr) const
// �������������ָ��elemPtrָ����������������Ԫ�صĺ�̵�ָ��
{
	return elemPtr->nextLink;
}
template <class ElemType>
bool RefGenList<ElemType>::Link(RefGenListNode< ElemType>* pPreNode, RefGenListNode< ElemType>* pNextNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pNextNode;
	return true;
}

template <class ElemType>
RefGenListNode<ElemType>* RefGenList<ElemType>::First() const
// �������������ָ���������������ĵ�һ��Ԫ�ص�ָ��
{
	return this->m_pNodeHead->m_pNext;
}

template <class ElemType>
// ��ԭ��Ԫ��e��Ϊ��ͷ���뵽���������������ǰ��
void RefGenList<ElemType>::Push(const ElemType& e)
{
	if (!this->Insert(0, e))
		return;
	this->m_pNodeCur->m_tag = _ATOM;//��ǰ�����Ԫ������Ϊԭ������:_ATOM
	
}
template <class ElemType>
// ���ӱ�subList��Ϊ��ͷ���뵽���������������ǰ��
void RefGenList<ElemType>::Push(RefGenList<ElemType>& subList)
{
	if (!this->Insert(0, 0))
	{//����һ��Ԫ��ʧ��
		return;
	}
	RefGenListNode<ElemType>* pCurNodePtr = this->m_pNodeCur;//�ղŲ����Ԫ��
	pCurNodePtr->m_pSubLink = subList.m_pNodeHead;				// �ӱ�
	subList.m_pNodeHead->m_nRef++;						// subList�������Լ�1
	pCurNodePtr->m_tag = _LIST;//��ǰ�����Ԫ������Ϊ�ӱ�_LIST
}
template <class ElemType>
//��չ����������
void RefGenList<ElemType>::ClearAux(RefGenListNode< ElemType>*& pHeadNode)
{
	if (!pHeadNode || pHeadNode->m_tag != _HEAD)
		return;//ͷ�ڵ����Ͳ��ǹ�������ʼ�ڵ�
	pHeadNode->m_nRef--;//�������Լ�1
	if (pHeadNode->m_nRef == 0)
	{//�������������Ϊ0ʱ��������ձ��еĽڵ�
		RefGenListNode<ElemType>* pNode = pHeadNode->m_pNext;//��ȡ��Ԫ���
		while (pNode)
		{//ѭ����սڵ�
			RefGenListNode<ElemType>* pNodeNext = pNode->m_pNext;//��ȡ��ɾ���ڵ�ĺ��
			if (pNode->m_tag == _LIST)//�жϽڵ������Ƿ�Ϊ:_LIST
				ClearAux(pNode->m_pSubLink);//����ӱ�,�ݹ����
			delete pNode;//ɾ���ڵ�
			pNode = pNodeNext;//pNodeָ����
		}
		delete pHeadNode;//�ͷŵ�ͷ�ڵ�
		pHeadNode = NULL;
	}
}


template<class ElemType>
void RefGenList<ElemType>::CreateAux(RefGenListNode<ElemType>*& first)
// ���������������firstΪ��Ԫ�ؽ����������������
{
	char ch;									// �ַ�����
	cin >> ch;									// �����ַ�
	switch (ch)
	{
	case ')':									// ������������������
		return;									// ����
	case '(':									// �ӱ�
		// ��ͷΪ�ӱ�
		first = new RefGenListNode<ElemType>(_LIST);// ���ɱ���

		RefGenListNode<ElemType>* pSubHead;		// �ӱ�ָ��
		pSubHead = new RefGenListNode<ElemType>(_HEAD);// �����ӱ��ͷ���
		pSubHead->m_nRef = 1;						// ������Ϊ1	
		first->m_pSubLink = pSubHead;				// subHeadΪ�ӱ�
		CreateAux(pSubHead->m_pNext);			// �ݹ齨���ӱ�

		cin >> ch;								// ����','
		if (ch != ',') cin.putback(ch);			// �粻��','����ch���˵�������
		CreateAux(first->m_pNext);			// �������������������һ���
		break;
	default:									// ԭ��
		// ��ͷΪԭ��
		cin.putback(ch);						// ��ch���˵�������
		ElemType amData;						// ԭ�ӽ������
		cin >> amData;							// ����ԭ�ӽ������
		first = new RefGenListNode<ElemType>(_ATOM);// ����ԭ����
		first->m_TData = amData;					// ԭ�ӽ������

		cin >> ch;								// ����','
		if (ch != ',') cin.putback(ch);			// �粻��','����ch���˵�������
		CreateAux(first->m_pNext);			// �������������������һ���
		break;
	}
}
template<class ElemType>
bool RefGenList<ElemType>::IsEmpty() const
{
	return LinkListBase< ElemType, RefGenListNode< ElemType>>::IsEmpty();//���ø��෽��
}
template<class ElemType>
void RefGenList<ElemType>::Input()
// �����������������
{
	if (this->m_pNodeHead)
	{
		ClearAux(this->m_pNodeHead);
	}
	char ch;									// �ַ�����
	this->m_pNodeHead = new RefGenListNode<ElemType>(_HEAD);	// �����������������ͷ���
	this->m_pNodeHead->m_nRef = 1;								// ������Ϊ1

	cin >> ch;									// �����һ��'('
	RefGenList<ElemType>::CreateAux(this->m_pNodeHead ->m_pNext);
	// ������m_pNodeHead ->m_pNextΪ��ͷ���������������
}
#endif
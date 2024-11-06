#if !defined(_LK_BINARY_TREE_BASE_H_)
#define _LK_BINARY_TREE_BASE_H_
#include "binary_tree_base.h"
template <class ElemType,class NodeType>
class LkBinaryTreeBase :public BinaryTreeBase<ElemType, NodeType*>
{
public:
	LkBinaryTreeBase();
	~LkBinaryTreeBase();
	LkBinaryTreeBase(LkBinaryTreeBase< ElemType, NodeType>& source);//���ƹ��캯��
	LkBinaryTreeBase(NodeType* r);					// ������rΪ���Ķ�����
	LkBinaryTreeBase(const ElemType& e);							// ������eΪ���Ķ�����
	virtual NodeType* GetRoot() const;						// ���ض������ĸ�
	virtual bool NodeEmpty(NodeType* pCur) const;				// �жϽ��cur�Ƿ�Ϊ��
	virtual bool GetItem(NodeType* pCur, ElemType& e) const;	// ���ؽ��cur��Ԫ��ֵ
	virtual bool SetElem(NodeType* pCur, const ElemType& e) ;	// �����cur��ֵ��Ϊe
	virtual bool Empty() const;								// �ж϶������Ƿ�Ϊ��
	virtual NodeType* LeftChild(NodeType* pCur) const;		// ���ض������н��cur������
	virtual NodeType* RightChild(NodeType* pCur) const;	// ���ض������н��cur���Һ���
	virtual NodeType* Parent(NodeType* pCur) const = 0;		// ���ض������н��cur��˫��
	virtual bool CreateBinaryTree(ElemType pre[], ElemType in[], int n);//���������������д���������
	LkBinaryTreeBase<ElemType, NodeType>& operator=(const LkBinaryTreeBase<ElemType, NodeType>& source);	// ���ظ�ֵ�����
protected:
	virtual ElemType NodeElem(NodeType* pCur) const ;		// ����ָ���ڵ��Ԫ��ֵ
	virtual void ReleaseNode(NodeType* &pCur);					// �����cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
	virtual NodeType* CreateChildNode(NodeType* pCur, bool bLeft);//����cur�ڵ���ӽڵ�
	virtual void LinkParentChild(NodeType* pParent, NodeType* pChild, bool bLeft) = 0;//����˫�׽ڵ�ͺ��ӽڵ�
	NodeType* CopyTreeAux(const NodeType* r);// ���ƶ�����
	NodeType* m_pRoot;//���ڵ�

};

template <class ElemType,class NodeType>
//����������޲������캯��
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase()
{
	m_pRoot = new NodeType;
}
template <class ElemType,class NodeType>
//�����������������
LkBinaryTreeBase<ElemType, NodeType>::~LkBinaryTreeBase()
{
	if (m_pRoot)
	{//���ڸ��ڵ�
		this->DestroyAux(m_pRoot);//���������ͷ��ڴ���Դ
		m_pRoot = NULL;
	}
}
template <class ElemType, class NodeType>
//������������ƹ���
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase(LkBinaryTreeBase< ElemType, NodeType>& source)
{
	*this = source;//����������'=',����ֱ�Ӹ�ֵ
}
template <class ElemType, class NodeType>
//���������������rΪ���Ķ�����
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase(NodeType* r)
{
	m_pRoot = CopyTreeAux(r);
}
template <class ElemType, class NodeType>
//���������������eΪ���Ķ�����
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase(const ElemType& e)
{
	m_pRoot = new NodeType;
	SetElem(m_pRoot, e);
}

template <class ElemType, class NodeType>
//������������ظ��ڵ�
NodeType* LkBinaryTreeBase<ElemType, NodeType>::GetRoot() const
{
	return m_pRoot;
}
template <class ElemType, class NodeType>
//����������жϽڵ��Ƿ�Ϊ��
bool LkBinaryTreeBase<ElemType, NodeType>::NodeEmpty(NodeType* pCur) const
{
	if (!pCur)
		return true;
	return false;
}
template <class ElemType, class NodeType>
//�����������ȡ�ڵ�����
bool LkBinaryTreeBase<ElemType, NodeType>::GetItem(NodeType* pCur, ElemType& e) const
{
	if (!pCur)
		return false;
	e = NodeElem(pCur);
	return true;
}
template <class ElemType, class NodeType>
//����������ж��Ƿ���Ϊ��
bool LkBinaryTreeBase<ElemType, NodeType>::Empty() const
{
	if (!m_pRoot)
		return true;//���ڵ�Ϊ�գ���Ϊ��
	if (!LeftChild(m_pRoot) && !RightChild(m_pRoot))
		return true;//���ڵ�û�����Ӻ��Һ��ӣ���Ϊ��
	return false;
}
template <class ElemType, class NodeType>
//�����������ȡpCur�ڵ������
NodeType* LkBinaryTreeBase<ElemType, NodeType>::LeftChild(NodeType* pCur) const
{
	return pCur->leftChild;//��������
}
template <class ElemType, class NodeType>
//�����������ȡpCur�ڵ���Һ���
NodeType* LkBinaryTreeBase<ElemType, NodeType>::RightChild(NodeType* pCur) const
{
	return pCur->rightChild;//�����Һ���
}
template <class ElemType, class NodeType>
//�������������pCur�ڵ������
bool LkBinaryTreeBase<ElemType, NodeType>::SetElem(NodeType* pCur, const ElemType& e)
{
	if (pCur)
		return false;
	pCur->data = e;
	return true;
}
template <class ElemType, class NodeType>
//�����������ȡpCur�ڵ������
ElemType LkBinaryTreeBase<ElemType, NodeType>::NodeElem(NodeType* pCur) const
{
	if (!pCur)
		return 0;
	return pCur->data;
}
template <class ElemType, class NodeType>
//����������ͷ�pCur�ڵ���ڴ���Դ
void LkBinaryTreeBase<ElemType, NodeType>::ReleaseNode(NodeType*& pCur)
{
	if (!pCur)
		return;
	delete pCur;
	pCur = NULL;
}
template <class ElemType, class NodeType>
//�������������pCur�ڵ���ӽڵ�
NodeType* LkBinaryTreeBase<ElemType, NodeType>::CreateChildNode(NodeType* pCur, bool bLeft)
{
	if (!pCur)
	{//�Ǹ��ڵ�
		NodeType* pNode = new NodeType;
		return pNode;
	}
	if (LeftChild(pCur) && bLeft)
		return NULL;
	else if (RightChild(pCur) && !bLeft)
		return NULL;
	NodeType* pNode = new NodeType;
	LinkParentChild(pCur, pNode, bLeft);//���ӵ�ǰ�ڵ�ͺ��ӽڵ�
	return pNode;
}
template <class ElemType, class NodeType>
//������������ƶ�����
NodeType* LkBinaryTreeBase<ElemType, NodeType>::CopyTreeAux(const NodeType* r)
{
	if (r == NULL)
	{	// ���ƿն�����
		return NULL;					// �ն�������Ϊ��	
	}
	else
	{	// ���Ʒǿն�����
		NodeType* pLChild = CopyTreeAux(LeftChild(r));	// ����������
		NodeType* pRChild = CopyTreeAux(RightChild(r));	// ����������
		NodeType* pParent = new NodeType;
		SetElem(pParent, r->data);//��������
		LinkParentChild(pParent, pLChild, true);//����˫�׽ڵ�����ӽڵ�
		LinkParentChild(pParent, pRChild, false);//����˫�׽ڵ���Һ��ӽڵ�
		return pParent;
	}
}
template <class ElemType, class NodeType>
//������������ظ�ֵ�����
LkBinaryTreeBase<ElemType, NodeType>& LkBinaryTreeBase<ElemType, NodeType>::operator=(const LkBinaryTreeBase<ElemType, NodeType>& source)
{
	if (&source != this)
	{
		DestroyAux(m_pRoot);//����������
		m_pRoot = CopyTreeAux(source.GetRoot());
	}
	return *this;
}
template <class ElemType, class NodeType>
//�������,���������������д�����ʽ�洢������
bool LkBinaryTreeBase<ElemType, NodeType>::CreateBinaryTree(ElemType pre[], ElemType in[], int n)
{
	if (!pre || !in || n < 1)
		return false;
	this->DestroyAux(m_pRoot);//����������
	m_pRoot = this->CreateBinaryTreeAux(NULL, pre, in, 0, n - 1, 0, n - 1,false);//�������������������ɶ�����
	return true;
}
#endif
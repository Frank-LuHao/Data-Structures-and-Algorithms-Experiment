#if !defined(_BINARY_TREE_H_)
#define _BINARY_TREE_H_
#include "lk_binary_tree_base.h"
#include "bin_tree_node.h"
template <class ElemType>
class BinaryTree :public LkBinaryTreeBase <ElemType, BinTreeNode<ElemType>>
{
public:
	BinaryTree();
	~BinaryTree();
	virtual BinTreeNode<ElemType>* Parent(BinTreeNode<ElemType>* pCur) const;		// ���ض������н��cur��˫��
protected:
	virtual void LinkParentChild(BinTreeNode<ElemType>* pParent, BinTreeNode<ElemType>* pChild, bool bLeft);
	BinTreeNode<ElemType>* ParentAux(BinTreeNode<ElemType>* r,BinTreeNode<ElemType>* cur) const;			// ����cur��˫��
};
template <class ElemType>
BinaryTree<ElemType>::BinaryTree()
{
}
template <class ElemType>
BinaryTree<ElemType>::~BinaryTree()
{
}
template <class ElemType>
BinTreeNode<ElemType>* BinaryTree<ElemType>::ParentAux(BinTreeNode<ElemType>* r, BinTreeNode<ElemType>* cur) const
// ���������������rΪ���Ķ�����, ���cur��˫��
{
	if (r == NULL) return NULL;		// �ն�����
	else if (r->leftChild == cur || r->rightChild == cur) return r; // rΪcur��˫��
	else
	{	// ����������˫��
		BinTreeNode<ElemType>* temPtr;	// ��ʱָ��

		temPtr = ParentAux(r->leftChild, cur);	// ������������cur��˫��	
		if (temPtr != NULL) return temPtr;		// ˫������������

		temPtr = ParentAux(r->rightChild, cur);// ������������cur��˫��	
		if (temPtr != NULL) return temPtr;		// ˫������������
		else return NULL;						// ��ʾcur��˫��
	}
}
template <class ElemType>
//�������������pCur��˫�׽ڵ�
BinTreeNode<ElemType>* BinaryTree<ElemType>::Parent(BinTreeNode<ElemType>* pCur) const
{
	return ParentAux(this->GetRoot(), pCur);
}
template <class ElemType>
//�������������˫�׽ڵ�ͺ��ӽڵ�
void BinaryTree<ElemType>::LinkParentChild(BinTreeNode<ElemType>* pParent, BinTreeNode<ElemType>* pChild, bool bLeft)
{
	if (!pParent)
		return;
	if (bLeft)
		pParent->leftChild = pChild;
	else
		pParent->rightChild = pChild;
}

#endif // !defined(_BINARY_TREE_H_)

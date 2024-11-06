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
	virtual BinTreeNode<ElemType>* Parent(BinTreeNode<ElemType>* pCur) const;		// 返回二叉树中结点cur的双亲
protected:
	virtual void LinkParentChild(BinTreeNode<ElemType>* pParent, BinTreeNode<ElemType>* pChild, bool bLeft);
	BinTreeNode<ElemType>* ParentAux(BinTreeNode<ElemType>* r,BinTreeNode<ElemType>* cur) const;			// 返回cur的双亲
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
// 操作结果：返回以r为根的二叉树, 结点cur的双亲
{
	if (r == NULL) return NULL;		// 空二叉树
	else if (r->leftChild == cur || r->rightChild == cur) return r; // r为cur的双亲
	else
	{	// 在子树上求双亲
		BinTreeNode<ElemType>* temPtr;	// 临时指针

		temPtr = ParentAux(r->leftChild, cur);	// 在左子树上求cur的双亲	
		if (temPtr != NULL) return temPtr;		// 双亲在左子树上

		temPtr = ParentAux(r->rightChild, cur);// 在右子树上求cur的双亲	
		if (temPtr != NULL) return temPtr;		// 双亲在右子树上
		else return NULL;						// 表示cur无双亲
	}
}
template <class ElemType>
//操作结果，返回pCur的双亲节点
BinTreeNode<ElemType>* BinaryTree<ElemType>::Parent(BinTreeNode<ElemType>* pCur) const
{
	return ParentAux(this->GetRoot(), pCur);
}
template <class ElemType>
//操作结果，链接双亲节点和孩子节点
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

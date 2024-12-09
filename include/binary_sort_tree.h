#if !defined(_BINARY_SORT_TREE_H_)
#define _BINARY_SORT_TREE_H_
#include "binary_tree.h"
template <class ElemType ,class KeyType>
class BinarySortTree:public BinaryTree<ElemType>
{
public:
	BinarySortTree();//构造函数
	~BinarySortTree();//析构函数
	BinTreeNode<ElemType>* Search(const KeyType& key) const;// 查找值为key的数据元素
	bool Insert(const ElemType& e);							// 插入数据元素e
	bool Delete(const KeyType& key);						// 删除关键字为key的数据元素
protected:
	BinTreeNode<ElemType>* SearchAux(const KeyType& key, BinTreeNode<ElemType>*& parent) const;		// 查找元素的值为key的数据元素
};
template <class ElemType, class KeyType>
BinarySortTree<ElemType,KeyType>::BinarySortTree()
{
}
template <class ElemType, class KeyType>
BinarySortTree<ElemType, KeyType>::~BinarySortTree()
{
}

template <class ElemType, class KeyType>
BinTreeNode<ElemType>* BinarySortTree<ElemType, KeyType>::SearchAux(const KeyType& key,
	BinTreeNode<ElemType>*& parent) const
// 操作结果: 返回指向元素的值为key的数据元素的指针，用parent返回其双亲
{
	BinTreeNode< ElemType>* p = this->GetRoot();			// 指向根结点
	parent = NULL;											// 指向p的双亲

	while (p != NULL && p->data != key)
	{	// 查找元素的值为key的结点
		if (key < p->data)
		{	// key更小，在左子树上进行查找
			parent = p;
			p = p->leftChild;
		}
		else
		{	// key更大，在右子树上进行查找
			parent = p;
			p = p->rightChild;
		}
	}

	return p;
}
template <class ElemType, class KeyType>
BinTreeNode<ElemType>* BinarySortTree<ElemType, KeyType>::Search(const KeyType& key) const
// 操作结果: 返回指向值为为key的数据元素的指针
{
	BinTreeNode<ElemType>* parent;								// 指向被查找结点的双亲
	return SearchAux(key, parent);
}

template <class ElemType, class KeyType>
bool BinarySortTree<ElemType, KeyType>::Insert(const ElemType& e)
// 操作结果: 插入数据元素e
{
	BinTreeNode<ElemType>* parent;								// 指向被查找结点的双亲

	if (SearchAux(e, parent) == NULL)
	{	// 查找失败，插入成功
		BinTreeNode<ElemType>* p;							// 插入的新结点
		p = new BinTreeNode<ElemType>(e);
		if (this->Empty())
		{	// 空二叉树，新结点为根结点
			this->m_pRoot = p;
		}
		else if (e < parent->data)
		{	// e更小，插入结点为parent的左孩子
			parent->leftChild = p;
		}
		else
		{	// e更大，插入结点为parent的右孩子
			parent->rightChild = p;
		}

		return true;
	}
	// 查找成功，插入失败
	return false;
}

template <class ElemType, class KeyType>
bool BinarySortTree<ElemType, KeyType>::Delete(const KeyType& key)
// 操作结果: 删除关键字为key的数据元素
{
	BinTreeNode<ElemType>* p, * parent,*tmp;//p为待删除节点，parent为其双亲，tmp为临时变量
	p = SearchAux(key, parent);				//查找关键字key的节点及其双亲
	if (p == NULL)
		return false;					// 查找失败，删除失败
	else
	{
		if (p->leftChild != NULL && p->rightChild != NULL)
		{
			// 关键字为key的结点有左右孩子	
			parent = p;  tmp = p->rightChild;		// tmp将指向右子树的最小值，parent是其双亲
			while (tmp->leftChild != NULL)
			{	// 查找右子树最小值(最左结点)
				parent = tmp;  tmp = tmp->leftChild;
			}
			p->data = tmp->data; 		// 右子树最小结点tmp数据替换p的数据
			p = tmp;					// tmp成为新的被删结点，p指向tmp
		}
		// 这时p只有一个孩子或p是叶子
		BinTreeNode<ElemType>** pp = NULL;//pp为p双亲指向p的指针地址,初始化时地址指向NULL,表示p无双亲
		if (parent)
		{//被删除节点有双亲
			pp =(parent->leftChild == p) ?  &parent->leftChild : &parent->rightChild;//pp指向左、或右孩子的地址
			if (p->leftChild != NULL) *pp = p->leftChild;//双亲节点孩子指向p的左孩子
			else if (p->rightChild != NULL) *pp = p->rightChild;//双亲节点孩子指向p的右孩子
			else *pp = NULL;//p为叶子节点，双亲节点孩子指向空
		}
		if (p == this->m_pRoot)
			this->m_pRoot = NULL;//树根节点赋空
		delete p;//删除节点p;
		return true;
	}
}
#endif // !defined(_BINARY_SORT_TREE_H_)

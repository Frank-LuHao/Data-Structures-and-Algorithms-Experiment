#if !defined(_THREAD_BINARY_TREE_H_)
#define _THREAD_BINARY_TREE_H_
#include "lk_binary_tree_base.h"
#include "thread_bin_tree_node.h"
typedef enum
{
	NO_THREAD_ORDER,//没有建立线索
	PRE_THREAD_ORDER,//先序线索
	IN_THREAD_ORDER,//中序线索
	POST_THREAD_ORDER,//后序线索
}TREAD_ORDER;
template <class ElemType>
class ThreadBinaryTree :public LkBinaryTreeBase<ElemType, ThreadBinTreeNode<ElemType>>
{
public:
	ThreadBinaryTree();
	~ThreadBinaryTree();
	virtual ThreadBinTreeNode<ElemType>* Parent(ThreadBinTreeNode<ElemType>* pCur) const; // 返回二叉树中结点cur的双亲
	virtual ThreadBinTreeNode<ElemType>* LeftChild(ThreadBinTreeNode<ElemType>* pCur) const; // 返回二叉树中结点cur的左孩子，重载
	virtual ThreadBinTreeNode<ElemType>* RightChild(ThreadBinTreeNode<ElemType>* pCur) const; // 返回二叉树中结点cur的右孩子，重载
	virtual void PreOrder(void (*visit)(const ElemType&)) const;	// 二叉树的先序遍历
	virtual void InOrder(void (*visit)(const ElemType&)) const;		// 二叉树的中序遍历	
	virtual void PostOrder(void (*visit)(const ElemType&)) const;	// 二叉树的后序遍历	
	void PreThread();			// 先序线索化二叉树
	void InThread();			// 中序线索化二叉树
	void PostThread();			// 后序线索化二叉树

protected:
	virtual void LinkParentChild(ThreadBinTreeNode<ElemType>* pParent, ThreadBinTreeNode<ElemType>* pChild, bool bLeft);//链接双亲和孩子
	void PreThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre);	// 先序线索化辅助函数
	void InThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre);	// 中序线索化以cur为根的二叉树辅助函数
	void PostThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre);// 后序线索化以cur为根的二叉树

private:
	TREAD_ORDER m_order;//当前建立的线索
};

template <class ElemType>
ThreadBinaryTree<ElemType>::ThreadBinaryTree()
{
	m_order = NO_THREAD_ORDER;
}
template <class ElemType>
ThreadBinaryTree<ElemType>::~ThreadBinaryTree()
{
}

template <class ElemType>
//操作结果，返回pCur的双亲节点
ThreadBinTreeNode<ElemType>* ThreadBinaryTree<ElemType>::Parent(ThreadBinTreeNode<ElemType>* pCur) const
{
	return pCur->parent;
}
template <class ElemType>
//操作结果，链接双亲节点和孩子节点
void ThreadBinaryTree<ElemType>::LinkParentChild(ThreadBinTreeNode<ElemType>* pParent, ThreadBinTreeNode<ElemType>* pChild, bool bLeft)
{
	if (!pParent)
		return;
	if (bLeft)
		pParent->leftChild = pChild;
	else
		pParent->rightChild = pChild;
	pChild->parent = pParent;//设置双亲指针
}
template <class ElemType>
//操作结果, 返回二叉树中结点cur的左孩子
ThreadBinTreeNode<ElemType>* ThreadBinaryTree<ElemType>::LeftChild(ThreadBinTreeNode<ElemType>* pCur) const
{
	if (!pCur)
		return NULL;
	if (pCur->leftTag == CHILD_PTR)//判断左标记是否为子树
		return pCur->leftChild;//返回左孩子
	return NULL;
}
template <class ElemType>
//操作结果, 返回二叉树中结点cur的右孩子
ThreadBinTreeNode<ElemType>* ThreadBinaryTree<ElemType>::RightChild(ThreadBinTreeNode<ElemType>* pCur) const
{
	if (!pCur)
		return NULL;
	if (pCur->rightTag == CHILD_PTR)//判断右标记是否为子树
		return pCur->rightChild;//返回右孩子
	return NULL;
}
template <class ElemType>
//操作结果，先序线索化辅助
void ThreadBinaryTree<ElemType>::PreThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre)
{
	if (cur != NULL)
	{	// 按先序遍历方式进行线索化
		if (LeftChild(cur) == NULL)
		{	// cur无左孩子,加线索 
			cur->leftChild = pre;				// cur前驱为pre
			cur->leftTag = THREAD_PTR;			// 线索标志
		}

		if (pre != NULL && RightChild(pre) == NULL)
		{	// pre无右孩子, 加线索
			pre->rightChild = cur;				// pre后继为cur
			pre->rightTag = THREAD_PTR;			// 线索标志 
		}

		pre = cur;								// 遍历下一结点时,cur为下一结点的前驱

		if (cur->leftTag == CHILD_PTR)
			PreThreadAux(cur->leftChild, pre);	// 线索化左子树
		if (cur->rightTag == CHILD_PTR)
			PreThreadAux(cur->rightChild, pre);// 线索化右子树
	}
}
template <class ElemType>
//操作结果，先序线索化二叉树
void ThreadBinaryTree<ElemType>::PreThread()
{
	ThreadBinTreeNode<ElemType>* pre = NULL;	// 开始线索化时前驱为空
	PreThreadAux(this->GetRoot(), pre);			// 先序线索化以root为根的二叉树
	if (RightChild(pre) == NULL)				// pre为前序序列中最后一个结点
		pre->rightTag = THREAD_PTR;				// 如无右孩子,则加线索标记
	m_order = PRE_THREAD_ORDER;
}
template <class ElemType>
//操作结果， 二叉树的先序遍历
void ThreadBinaryTree<ElemType>::PreOrder(void (*visit)(const ElemType& )) const
{
	if (m_order == PRE_THREAD_ORDER)
	{//如果已经进行了先序线索化，采用线索化遍历
		if (this->GetRoot() != NULL)
		{
			ThreadBinTreeNode<ElemType>* cur = this->GetRoot();
			// 从根开始遍历,根结点为中先序序列的第一个结点
			while (cur != NULL)
			{
				(*visit)(cur->data);					// 访问当前结点

				if (cur->rightTag == THREAD_PTR)
				{	// 右链为线索，后继为cur->rightChild
					cur = cur->rightChild;
				}
				else
				{	// 右链为孩子
					if (cur->leftTag == CHILD_PTR)
						cur = cur->leftChild;			// cur有左孩子,则左孩子为后继
					else
						cur = cur->rightChild;			// cur无左孩子,则右孩子为后继
				}
			}
		}
	}
	else
	{//否则调用基类的先序遍历方法
		LkBinaryTreeBase < ElemType, ThreadBinTreeNode<ElemType>>::PreOrder(visit);
	}

}
template <class ElemType>
// 操作结果：中序线索化以cur为根的二叉树,pre表示cur的前驱
void ThreadBinaryTree<ElemType>::InThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre)
{
	if (cur != NULL)
	{	// 按中序遍历方式进行线索化
		if (cur->leftTag == CHILD_PTR)
			InThreadAux(cur->leftChild, pre);	// 线索化左子树

		if (LeftChild(cur) == NULL)
		{	// cur无左孩子,加线索 
			cur->leftChild = pre;				// cur前驱为pre
			cur->leftTag = THREAD_PTR;			// 线索标志
		}
		else
		{	// cur有左孩子, 修改标志 
			cur->leftTag = CHILD_PTR;			// 孩子指针标志
		}

		if (pre != NULL && RightChild(pre) == NULL)
		{	// pre无右孩子, 加线索
			pre->rightChild = cur;				// pre后继为cur
			pre->rightTag = THREAD_PTR;			// 线索标志 
		}
		else if (pre != NULL)
		{	// cur有右孩子, 修改标志 
			pre->rightTag = CHILD_PTR;			// 孩子指针标志
		}
		pre = cur;								// 遍历下一结点时,cur为下一结点的前驱

		if (cur->rightTag == CHILD_PTR)
			InThreadAux(cur->rightChild, pre);	// 线索化右子树
	}
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::InThread()
// 操作结果：中序线索化二叉树
{
	ThreadBinTreeNode<ElemType>* pre = NULL;	// 开始线索化时前驱为空
	InThreadAux(this->GetRoot(), pre);					// 中序线索化以root为根的二叉树
	if (RightChild(pre) == NULL)				// pre为中序序列中最后一个结点
		pre->rightTag = THREAD_PTR;				// 如无右孩子,则加线索标记
	m_order = IN_THREAD_ORDER;					//当前二叉树为中序线索化
}

template <class ElemType>
// 操作结果：二叉树的中序遍历	
void ThreadBinaryTree<ElemType>::InOrder(void (*visit)(const ElemType&)) const
{
	if (m_order == IN_THREAD_ORDER)
	{//如果已经进行了中序线索化，采用线索化遍历
		if (this->GetRoot() != NULL)
		{
			ThreadBinTreeNode<ElemType>* cur = this->GetRoot();	// 从根开始遍历

			while (cur->leftTag == CHILD_PTR)			// 查找最左侧的结点，此结 
				cur = cur->leftChild;					// 点为中序序列的第一个结点
			while (cur != NULL)
			{
				(*visit)(cur->data);					// 访问当前结点

				if (cur->rightTag == THREAD_PTR)
				{	// 右链为线索，后继为cur->rightChild
					cur = cur->rightChild;
				}
				else
				{	// 右链为孩子，cur右子树最左侧的结点为后继
					cur = cur->rightChild;				// cur指向右孩子	
					while (cur->leftTag == CHILD_PTR)
						cur = cur->leftChild;			// 查找原cur右子树最左侧的结点
				}
			}
		}
	}
	else
	{	//否则调用基类的中序遍历方法
		LkBinaryTreeBase < ElemType, ThreadBinTreeNode<ElemType>>::InOrder(visit);
	}
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::PostThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre)
// 操作结果：后序线索化以cur为根的二叉树,pre表示cur的前驱
{
	if (cur != NULL)
	{	// 按后序遍历方式进行线索化
		if (cur->leftTag == CHILD_PTR)
			PostThreadAux(cur->leftChild, pre);// 线索化左子树
		if (cur->rightTag == CHILD_PTR)
			PostThreadAux(cur->rightChild, pre);// 线索化右子树

		if (LeftChild(cur) == NULL)
		{	// cur无左孩子,加线索 
			cur->leftChild = pre;				// cur前驱为pre
			cur->leftTag = THREAD_PTR;			// 线索标志
		}

		if (pre != NULL && RightChild(pre) == NULL)
		{	// pre无右孩子, 加线索
			pre->rightChild = cur;				// pre后继为cur
			pre->rightTag = THREAD_PTR;			// 线索标志 
		}

		pre = cur;								// 遍历下一结点时,cur为下一结点的前驱
	}
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::PostThread()
// 操作结果：后序线索化二叉树
{
	ThreadBinTreeNode<ElemType>* pre = NULL;	// 开始线索化时前驱为空
	PostThreadAux(this->GetRoot(), pre);						// 后序线索化以root为根的二叉树
	if (RightChild(pre) == NULL)					// pre为后序序列中最后一个结点
		pre->rightTag = THREAD_PTR;					// 如无右孩子,则加线索标记
	m_order = POST_THREAD_ORDER;					//当前二叉树为中序线索化
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::PostOrder(void (*visit)(const ElemType&)) const
// 操作结果：二叉树的后序遍历	
{
	if (m_order == POST_THREAD_ORDER)
	{
		if (this->GetRoot() != NULL)
		{
			ThreadBinTreeNode<ElemType>* cur = this->GetRoot();				// 从根开始遍历
			while (cur->leftTag == CHILD_PTR || cur->rightTag == CHILD_PTR)
			{	// 查找最左下的结点,此结点为后序序列第一个结点
				if (cur->leftTag == CHILD_PTR) cur = cur->leftChild;	// 移向左孩子
				else cur = cur->rightChild;								// 无左孩子,则移向右孩子
			}

			while (cur != NULL)
			{
				(*visit)(cur->data);									// 访问当前结点

				ThreadBinTreeNode<ElemType>* pt = cur->parent;		// 当前结点的双亲 
				if (cur->rightTag == THREAD_PTR)
				{	// 右链为线索, 后继为cur->rightChild
					cur = cur->rightChild;
				}
				else if (cur == this->GetRoot())
				{	// 结点cur是二叉树的根，其后继为空
					cur = NULL;
				}
				else if (pt->rightChild == cur || pt->leftChild == cur && pt->rightTag == THREAD_PTR)
				{	// 结点cur是其双亲的右孩子或是其双亲的左孩子且其双亲没有右子树，则
					// 其后继即为双亲结点
					cur = pt;
				}
				else
				{	// 结点cur是其双亲的左孩子，且其双亲有右子树，则其后继为双亲的右子树中按后
					// 续遍历的第一个访问的结点，即其双亲的右子树中最左下的结点
					cur = pt->rightChild;								// cur指向双亲的右孩子
					while (cur->leftTag == CHILD_PTR || cur->rightTag == CHILD_PTR)
					{	// 查找最左下的结点,此结点为后序序列第一个结点
						if (cur->leftTag == CHILD_PTR) cur = cur->leftChild;	// 移向左孩子
						else cur = cur->rightChild;						// 无左孩子,则移向右孩子
					}
				}
			}
		}
	}
	else
	{	//否则调用基类的后序遍历方法
		LkBinaryTreeBase < ElemType, ThreadBinTreeNode<ElemType>>::PostOrder(visit);
	}

}
#endif
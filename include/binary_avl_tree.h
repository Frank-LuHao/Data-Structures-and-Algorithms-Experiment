#if !defined(_BIN_AVL_TREE_H_)
#define _BIN_AVL_TREE_H_
#include "bin_avl_tree_node.h"
#include "lk_binary_tree_base.h"
#include "lk_stack.h"
template <class ElemType, class KeyType>
class BinaryAVLTree:public LkBinaryTreeBase<ElemType, BinAVLTreeNode<ElemType>>
{
public:
	BinaryAVLTree();
	~BinaryAVLTree();
	virtual BinAVLTreeNode<ElemType>* Parent(BinAVLTreeNode<ElemType>* pCur) const;		// 返回二叉树中结点cur的双亲
	BinAVLTreeNode<ElemType>* Search(const KeyType& key) const;							//查找关键字
	bool Insert(const ElemType& e);							// 插入数据元素e
	bool Delete(const KeyType& key);						// 删除关键字为key的数据元素
protected:
	virtual void LinkParentChild(BinAVLTreeNode<ElemType>* pParent, BinAVLTreeNode<ElemType>* pChild, bool bLeft);
	BinAVLTreeNode<ElemType>* ParentAux(BinAVLTreeNode<ElemType>* r, BinAVLTreeNode<ElemType>* cur) const;			// 返回cur的双亲
	void DeleteAux(const KeyType& key, BinAVLTreeNode<ElemType>*& p, LinkStack< BinAVLTreeNode<ElemType>*>& s);	// 删除p指向的结点
	BinAVLTreeNode<ElemType>* SearchAux(const KeyType& key, BinAVLTreeNode<ElemType>*& parent) const;//查找辅助函数
	BinAVLTreeNode<ElemType>* SearchAux(const KeyType& key, BinAVLTreeNode<ElemType>*& f,
		LinkStack<BinAVLTreeNode<ElemType>*>& s); // 返回指向元素值为key的元素的指针，用f返回其双亲
	void LeftRotate(BinAVLTreeNode<ElemType>*& subRoot);	// 对以subRoot为根的二叉树作左旋
	void RightRotate(BinAVLTreeNode<ElemType>*& subRoot);	// 对以subRoot为根的二叉树作右旋处理
	void InsertLeftBalance(BinAVLTreeNode<ElemType>*& subRoot);	// 对以subRoot为根的二叉树插入时作左平衡处理
	void InsertRightBalance(BinAVLTreeNode<ElemType>*& subRoot);// 对以subRoot为根的二叉树插入时作右平衡处理
	void InsertBalance(const ElemType& e, LinkStack< BinAVLTreeNode<ElemType>*>& s);// 从插入结点e根据查找路径进行回溯，并作平衡处理
	void DeleteLeftBalance(BinAVLTreeNode<ElemType>*& subRoot, bool& isShorter);// 对以subRoot为根的二叉树删除时作左平衡处理
	void DeleteRightBalance(BinAVLTreeNode<ElemType>*& subRoot, bool& isShorter);// 对以subRoot为根的二叉树删除时作右平衡处理
	void DeleteBalance(const KeyType& key, LinkStack<BinAVLTreeNode<ElemType>*>& s);// 从删除结点根据查找路径进行回溯，并作平衡处理
};
template <class ElemType, class KeyType>
BinaryAVLTree<ElemType,KeyType>::BinaryAVLTree()
{
}
template <class ElemType, class KeyType>
BinaryAVLTree<ElemType, KeyType>::~BinaryAVLTree()
{
}
template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::LinkParentChild(BinAVLTreeNode<ElemType>* pParent, BinAVLTreeNode<ElemType>* pChild, bool bLeft)
{
	if (!pParent)
		return;
	if (bLeft)
		pParent->leftChild = pChild;
	else
		pParent->rightChild = pChild;
}
template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::ParentAux(BinAVLTreeNode<ElemType>* r, BinAVLTreeNode<ElemType>* cur) const
// 操作结果：返回以r为根的二叉树, 结点cur的双亲
{
	if (r == NULL) return NULL;		// 空二叉树
	else if (r->leftChild == cur || r->rightChild == cur) return r; // r为cur的双亲
	else
	{	// 在子树上求双亲
		BinAVLTreeNode<ElemType>* temPtr;	// 临时指针

		temPtr = ParentAux(r->leftChild, cur);	// 在左子树上求cur的双亲	
		if (temPtr != NULL) return temPtr;		// 双亲在左子树上

		temPtr = ParentAux(r->rightChild, cur);// 在右子树上求cur的双亲	
		if (temPtr != NULL) return temPtr;		// 双亲在右子树上
		else return NULL;						// 表示cur无双亲
	}
}
template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::Parent(BinAVLTreeNode<ElemType>* pCur) const
{
	return ParentAux(this->GetRoot(), pCur);
}

template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::SearchAux(const KeyType& key,	BinAVLTreeNode<ElemType>*& parent) const
	// 操作结果: 返回指向元素的值为key的数据元素的指针，用parent返回其双亲
{
	BinAVLTreeNode< ElemType>* p = this->GetRoot();			// 指向根结点
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
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::SearchAux(
	const KeyType& key, BinAVLTreeNode<ElemType>*& f,
	LinkStack<BinAVLTreeNode<ElemType>*>& s)
	// 操作结果: 返回指向关键字为key的元素的指针,用f返回其双亲
{
	BinAVLTreeNode<ElemType>* p = this->GetRoot();	// 指向当前结点
	f = NULL;		// 指向p的双亲
	while (p != NULL && p->data != key)
	{	// 查寻关键字为key的结点
		if (key < p->data)
		{	// key更小，在左子树上进行查找
			f = p;
			s.Push(p);
			p = p->leftChild;
		}
		else
		{	// key更大，在右子树上进行查找
			f = p;
			s.Push(p);
			p = p->rightChild;
		}
	}
	return p;
}


template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::Search(const KeyType& key) const
// 操作结果: 返回指向值为为key的数据元素的指针
{
	BinAVLTreeNode<ElemType>* parent;								// 指向被查找结点的双亲
	return SearchAux(key, parent);
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::LeftRotate(BinAVLTreeNode<ElemType>*& subRoot)
// 操作结果: 对以subRoot为根的二叉树作左旋处理，处理之后subRoot指向新的树根结点，也就是
//	旋转处理前的右子树的根结点
{
	BinAVLTreeNode<ElemType>* ptrRChild;		// 工作变量
	ptrRChild = subRoot->rightChild;			// ptrRChild指向subRoot右孩子
	subRoot->rightChild = ptrRChild->leftChild;	// ptrRChild的左子树链接为subRoot的右子树
	ptrRChild->leftChild = subRoot;				// subRoot链接为ptrRChild的左孩子
	subRoot = ptrRChild;						// subRoot指向新的根结点
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::RightRotate(BinAVLTreeNode<ElemType>*& subRoot)
// 操作结果: 对以subRoot为根的二叉树作右旋处理，处理之后subRoot指向新的树根结点，也就是
//	旋转处理前的左子树的根结点
{
	BinAVLTreeNode<ElemType>* ptrLChild;		// 工作变量
	ptrLChild = subRoot->leftChild;				// ptrChild指向subRoot左孩子
	subRoot->leftChild = ptrLChild->rightChild;	// ptrLChild的右子树链接为subRoot的左子树
	ptrLChild->rightChild = subRoot;			// subRoot链接为ptrLChild的右孩子
	subRoot = ptrLChild;						// subRoot指向新的根结点
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::InsertLeftBalance(BinAVLTreeNode<ElemType>*& subRoot)
// 操作结果: 对以subRoot为根的二叉树插入时作左平衡处理,插入结点在subRoot的左子树上，处理后
//	subRoot指向新的树根结点
{
	BinAVLTreeNode<ElemType>* ptrLChild, * ptrLRChild;// 工作变量	
	ptrLChild = subRoot->leftChild;	// ptrLChild指向subRoot左孩子
	switch (ptrLChild->bf)
	{	// 根据subRoot的左子树的平衡因子作相应的平衡处理
	case LH:							// 插入结点在subRoot的左孩子的左子树上，作单右旋处理
		subRoot->bf = ptrLChild->bf = EH;// 平衡因子都为0
		RightRotate(subRoot);			// 右旋
		break;
	case RH:							// 插入结点在subRoot的左孩子的右子树上，作先左旋后右旋处理
		ptrLRChild = ptrLChild->rightChild;// ptrLRChild指向subRoot的左孩子的右子树的根
		switch (ptrLRChild->bf)
		{	// 修改subRoot及左孩子的平衡因子
		case LH:						// 插入结点在ptrLRChild的左子树上
			subRoot->bf = RH;
			ptrLChild->bf = EH;
			break;
		case EH:						// 插入前ptrLRChild为空,插入后trLRChild指向插入结点
			subRoot->bf = ptrLChild->bf = EH;
			break;
		case RH:						// 插入结点在ptrLRChild的右子树上
			subRoot->bf = EH;
			ptrLChild->bf = LH;
			break;
		}
		ptrLRChild->bf = 0;
		LeftRotate(subRoot->leftChild);	// 对subRoot左子树作左旋处理
		RightRotate(subRoot);			// 对subRoot作右旋处理
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::InsertRightBalance(BinAVLTreeNode<ElemType>*& subRoot)
// 操作结果: 对以subRoot为根的二叉树插入时作右平衡处理,插入结点在subRoot的右子树上,处理后
//	subRoot指向新的树根结点
{
	BinAVLTreeNode<ElemType>* ptrRChild, * ptrRLChild;	// 工作变量
	ptrRChild = subRoot->rightChild;	// ptrRChild指向subRoot右孩子
	switch (ptrRChild->bf)
	{	// 根据subRoot的右子树的平衡因子作相应的平衡处理
	case RH:							// 插入结点在subRoot的右孩子的右子树上,作单左旋处理
		subRoot->bf = ptrRChild->bf = EH;// 平衡因子都为0
		LeftRotate(subRoot);			// 左旋
		break;
	case LH:							// 插入结点在subRoot的右孩子的左子树上,作先右旋后左旋处理
		ptrRLChild = ptrRChild->leftChild;// ptrRLChild指向subRoot的右孩子的左子树的根
		switch (ptrRLChild->bf)
		{	// 修改subRoot及右孩子的平衡因子
		case RH:						// 插入结点在ptrRLChild的右子树上
			subRoot->bf = LH;
			ptrRChild->bf = EH;
			break;
		case EH:						// 插入前ptrRLChild为空,插入后ptrRLChild指向插入结点
			subRoot->bf = ptrRChild->bf = EH;
			break;
		case LH:						// 插入结点在ptrRLChild的左子树上
			subRoot->bf = EH;
			ptrRChild->bf = RH;
			break;
		}
		ptrRLChild->bf = 0;
		RightRotate(subRoot->rightChild);// 对subRoot右子树作右旋处理
		LeftRotate(subRoot);			// 对subRoot作左旋处理
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::InsertBalance(const ElemType& e,
	LinkStack<BinAVLTreeNode<ElemType>*>& s)
	// 操作结果: 从插入元素e根据查找路径进行回溯，并作平衡处理
{
	bool isTaller = true;
	while (!s.Empty() && isTaller)
	{
		BinAVLTreeNode<ElemType>* ptrCurNode, * ptrParent;
		s.Pop(ptrCurNode);			// 取出待平衡的结点
		if (s.Empty())
		{	// ptrCurNode已为根结点，ptrParent为空
			ptrParent = NULL;
		}
		else
		{	// ptrCurNode不为根结点，取出双亲ptrParent
			s.Top(ptrParent);
		}

		if (e < ptrCurNode->data)
		{	// e插入在ptrCurNode的左子树上
			switch (ptrCurNode->bf)
			{	// 检查ptrCurNode的平衡因子
			case LH:						// 插入后ptrCurNode->bf=2, 作左平衡处理
				if (ptrParent == NULL)
				{	// 已回溯到根结点
					InsertLeftBalance(ptrCurNode);
					this->m_pRoot = ptrCurNode;		// 转换后ptrCurNode为新根
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent左子树作平衡处理
					InsertLeftBalance(ptrParent->leftChild);
				}
				else
				{	// ptrParent右子树作平衡处理
					InsertLeftBalance(ptrParent->rightChild);
				}
				isTaller = false;
				break;
			case EH:					// 插入后ptrCurNode->bf=LH
				ptrCurNode->bf = LH;
				break;
			case RH:					// 插入后ptrCurNode->bf=EH
				ptrCurNode->bf = EH;
				isTaller = false;
				break;
			}
		}
		else
		{	// e插入在ptrCurNode的右子树上
			switch (ptrCurNode->bf)
			{	// 检查ptrCurNode的平衡度
			case RH:						// 插入后ptrCurNode->bf=-2, 作右平衡处理
				if (ptrParent == NULL)
				{	// 已回溯到根结点
					InsertRightBalance(ptrCurNode);
					this->m_pRoot = ptrCurNode;		// 转换后ptrCurNode为新根
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent左子树作平衡处理
					InsertRightBalance(ptrParent->leftChild);
				}
				else
				{	// ptrParent右子树作平衡处理
					InsertRightBalance(ptrParent->rightChild);
				}
				isTaller = false;
				break;
			case EH:						// 插入后ptrCurNode->bf=RH
				ptrCurNode->bf = RH;
				break;
			case LH:						// 插入后ptrCurNode->bf=EH
				ptrCurNode->bf = EH;
				isTaller = false;
				break;
			}
		}
	}
}

template <class ElemType, class KeyType>
bool BinaryAVLTree<ElemType, KeyType>::Insert(const ElemType& e)
// 操作结果: 插入数据元素e
{
	BinAVLTreeNode<ElemType>* f;
	LinkStack< BinAVLTreeNode<ElemType>*> s;
	if (SearchAux(e, f, s) == NULL)
	{	// 查找失败, 插入成功
		BinAVLTreeNode<ElemType>* p;		// 插入的新结点
		p = new BinAVLTreeNode<ElemType>(e);// 生成插入结点
		p->bf = 0;
		if (this->Empty())
		{	// 空二叉树，新结点为根结点
			this->m_pRoot = p;
		}
		else if (e < f->data)
		{	// e更小，插入结点为f的左孩子
			f->leftChild = p;
		}
		else
		{	// e更大，插入结点为f的右孩子
			f->rightChild = p;
		}

		InsertBalance(e, s);// 插入结点后作平衡处理
		return true;
	}
	else
	{	// 查找成功，插入失败
		return false;
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteLeftBalance(BinAVLTreeNode<ElemType>*&
	subRoot, bool& isShorter)
	// 操作结果: 对以subRoot为根的二叉树删除时作左平衡处理，删除subRoot的左子树上的结点，处
	//	理后subRoot指向新的树根结点
{
	BinAVLTreeNode<ElemType>* ptrRChild, * ptrRLChild;	// 工作变量
	ptrRChild = subRoot->rightChild;		// ptrRChild指向subRoot右孩子
	switch (ptrRChild->bf)
	{	// 根据subRoot的右子树的平衡因子作相应的平衡处理
	case RH:								// 右高，作单左旋转
		subRoot->bf = ptrRChild->bf = EH;	// 平衡因子都为0
		LeftRotate(subRoot);				// 左旋
		isShorter = true;
		break;
	case EH:								// 等高，作单左旋转
		subRoot->bf = RH;
		ptrRChild->bf = LH;
		LeftRotate(subRoot);				// 左旋
		isShorter = false;
		break;
	case LH:								// 左高,先右旋后左旋
		ptrRLChild = ptrRChild->leftChild;	// ptrRLChild指向subRoot的右孩子的左子树的根
		switch (ptrRLChild->bf)
		{	// 修改subRoot及右孩子的平衡因子
		case LH:
			subRoot->bf = EH;
			ptrRChild->bf = RH;
			break;
		case EH:
			subRoot->bf = ptrRChild->bf = EH;
			break;
		case RH:
			subRoot->bf = LH;
			ptrRChild->bf = EH;
			break;
		}
		isShorter = true;
		ptrRLChild->bf = 0;
		RightRotate(subRoot->rightChild);	// 对subRoot右子树作右旋处理
		LeftRotate(subRoot);				// 对subRoot作左旋处理
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteRightBalance(BinAVLTreeNode<ElemType>*
	& subRoot, bool& isShorter)
	// 操作结果: 对以subRoot为根的二叉树删除时作右平衡处理，删除subRoot的右子树上的结点，处
	//	理后subRoot指向新的树根结点
{
	BinAVLTreeNode<ElemType>* ptrLChild, * ptrLRChild;
	ptrLChild = subRoot->leftChild;	// ptrLChild指向subRoot左孩子
	switch (ptrLChild->bf)
	{	// 根据subRoot的左子树的平衡因子作相应的平衡处理
	case LH:							// 左高，作单右旋转
		subRoot->bf = ptrLChild->bf = EH;// 平衡因子都为0
		RightRotate(subRoot);			// 右旋
		isShorter = true;
		break;
	case EH:							// 等高，作单右旋转
		subRoot->bf = LH;
		ptrLChild->bf = RH;
		RightRotate(subRoot);			// 右旋
		isShorter = false;
		break;
	case RH:							// 右高，先左旋后右旋
		ptrLRChild = ptrLChild->rightChild;// ptrLRChild指向subRoot的左孩子的右子树的根
		switch (ptrLRChild->bf)
		{	// 修改subRoot及左孩子的平衡因子
		case LH:
			subRoot->bf = RH;
			ptrLChild->bf = EH;
			break;
		case EH:
			subRoot->bf = ptrLChild->bf = EH;
			break;
		case RH:
			subRoot->bf = EH;
			ptrLChild->bf = LH;
			break;
		}
		isShorter = true;
		ptrLRChild->bf = 0;
		LeftRotate(subRoot->leftChild);	// 对subRoot左子树作左旋处理
		RightRotate(subRoot);			// 对subRoot作右旋处理
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteBalance(const KeyType& key,
	LinkStack<BinAVLTreeNode<ElemType>*>& s)
	// 操作结果: 从删除结点根据查找路径进行回溯，并作平衡处理
{
	bool isShorter = true;
	while (!s.Empty() && isShorter)
	{
		BinAVLTreeNode<ElemType>* ptrCurNode, * ptrParent;
		s.Pop(ptrCurNode);					// 取出待平衡的结点
		if (s.Empty())
		{	// ptrCurNode已为根结点，ptrParent为空
			ptrParent = NULL;
		}
		else
		{	// ptrCurNode不为根结点，取出双亲ptrParent
			s.Top(ptrParent);
		}

		if (key < ptrCurNode->data)
		{	// 删除ptrCurNode的左子树上的结点
			switch (ptrCurNode->bf)
			{	// 检查ptrCurNode的平衡度
			case LH:						// 左高
				ptrCurNode->bf = EH;
				break;
			case EH:						// 等高
				ptrCurNode->bf = RH;
				isShorter = false;
				break;
			case RH:						// 右高
				if (ptrParent == NULL)
				{	// 已回溯到根结点
					DeleteLeftBalance(ptrCurNode, isShorter);
					this->m_pRoot = ptrCurNode;		// 转换后ptrCurNode为新根
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent左子树作平衡处理
					DeleteLeftBalance(ptrParent->leftChild, isShorter);
				}
				else
				{	// ptrParent右子树作平衡处理
					DeleteLeftBalance(ptrParent->rightChild, isShorter);
				}
				break;
			}
		}
		else
		{	// 删除ptrCurNode的右子树上的结点
			switch (ptrCurNode->bf)
			{	// 检查ptrCurNode的平衡度
			case RH: 						// 右高
				ptrCurNode->bf = EH;
				break;
			case EH: 						// 等高
				ptrCurNode->bf = LH;
				isShorter = false;
				break;
			case LH: 						// 左高
				if (ptrParent == NULL)
				{	// 已回溯到根结点
					DeleteLeftBalance(ptrCurNode, isShorter);
					this->m_pRoot = ptrCurNode;		// 转换后ptrCurNode为新根
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent左子树作平衡处理
					DeleteLeftBalance(ptrParent->leftChild, isShorter);
				}
				else
				{	// ptrParent右子树作平衡处理
					DeleteLeftBalance(ptrParent->rightChild, isShorter);
				}
				break;
			}
		}
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteAux(const KeyType& key,
	BinAVLTreeNode<ElemType>*& p, LinkStack< BinAVLTreeNode<ElemType>*>& s)
	// 操作结果: 删除p指向的结点
{
	BinAVLTreeNode<ElemType>* temPtr, * temF;  // 临时变量
	if (p->leftChild == NULL && p->rightChild == NULL)
	{	// p为叶结点
		delete p;
		p = NULL;
		DeleteBalance(key, s);
	}
	else if (p->leftChild == NULL)
	{	// p只有左子树为空
		temPtr = p;
		p = p->rightChild;
		delete temPtr;
		DeleteBalance(key, s);
	}
	else if (p->rightChild == NULL)
	{	// p只有右子树非空
		temPtr = p;
		p = p->leftChild;
		delete temPtr;
		DeleteBalance(key, s);
	}
	else
	{	// p左右子非空
		temF = p;
		s.Push(temF);
		temPtr = p->leftChild;
		while (temPtr->rightChild != NULL)
		{	// 查找p在中序序列中直接前驱temPtr及其双亲temF，temPtr无右子树为空
			temF = temPtr;
			s.Push(temF);
			temPtr = temPtr->rightChild;
		}
		p->data = temPtr->data;// 将temPtr指向结点的元素值赋值给p指向结点的元素值

		// 删除temPtr指向的结点
		if (temF->rightChild == temPtr)
		{	// 删除temF的右孩子
			DeleteAux(key, temF->rightChild, s);
		}
		else
		{	// 删除temF的左孩子
			DeleteAux(key, temF->leftChild, s);
		}
	}
}

template <class ElemType, class KeyType>
bool BinaryAVLTree<ElemType, KeyType>::Delete(const KeyType& key)
// 操作结果: 删除关键字为key的结点
{
	BinAVLTreeNode<ElemType>* p, * f;
	LinkStack< BinAVLTreeNode<ElemType>*> s;
	p = SearchAux(key, f, s);
	if (p == NULL)
	{	// 查找失败, 删除失败
		return false;
	}
	else
	{	// 查找成功, 插入成功
		if (f == NULL)
		{	// 被删除结点为根结点
			DeleteAux(key, this->m_pRoot, s);
		}
		else if (key < f->data)
		{	// key更小, 删除f的左孩子
			DeleteAux(key, f->leftChild, s);
		}
		else
		{	// key更大, 删除f的右孩子
			DeleteAux(key, f->rightChild, s);
		}
		return true;
	}
}
#endif // !defined(_BIN_AVL_TREE_H_)

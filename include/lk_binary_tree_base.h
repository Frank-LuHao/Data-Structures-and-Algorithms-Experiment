#if !defined(_LK_BINARY_TREE_BASE_H_)
#define _LK_BINARY_TREE_BASE_H_
#include "binary_tree_base.h"
template <class ElemType,class NodeType>
class LkBinaryTreeBase :public BinaryTreeBase<ElemType, NodeType*>
{
public:
	LkBinaryTreeBase();
	~LkBinaryTreeBase();
	LkBinaryTreeBase(LkBinaryTreeBase< ElemType, NodeType>& source);//复制构造函数
	LkBinaryTreeBase(NodeType* r);					// 复制以r为根的二叉树
	LkBinaryTreeBase(const ElemType& e);							// 建立以e为根的二叉树
	virtual NodeType* GetRoot() const;						// 返回二叉树的根
	virtual bool NodeEmpty(NodeType* pCur) const;				// 判断结点cur是否为空
	virtual bool GetItem(NodeType* pCur, ElemType& e) const;	// 返回结点cur的元素值
	virtual bool SetElem(NodeType* pCur, const ElemType& e) ;	// 将结点cur的值置为e
	virtual bool Empty() const;								// 判断二叉树是否为空
	virtual NodeType* LeftChild(NodeType* pCur) const;		// 返回二叉树中结点cur的左孩子
	virtual NodeType* RightChild(NodeType* pCur) const;	// 返回二叉树中结点cur的右孩子
	virtual NodeType* Parent(NodeType* pCur) const = 0;		// 返回二叉树中结点cur的双亲
	virtual bool CreateBinaryTree(ElemType pre[], ElemType in[], int n);//根据先序、中序序列创建二叉树
	LkBinaryTreeBase<ElemType, NodeType>& operator=(const LkBinaryTreeBase<ElemType, NodeType>& source);	// 重载赋值运算符
protected:
	virtual ElemType NodeElem(NodeType* pCur) const ;		// 返回指定节点的元素值
	virtual void ReleaseNode(NodeType* &pCur);					// 将结点cur置空,如果是链式存储删除节点内存
	virtual NodeType* CreateChildNode(NodeType* pCur, bool bLeft);//创建cur节点的子节点
	virtual void LinkParentChild(NodeType* pParent, NodeType* pChild, bool bLeft) = 0;//链接双亲节点和孩子节点
	NodeType* CopyTreeAux(const NodeType* r);// 复制二叉树
	NodeType* m_pRoot;//根节点

};

template <class ElemType,class NodeType>
//操作结果，无参数构造函数
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase()
{
	m_pRoot = new NodeType;
}
template <class ElemType,class NodeType>
//操作结果，析构对象
LkBinaryTreeBase<ElemType, NodeType>::~LkBinaryTreeBase()
{
	if (m_pRoot)
	{//存在根节点
		this->DestroyAux(m_pRoot);//销毁树，释放内存资源
		m_pRoot = NULL;
	}
}
template <class ElemType, class NodeType>
//操作结果，复制构造
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase(LkBinaryTreeBase< ElemType, NodeType>& source)
{
	*this = source;//由于重载了'=',可以直接赋值
}
template <class ElemType, class NodeType>
//操作结果，复制以r为根的二叉树
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase(NodeType* r)
{
	m_pRoot = CopyTreeAux(r);
}
template <class ElemType, class NodeType>
//操作结果，建立以e为根的二叉树
LkBinaryTreeBase<ElemType, NodeType>::LkBinaryTreeBase(const ElemType& e)
{
	m_pRoot = new NodeType;
	SetElem(m_pRoot, e);
}

template <class ElemType, class NodeType>
//操作结果，返回根节点
NodeType* LkBinaryTreeBase<ElemType, NodeType>::GetRoot() const
{
	return m_pRoot;
}
template <class ElemType, class NodeType>
//操作结果，判断节点是否为空
bool LkBinaryTreeBase<ElemType, NodeType>::NodeEmpty(NodeType* pCur) const
{
	if (!pCur)
		return true;
	return false;
}
template <class ElemType, class NodeType>
//操作结果，获取节点数据
bool LkBinaryTreeBase<ElemType, NodeType>::GetItem(NodeType* pCur, ElemType& e) const
{
	if (!pCur)
		return false;
	e = NodeElem(pCur);
	return true;
}
template <class ElemType, class NodeType>
//操作结果，判断是否树为空
bool LkBinaryTreeBase<ElemType, NodeType>::Empty() const
{
	if (!m_pRoot)
		return true;//根节点为空，树为空
	if (!LeftChild(m_pRoot) && !RightChild(m_pRoot))
		return true;//根节点没有左孩子和右孩子，树为空
	return false;
}
template <class ElemType, class NodeType>
//操作结果，获取pCur节点的左孩子
NodeType* LkBinaryTreeBase<ElemType, NodeType>::LeftChild(NodeType* pCur) const
{
	return pCur->leftChild;//返回左孩子
}
template <class ElemType, class NodeType>
//操作结果，获取pCur节点的右孩子
NodeType* LkBinaryTreeBase<ElemType, NodeType>::RightChild(NodeType* pCur) const
{
	return pCur->rightChild;//返回右孩子
}
template <class ElemType, class NodeType>
//操作结果，设置pCur节点的数据
bool LkBinaryTreeBase<ElemType, NodeType>::SetElem(NodeType* pCur, const ElemType& e)
{
	if (pCur)
		return false;
	pCur->data = e;
	return true;
}
template <class ElemType, class NodeType>
//操作结果，获取pCur节点的数据
ElemType LkBinaryTreeBase<ElemType, NodeType>::NodeElem(NodeType* pCur) const
{
	if (!pCur)
		return 0;
	return pCur->data;
}
template <class ElemType, class NodeType>
//操作结果，释放pCur节点的内存资源
void LkBinaryTreeBase<ElemType, NodeType>::ReleaseNode(NodeType*& pCur)
{
	if (!pCur)
		return;
	delete pCur;
	pCur = NULL;
}
template <class ElemType, class NodeType>
//操作结果，创建pCur节点的子节点
NodeType* LkBinaryTreeBase<ElemType, NodeType>::CreateChildNode(NodeType* pCur, bool bLeft)
{
	if (!pCur)
	{//是根节点
		NodeType* pNode = new NodeType;
		return pNode;
	}
	if (LeftChild(pCur) && bLeft)
		return NULL;
	else if (RightChild(pCur) && !bLeft)
		return NULL;
	NodeType* pNode = new NodeType;
	LinkParentChild(pCur, pNode, bLeft);//链接当前节点和孩子节点
	return pNode;
}
template <class ElemType, class NodeType>
//操作结果，复制二叉树
NodeType* LkBinaryTreeBase<ElemType, NodeType>::CopyTreeAux(const NodeType* r)
{
	if (r == NULL)
	{	// 复制空二叉树
		return NULL;					// 空二叉树根为空	
	}
	else
	{	// 复制非空二叉树
		NodeType* pLChild = CopyTreeAux(LeftChild(r));	// 复制左子树
		NodeType* pRChild = CopyTreeAux(RightChild(r));	// 复制右子树
		NodeType* pParent = new NodeType;
		SetElem(pParent, r->data);//复制数据
		LinkParentChild(pParent, pLChild, true);//链接双亲节点和左孩子节点
		LinkParentChild(pParent, pRChild, false);//链接双亲节点和右孩子节点
		return pParent;
	}
}
template <class ElemType, class NodeType>
//操作结果，重载赋值运算符
LkBinaryTreeBase<ElemType, NodeType>& LkBinaryTreeBase<ElemType, NodeType>::operator=(const LkBinaryTreeBase<ElemType, NodeType>& source)
{
	if (&source != this)
	{
		DestroyAux(m_pRoot);//首先销毁树
		m_pRoot = CopyTreeAux(source.GetRoot());
	}
	return *this;
}
template <class ElemType, class NodeType>
//操作结果,根据先序、中序序列创建链式存储二叉树
bool LkBinaryTreeBase<ElemType, NodeType>::CreateBinaryTree(ElemType pre[], ElemType in[], int n)
{
	if (!pre || !in || n < 1)
		return false;
	this->DestroyAux(m_pRoot);//首先销毁树
	m_pRoot = this->CreateBinaryTreeAux(NULL, pre, in, 0, n - 1, 0, n - 1,false);//根据先序、中序序列生成二叉树
	return true;
}
#endif
#if !defined(_SQ_BINARY_TREE_H_)
#define _SQ_BINARY_TREE_H_
#include "binary_tree_base.h"
#include "SqList.h"
#include "sq_bin_tree_node.h"
template <class ElemType>
#define DEFAULT_SIZE 256
class SqBinaryTree :public BinaryTreeBase<ElemType,int>,private SqList<SqBinTreeNode<ElemType>>
{
public:
	SqBinaryTree();//构造空2差树
	SqBinaryTree(ElemType e,int nSize = DEFAULT_SIZE);//构造以e为根的2叉树
	~SqBinaryTree();
	SqBinaryTree(const SqBinaryTree<ElemType>& source);		// 复制构造函数模板
	SqBinaryTree(SqBinTreeNode<ElemType> es[], int r, int size = DEFAULT_SIZE);
	// 由es[]、r与size构造二叉树	
	SqBinaryTree<ElemType>& operator=(const SqBinaryTree<ElemType>&source);//重载'='运算符
	virtual int GetRoot() const ;						// 返回二叉树的根
	virtual bool NodeEmpty(int cur) const ;				// 判断结点cur是否为空
	virtual bool GetItem(int cur, ElemType& e) const ;	// 返回结点cur的元素值
	virtual bool SetElem(int cur, const ElemType& e) ;	// 将结点cur的值置为e
	virtual bool Empty() const ;						// 判断二叉树是否为空
	virtual int &LeftChild(int cur) const ;		// 返回二叉树中结点cur的左孩子
	virtual int &RightChild(int cur) const ;	// 返回二叉树中结点cur的右孩子
	virtual int Parent(int cur) const ;		// 返回二叉树中结点cur的双亲
	virtual bool CreateBinaryTree(ElemType pre[], ElemType in[], int n);//根据先序、中序序列创建二叉树
protected:
	virtual ElemType NodeElem(int cur) const ;		// 返回指定节点的元素值
	virtual void ReleaseNode(int& cur) ;			// 将结点cur置空,如果是链式存储删除节点内存
	virtual int CreateChildNode(int cur, bool bLeft) ;//创建cur节点的子节点
	int m_nRoot;
};
template <class ElemType>
SqBinaryTree<ElemType>::SqBinaryTree()
	:SqList<SqBinTreeNode<ElemType>>(DEFAULT_SIZE)
{

	this->m_nDataLen = DEFAULT_SIZE;
	memset(this->m_pElemData, 0, sizeof(SqBinTreeNode<ElemType>) * DEFAULT_SIZE);
	m_nRoot = 1;
}
template <class ElemType>
SqBinaryTree<ElemType>::SqBinaryTree(ElemType e,int nSize)
	:SqList<SqBinTreeNode<ElemType>>(nSize)
{
	
	this->m_nDataLen = nSize;
	memset(this->m_pElemData, 0, sizeof(SqBinTreeNode<ElemType>) * nSize);
	m_nRoot = 1;
	this->m_pElemData[m_nRoot].tag = USED_NODE;	// 根标志
	this->m_pElemData[m_nRoot].data = e;			// 根的数据元素值
}
template <class ElemType>
SqBinaryTree<ElemType>::~SqBinaryTree()
{
}

template <class ElemType>
SqBinaryTree<ElemType>& SqBinaryTree<ElemType>::operator=(const SqBinaryTree<ElemType>& source)
// 操作结果：由已知二叉树source复制到当前二叉树——重载赋值运算符
{
	if (&source != this)
	{
		this->Clear();				//调用顺序表函数清空数据
		int maxSize = source.m_nDataLen;	// 最大结点个数
		this->SetBufferSize(maxSize + 1);		//重新顺序表缓冲区大小
		// 复制结点数据
		memcpy(this->m_pElemData, source.m_pElemData, sizeof(SqBinTreeNode<ElemType>) * maxSize);
		this->m_nDataLen = maxSize;		//设置顺序表的当前有效数据长度
		m_nRoot = source.GetRoot();		// 复制二叉树根
	}
	return *this;
}

template <class ElemType>
SqBinaryTree<ElemType>::SqBinaryTree(const SqBinaryTree<ElemType>& source)
// 操作结果：由已知二叉树构造新二叉树——复制构造函数模板
{
	*this = source;//由于重载了'='，可以直接赋值
}

template <class ElemType>
SqBinaryTree<ElemType>::SqBinaryTree(SqBinTreeNode<ElemType> es[], int r, int size)
// 操作结果：由es[]与r构造最大结点个数为size的二叉树, 
	:SqList<SqBinTreeNode<ElemType>>(size)
{
	
	this->m_nDataLen = size;									// 最大结点个数
	memcpy(this->m_pElemData, es, sizeof(SqBinTreeNode<ElemType>) * size);// 复制结点
	m_nRoot = r;			// 复制二叉树根
}

template <class ElemType>
int SqBinaryTree<ElemType>::GetRoot() const
// 操作结果：返回二叉树的根
{
	return m_nRoot;					// m_nRoot为根
}



template <class ElemType>
int & SqBinaryTree<ElemType>::LeftChild(int cur) const
// 操作结果：返回二叉树结点cur的左孩子
{
	int leftChild = 2 * cur;// 2 * cur为左孩子 
	return leftChild;
}

template <class ElemType>
int & SqBinaryTree<ElemType>::RightChild(int cur) const
// 操作结果：返回二叉树结点cur的右孩子
{
	int rightChild = 2 * cur;// 2 * cur + 1为右孩子
	return rightChild;			
}

template <class ElemType>
int SqBinaryTree<ElemType>::Parent(int cur) const
// 操作结果：返回二叉树结点cur的双亲
{
	return cur / 2;				// cur / 2为双亲
}
template <class ElemType>
bool SqBinaryTree<ElemType>::NodeEmpty(int cur) const
// 操作结果：如果结点cur为空,则返回true,否则返回false
{
	if (cur <1 || cur > this->m_nDataLen || this->m_pElemData[cur].tag == EMPTY_NODE) return true;	// 此时结点为空
	else return false;
}
template <class ElemType>
bool SqBinaryTree<ElemType>::GetItem(int cur, ElemType& e) const
// 操作结果：用e返回结点cur的元素值,则操作成功,返回true,否则返回false
{
	if (NodeEmpty(cur)) return false;		// 操作失败
	else
	{
		e = this->m_pElemData[cur].data;				// 用e返回结点cur的元素值
		return true;						// 操作成功
	}
}

template <class ElemType>
bool SqBinaryTree<ElemType>::SetElem(int cur, const ElemType& e)
// 操作结果：如果不存在结点cur,则返回false,否则返回true,并将结点cur的值设置为e
{
	if (NodeEmpty(cur)) return false;		// 操作失败
	else
	{	// 存在结点cur
		this->m_pElemData[cur].data = e;				// 将结点cur的值设置为e
		return true;						// 返回true
	}
}

template <class ElemType>
bool SqBinaryTree<ElemType>::Empty() const
// 操作结果：判断二叉树是否为空
{
	return NodeEmpty(m_nRoot);
}
template <class ElemType>
//操作结果，返回节点cur的数据
ElemType SqBinaryTree<ElemType>::NodeElem(int cur) const
{
	return this->m_pElemData[cur].data;
}
template <class ElemType>
//操作结果，设置节点cur为空节点
void SqBinaryTree<ElemType>::ReleaseNode(int& cur)
{
	this->m_pElemData[cur].tag = EMPTY_NODE;
}
template <class ElemType>
//操作结果,创建节点cur的孩子节点
int SqBinaryTree<ElemType>::CreateChildNode(int cur, bool bLeft)
{
	if (cur < 0)
	{//是根节点

		return 1;
	}
	int nChildNodePtr = 0;
	if (NodeEmpty(cur))
		return 0;//当前节点为空节点，返回0
	if (bLeft)
		nChildNodePtr = LeftChild(cur);//孩子节点为左孩子节点
	else 
		nChildNodePtr = RightChild(cur);//孩子节点为右孩子节点
	if (nChildNodePtr >= this->m_nBufferLen)
	{
		this->SetBufferSize(nChildNodePtr*2);//重新分配内存空间
		this->m_nDataLen = nChildNodePtr + 1;//设置当前有效数据长度
	}
	this->m_pElemData[nChildNodePtr].tag = USED_NODE;//标志设置为有效
	return nChildNodePtr;
}
template <class ElemType>
//操作结果，根据先序、中序序列生成顺序存储二叉树
bool SqBinaryTree<ElemType>::CreateBinaryTree(ElemType pre[], ElemType in[], int n)
{
	if (!pre || !in || n < 1)
		return false;
	if (!this->SetBufferSize(n + 1))//重新根据n设置缓冲区大小
		return false;//设置缓冲区失败，返回
	memset(this->m_pElemData, 0, sizeof(SqBinTreeNode<ElemType>) * (n + 1));//清空二叉树
	m_nRoot = this->CreateBinaryTreeAux(-1, pre, in, 0, n - 1, 0, n - 1, false);
	return true;
}


#endif // !defined(_SQ_BINARY_TREE_H_)

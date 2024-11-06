#if !defined(_CHILD_SIBLING_TREE_H_)
#define _CHILD_SIBLING_TREE_H_
#include "tree_base.h"
#include "child_sibling_tree_node.h"
// 孩子、兄弟表示树结点类模板
template <class ElemType>
class ChildSiblingTree :public TreeBase<ElemType, ChildSiblingTreeNode<ElemType>*>
{
public:
	ChildSiblingTree();
	~ChildSiblingTree();
	ChildSiblingTree(const ElemType& e);// 建立以数据元素e为根的树
	ChildSiblingTree(const ChildSiblingTree<ElemType>& source);	// 复制构造函数模板
	ChildSiblingTree(ElemType items[], int parents[], int r, int n);
	// 建立数据元素为items[]，对应结点双亲为parents[]，根结点位置为r，结点个数为n的树
	ChildSiblingTree<ElemType>& operator=(const ChildSiblingTree<ElemType>& source);
	// 重载赋值运算符
	virtual ChildSiblingTreeNode<ElemType>* GetRoot() const;						// 返回树的根
	virtual bool Empty() const;									// 判断树是否为空
	virtual bool NodeEmpty(ChildSiblingTreeNode<ElemType>* cur) const;					// 判断结点cur是否为空
	virtual bool GetElem(ChildSiblingTreeNode<ElemType>* cur, ElemType& e) const;	// 用e返回结点元素值
	virtual bool SetElem(ChildSiblingTreeNode<ElemType>* cur, const ElemType& e);		// 将结cur的值置为e
	virtual ChildSiblingTreeNode<ElemType>* FirstChild(ChildSiblingTreeNode<ElemType>* cur) const;	// 返回树结点cur的第一个孩子
	virtual ChildSiblingTreeNode<ElemType>* RightSibling(ChildSiblingTreeNode<ElemType>* cur) const;// 返回树结点cur的右兄弟
	virtual ChildSiblingTreeNode<ElemType>* Parent(ChildSiblingTreeNode<ElemType>* cur) const;		// 返回树结点cur的双亲
private:
	virtual ElemType NodeElem(ChildSiblingTreeNode<ElemType>* cur) const;		// 返回指定节点的元素值
	virtual void ReleaseNode(ChildSiblingTreeNode<ElemType>*& cur);				// 将结点cur置空,如果是链式存储删除节点内存
	virtual ChildSiblingTreeNode<ElemType>* CreateChildNode(ChildSiblingTreeNode<ElemType>* cur, int  position, const ElemType& e);//创建cur节点的子节点
	virtual ChildSiblingTreeNode<ElemType>* RemoveChild(ChildSiblingTreeNode<ElemType>* parent, int position);//把position的节点从parent移除
	virtual void ClearRemovedNode();//清空已设置删除标记的节点
	ChildSiblingTreeNode<ElemType>* ParentAux(ChildSiblingTreeNode<ElemType>* r,ChildSiblingTreeNode<ElemType>* cur) const;	// 返回cur的双亲辅助函数
	ChildSiblingTreeNode<ElemType>* CopyTreeAux(const ChildSiblingTreeNode<ElemType>* r);	// 复制树辅助函数
	ChildSiblingTreeNode<ElemType>* CreateTreeGAux(ElemType items[], int parents[], int r, int n);
	// 建立数据元素为items[],对应结点双亲为parents[],根结点位置为r,结点个数为n的
	// 树，并返回树的根
	ChildSiblingTreeNode<ElemType>* m_pRoot;//根节点
};
template <class ElemType>
ChildSiblingTree<ElemType>::ChildSiblingTree()
{
	m_pRoot = NULL;
}
template <class ElemType>
ChildSiblingTree<ElemType>::~ChildSiblingTree()
{
}
template <class ElemType>
//操作结果：获取根节点
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::GetRoot() const
{
	return m_pRoot;
}
template <class ElemType>
//操作结果：判断树是否为空
bool ChildSiblingTree<ElemType>::Empty() const
{
	if (!m_pRoot)
		return true;
	return false;
}
template <class ElemType>
//操作结果：判断结点cur是否为空
bool ChildSiblingTree<ElemType>::NodeEmpty(ChildSiblingTreeNode<ElemType>* cur) const
{
	if (!cur)
		return true;
	return false;
}
template <class ElemType>
//操作结果：用e返回结点元素值
bool ChildSiblingTree<ElemType>::GetElem(ChildSiblingTreeNode<ElemType>* cur, ElemType& e) const
{
	if (!cur)
		return false;
	e = cur->data;
	return true;
}
template <class ElemType>
//操作结果：将结cur的值置为e
bool ChildSiblingTree<ElemType>::SetElem(ChildSiblingTreeNode<ElemType>* cur, const ElemType& e)
{
	if (!cur)
		return false;
	cur->data = e;
	return true;
}
template <class ElemType>
//操作结果：返回节点cur的第一个孩子
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::FirstChild(ChildSiblingTreeNode<ElemType>* cur) const
{
	return cur->firstChild;
}
template <class ElemType>
//操作结果：返回树结点cur的右兄弟
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::RightSibling(ChildSiblingTreeNode<ElemType>* cur) const
{
	return cur->rightSibling;
}
template <class ElemType>
//操作结果：返回cur的双亲辅助函数
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::ParentAux(ChildSiblingTreeNode<ElemType>* r, ChildSiblingTreeNode<ElemType>* cur) const
{
	if (r == NULL) return NULL;					// 空二叉树

	ChildSiblingTreeNode<ElemType>* child;		// 孩子 
	for (child = FirstChild(r); child != NULL;
		child = RightSibling(child))
	{	// 找查r的孩子
		if (child == cur) return r;				// cur双亲为r
	}

	for (child = FirstChild(r); child != NULL; child = RightSibling(child))
	{	// 在子树上求双亲
		ChildSiblingTreeNode<ElemType>* tmp;
		tmp = ParentAux(child, cur);			// 在子树上求cur的双亲	
		if (tmp != NULL) return tmp;			// 双亲在子树上
	}

	return NULL;								// 未找到双亲
}

template <class ElemType>
//操作结果：返回树结点cur的双亲
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::Parent(ChildSiblingTreeNode<ElemType>* cur) const
{
	return ParentAux(GetRoot(), cur);				// 用辅助函数求树中结点的双亲
}
template <class ElemType>
//操作结果：返回指定节点的元素值
ElemType ChildSiblingTree<ElemType>::NodeElem(ChildSiblingTreeNode<ElemType>* cur) const
{
	return cur->data;
}
template <class ElemType>
//操作结果：将结点cur置空,如果是链式存储删除节点内存
void ChildSiblingTree<ElemType>::ReleaseNode(ChildSiblingTreeNode<ElemType>*& cur)
{
	if (cur)
	{
		delete cur;
		cur = NULL;
	}
}
template <class ElemType>
//操作结果：创建cur节点的子节点
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::CreateChildNode(ChildSiblingTreeNode<ElemType>* cur, int  position, const ElemType& e)
{
	if (!cur)
		return NULL;
	ChildSiblingTreeNode<ElemType>* pNode = new ChildSiblingTreeNode<ElemType>;
	pNode->data = e;
	if (position == 0)
	{
		ChildSiblingTreeNode<ElemType>* pFirstChild = cur->firstChild;
		cur->firstChild = pNode;
		pNode->rightSibling = pFirstChild;
		return pNode;
	}

	ChildSiblingTreeNode<ElemType>* pScanNode = cur->firstChild;
	ChildSiblingTreeNode<ElemType>* pTailNode = NULL;
	int pos = 0;
	bool bFindPos = false;
	while (pScanNode)
	{
		pos++;
		if (pos == position)
		{//在position位置插入pNode;
			ChildSiblingTreeNode<ElemType>* pRightSibling = pScanNode->rightSibling;
			pScanNode->rightSibling = pNode;
			pNode->rightSibling = pRightSibling;
			bFindPos = TRUE;
			break;//终止循环
		}
		if (!pScanNode->rightSibling)
			pTailNode = pScanNode;
		pScanNode = pScanNode->rightSibling;
	}
	if (!bFindPos)
	{
		//追加到末尾
		if (!pTailNode)
		{
			delete pNode;
			return NULL;
		}
		pTailNode->rightSibling = pNode;

	}
	return pNode;
}
template <class ElemType>
//操作结果：把position的节点从parent移除
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::RemoveChild(ChildSiblingTreeNode<ElemType>* parent, int position)
{
	if (!parent)
		return NULL;
	ChildSiblingTreeNode<ElemType>* pNode  = NULL;
	if (position == 0)
	{
		pNode = parent->firstChild;
		if (!pNode)
			return NULL;
		ChildSiblingTreeNode<ElemType>* pRightSibling = pNode->rightSibling;//获得右兄弟
		parent->firstChild = pRightSibling;//双亲节点的第一个孩子为右兄弟
		pNode->rightSibling = NULL;
		return pNode;
	}
	ChildSiblingTreeNode<ElemType>* pScanNode = parent->firstChild;
	int pos = 0;
	while (pScanNode)
	{
		pos++;
		if (pos == position)
		{
			pNode = pScanNode->rightSibling;//此时pScanNode的右兄弟就是要移除的节点
			ChildSiblingTreeNode<ElemType>* pRightSibling = pNode->rightSibling;//获得移除节点的右兄弟
			pScanNode->rightSibling = pRightSibling;//从兄弟链中移除pNode;
			pNode->rightSibling = NULL;
			break;//终止循环
		}
		pScanNode = pScanNode->rightSibling;
	}
	return pNode;

}
template <class ElemType>
//清空已设置删除标记的节点,孩子兄弟树什么都不需要做
void ChildSiblingTree<ElemType>::ClearRemovedNode()
{

}
template <class ElemType>
// 建立以数据元素e为根的树
ChildSiblingTree<ElemType>::ChildSiblingTree(const ElemType& e)
{
	m_pRoot = new ChildSiblingTreeNode<ElemType>;
	m_pRoot->data = e;
}
template <class ElemType>
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::CopyTreeAux(const ChildSiblingTreeNode<ElemType>* r)
// 操作结果：将以r为根的树复制成新的树,返回新树的根
{
	if (r == NULL)
	{	// 复制空二叉树
		return NULL;					// 空二叉树根为空	
	}
	else
	{	// 复制非空二叉树
		ChildSiblingTreeNode<ElemType>* rt = new ChildSiblingTreeNode<ElemType>(r->data);	// 复制根结点
		ChildSiblingTreeNode<ElemType>* subTreeRoot, * cur;
		for (const ChildSiblingTreeNode<ElemType>* child = FirstChild(r); child != NULL;
			child = RightSibling(child))
		{	// 复制子树
			subTreeRoot = CopyTreeAux(child);	// 复制子树
			if (rt->firstChild == NULL)
			{	// subTreeRoot为首孩子
				rt->firstChild = subTreeRoot;	// subTreeRoot为首孩子	
				cur = subTreeRoot;				// cur最新复制的子树的根
			}
			else
			{	// subTreeRoot为cur的右兄弟
				cur->rightSibling = subTreeRoot;// subTreeRoot为右兄弟
				cur = subTreeRoot;				// cur最新复制的子树的根
			}
		}
		return rt;
	}
}

template <class ElemType>
ChildSiblingTree<ElemType>::ChildSiblingTree(const ChildSiblingTree<ElemType>& source)
// 操作结果：由已知树构造新树——复制构造函数模板
{
	m_pRoot = CopyTreeAux(source.GetRoot());
}

template <class ElemType>
ChildSiblingTree<ElemType>& ChildSiblingTree<ElemType>::operator=(const ChildSiblingTree<ElemType>& source)
// 操作结果：由已知树copy复制到当前树——重载赋值运算符
{
	if (&source != this)
	{
		DestroyAux(m_pRoot);							// 销毁树
		m_pRoot = CopyTreeAux(source.GetRoot());
	}
	return *this;
}

template <class ElemType>
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::CreateTreeGAux(ElemType items[], int parents[], int r, int n)
// 操作结果：建立数据元素为items[],对应结点双亲为parents[],根结点位置为r,结点个数为n的树，并返回树的根
{
	if (r >= 0 && r < n)
	{	// r合法
		ChildSiblingTreeNode<ElemType>* rt = new ChildSiblingTreeNode<ElemType>(items[r]);	// 复制根结点
		ChildSiblingTreeNode<ElemType>* subTreeRoot, * cur;

		for (int pos = 0; pos < n; pos++)
		{	// 查找r的孩子
			if (parents[pos] == r)
			{	// 建立以pos为根的子树
				subTreeRoot = CreateTreeGAux(items, parents, pos, n);
				if (rt->firstChild == NULL)
				{	// subTreeRoot为第一棵子树的根
					rt->firstChild = subTreeRoot;		// subTreeRoot为首孩子	
					cur = subTreeRoot;					// cur最新建立的子树的根
				}
				else
				{	// subTreeRoot为cur的右兄弟
					cur->rightSibling = subTreeRoot;	// subTreeRoot为右兄弟
					cur = subTreeRoot;					// cur最新复制的子树的根
				}
			}
		}
		return rt;
	}
	else return NULL;									// r非法，建立空树
}

template <class ElemType>
ChildSiblingTree<ElemType>::ChildSiblingTree(ElemType items[], int parents[], int r, int n)
// 操作结果：建立数据元素为items[],对应结点双亲为parents[],根结点位置为r,结点个数为n的树
{
	m_pRoot = CreateTreeGAux(items, parents, r, n);	// 用辅助函数建立树
}
#endif
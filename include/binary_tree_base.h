#if !defined(_BINARY_TREE_BASE_H_)
#define _BINARY_TREE_BASE_H_
#include "lk_queue.h"
using namespace std;
template <class  ElemType,class NodePtr>
class BinaryTreeBase
{
public:
	BinaryTreeBase();
	~BinaryTreeBase();
	// 二叉树方法声明及重载编译系统默认方法声明:
	virtual NodePtr GetRoot() const = 0;						// 返回二叉树的根
	virtual bool NodeEmpty(NodePtr cur) const = 0;				// 判断结点cur是否为空
	virtual bool GetItem(NodePtr cur, ElemType& e) const = 0;	// 返回结点cur的元素值
	virtual bool SetElem(NodePtr cur, const ElemType& e) = 0;	// 将结点cur的值置为e
	virtual bool Empty() const = 0;								// 判断二叉树是否为空
	virtual NodePtr LeftChild(NodePtr cur) const = 0;		// 返回二叉树中结点cur的左孩子
	virtual NodePtr RightChild(NodePtr cur) const = 0;	// 返回二叉树中结点cur的右孩子
	virtual NodePtr Parent(NodePtr cur) const = 0;		// 返回二叉树中结点cur的双亲
	virtual bool CreateBinaryTree(ElemType pre[], ElemType in[], int n) = 0;//根据先序、中序序列创建二叉树
	virtual void InsertLeftChild(NodePtr cur, const ElemType& e);		// 插入左孩子
	virtual void InsertRightChild(NodePtr cur, const ElemType& e);		// 插入右孩子
	virtual void InOrder(void (*visit)(const ElemType&)) const;	// 二叉树的中序遍历	
	virtual void PreOrder(void (*visit)(const ElemType&)) const;	// 二叉树的先序遍历
	virtual void PostOrder(void (*visit)(const ElemType&)) const;	// 二叉树的后序遍历
	virtual void LevelOrder(void (*visit)(const ElemType&)) const;	// 二叉树的层次遍历
	virtual int NodeCount() const;									// 求二叉树的结点个数
	virtual void DeleteLeftChild(NodePtr cur);						// 删除左子树
	virtual void DeleteRightChild(NodePtr cur);						// 删除右子村
	virtual int	Height() const;										// 求二叉树的高
	void DisplayBTWithTreeShape();//显示二叉树
protected:
	// 辅助函数模板:
	int NodeCountAux(NodePtr r) const;							// 返回二叉树的结点个数
	void DestroyAux(NodePtr r);								// 销毁以r为根二叉树
	void PreOrderAux(NodePtr r, void (*visit)(const ElemType&)) const;	// 先序遍历
	void InOrderAux(NodePtr r, void (*visit)(const ElemType&)) const;		// 中序遍历
	void PostOrderAux(NodePtr r, void (*visit)(const ElemType&)) const;	// 后序遍历
	int HeightAux(NodePtr r) const;							// 返回二叉树的高
	NodePtr CreateBinaryTreeAux(NodePtr pParent,ElemType pre[], ElemType in[],
		int preLeft, int preRight, int inLeft, int inRight,bool bLeft);//根据前序和中序序列创建二叉树
	void DisplayBTWithTreeShapeAux(NodePtr r, int level);//显示二叉树辅助函数
	virtual ElemType NodeElem(NodePtr cur) const = 0;		// 返回指定节点的元素值
	virtual void ReleaseNode(NodePtr& cur) = 0;				// 将结点cur置空,如果是链式存储删除节点内存
	virtual NodePtr CreateChildNode(NodePtr cur, bool bLeft) = 0;//创建cur节点的子节点
};
template <class ElemType,class NodePtr>
BinaryTreeBase<ElemType,NodePtr>::BinaryTreeBase()
{
}
template<class ElemType,class NodePtr>
BinaryTreeBase<ElemType,NodePtr>::~BinaryTreeBase()
{
}


template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::PreOrderAux(NodePtr r, void (*visit)(const ElemType&)) const
// 操作结果：先序遍历以r为根的二叉树
{
	if (!NodeEmpty(r))
	{	// 根非空
		(*visit)(NodeElem(r));			// 访问根结点
		PreOrderAux(LeftChild(r), visit);	// 遍历左子树
		PreOrderAux(RightChild(r), visit);	// 遍历右子树
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::PreOrder(void (*visit)(const ElemType&)) const
// 操作结果：先序遍历二叉树
{
	PreOrderAux(GetRoot(), visit);
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InOrderAux(NodePtr r, void (*visit)(const ElemType&)) const
// 操作结果：中序遍历以r为根的二叉树
{
	if (!NodeEmpty(r))
	{	// 根非空
		InOrderAux(LeftChild(r), visit);		// 遍历左子树
		(*visit)(NodeElem(r));					// 访问根结点
		InOrderAux(RightChild(r), visit);		// 遍历右子树
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InOrder(void (*visit)(const ElemType&)) const
// 操作结果：中序遍历二叉树
{
	InOrderAux(GetRoot(), visit);
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::PostOrderAux(NodePtr r, void (*visit)(const ElemType&)) const
// 操作结果：后序遍历以r为根的二叉树
{
	if (!NodeEmpty(r))
	{	// 根非空
		PostOrderAux(LeftChild(r), visit);	// 遍历左子树
		PostOrderAux(RightChild(r), visit);	// 遍历右子树
		(*visit)(NodeElem(r));				// 访问根结点
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::PostOrder(void (*visit)(const ElemType&)) const
// 操作结果：后序遍历二叉树
{
	PostOrderAux(GetRoot(), visit);
}
template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::LevelOrder(void (*visit)(const ElemType&)) const
// 操作结果：层次遍历二叉树
{
	LinkQueue<NodePtr> q;							// 队列
	NodePtr cur;
	
	// 当前结点
	NodePtr root = GetRoot();
	if (!NodeEmpty(root)) q.InQueue(root);		// 如果根非空，则入队，以便从根结点开始进行层次遍历
	while (!q.IsEmpty())
	{	// q非空,说明还有结点未访问
		q.OutQueue(cur);						// 出队结点为当前访问的结点
		(*visit)(NodeElem(cur));				// 访问elems[cur].data
		if (!NodeEmpty(LeftChild(cur)))			// 左孩子非空
			q.InQueue(LeftChild(cur));			// 左孩子入队
		if (!NodeEmpty(RightChild(cur)))		// 右孩子非空
			q.InQueue(RightChild(cur));			// 右孩子入队
	}
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::HeightAux(NodePtr r) const
// 操作结果：返回以r为根的二叉树的高
{
	if (NodeEmpty(r))
	{	// 空二叉树高为0
		return 0;
	}
	else
	{	// 非空二叉树高为左右子树的高的最大值再加1
		int lHeight, rHeight;
		lHeight = HeightAux(LeftChild(r));		// 左子树的高
		rHeight = HeightAux(RightChild(r));	// 右子树的高
		return 1 + (lHeight > rHeight ? lHeight : rHeight);
		// 非空二叉树高为左右子树的高的最大值再加1
	}
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::Height() const
// 操作结果：返回二叉树的高
{
	return HeightAux(GetRoot());
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::NodeCountAux(NodePtr r) const
// 操作结果：返回以r为根的二叉树的结点个数
{
	if (NodeEmpty(r)) return 0;	// 空二叉树结点个数为0
	else return 1 + NodeCountAux(LeftChild(r)) + NodeCountAux(RightChild(r));
	// 非空二叉树结点个为左右子树的结点个数之和再加1
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::NodeCount() const
// 操作结果：返回二叉树的结点个数
{
	return NodeCountAux(GetRoot());
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DeleteLeftChild(NodePtr cur)
// 初始条件：cur非空
// 操作结果：删除cur左子树
{
	if (NodeEmpty(cur))
	{	// cur为空
		return;
	}
	else
	{	// cur非空
		DestroyAux(LeftChild(cur));	// 删除cur左子树
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DeleteRightChild(NodePtr cur)
// 初始条件：cur非空
// 操作结果：删除cur右子树
{
	if (NodeEmpty(cur))
	{	// cur为空
		return;
	}
	else
	{	// cur非空
		DestroyAux(RightChild(cur));	// 删除cur右子树
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DestroyAux(NodePtr r)
// 操作结果：销毁以r的二叉树
{
	if (!NodeEmpty(r))
	{	// r非空,实施销毁
		DestroyAux(LeftChild(r));		// 销毁左子树
		DestroyAux(RightChild(r));		// 销毁右子树
		ReleaseNode(r);				// 将r置空
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InsertLeftChild(NodePtr cur, const ElemType& e)
// 初始条件：cur和cur左孩子非空
// 操作结果：插入e为cur的左孩子，如果cur的左孩子非空，操作作失败
{
	if (NodeEmpty(cur))
	{	// cur为空，返回
		return;
	}
	else if (!NodeEmpty(LeftChild(cur)))
		return;//左孩子不为空，返回
	else
	{	// 插入右孩子
		NodePtr nodeChild = CreateChildNode(cur, true);
		if (!NodeEmpty(nodeChild))
		{	// 创建左孩子成功,且合法
			SetElem(nodeChild, e);
		}
		return;
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InsertRightChild(NodePtr cur, const ElemType& e)
// 初始条件：cur和cur右孩子非空
// 操作结果：插入e为cur的右孩子，如果cur的右孩子非空，操作作失败
{
	if (NodeEmpty(cur))
	{	//  cur为空，返回
		return;
	}
	else if(!NodeEmpty(RightChild(cur)))
		return;//右孩子不为空，返回
	else
	{	// 插入右孩子
		NodePtr nodeChild = CreateChildNode(cur,false);
		if (!NodeEmpty(nodeChild))
		{	// 创建右孩子成功,且合法
			SetElem(nodeChild,e);
		}
		return;
	}
}
template<class ElemType, class NodePtr>
NodePtr BinaryTreeBase<ElemType, NodePtr>::CreateBinaryTreeAux(NodePtr pParent,ElemType pre[], ElemType in[],
	int preLeft, int preRight, int inLeft, int inRight, bool bLeft)
{
	NodePtr r = NULL;
	if (preLeft > preRight || inLeft > inRight)
	{	// 二叉树无结点,空二叉树
		r = NULL;									// 空二叉树根为空
	}
	else
	{	// 二叉树有结点,非空二叉树
		r = CreateChildNode(pParent,bLeft);// 生成根结点
		SetElem(r, pre[preLeft]);//设置根节点的值
		int mid = inLeft;							// mid为pre[preLeft]在in[]中的位置
		while (in[mid] != pre[preLeft])
		{	// 查找pre[preLeft]在in[]中的位置,也就是中序序列中根的位置
			mid++;
		}
		CreateBinaryTreeAux(r,  pre, in, preLeft + 1, preLeft + mid - inLeft, inLeft, mid - 1, true);// 生成左子树
		CreateBinaryTreeAux(r,  pre, in, preLeft + mid - inLeft + 1, preRight, mid + 1,inRight,false);	// 生成右子树
	}
	return r;
}
template<class ElemType, class NodePtr>
//操作结果,显示二叉树辅助函数
void BinaryTreeBase<ElemType, NodePtr>::DisplayBTWithTreeShapeAux(NodePtr r, int level)
{
	if (!NodeEmpty(r))
	{	// 空树不显式，只显式非空树
		DisplayBTWithTreeShapeAux(RightChild(r), level + 1);//显示右子树
		cout << endl;					//显示新行	
		for (int temPos = 0; temPos < level - 1; temPos++)
			cout << " ";				//确保在第level列显示结点
		cout << r->data;				//显示结点
		DisplayBTWithTreeShapeAux(LeftChild(r), level + 1);//显示左子树
	}
}
//操作结果,显示二叉树
template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DisplayBTWithTreeShape()
{
	DisplayBTWithTreeShapeAux(GetRoot(), 1);
	// 树状显示以bt.GetRoot()为根的二叉树
	cout << endl;
}
#endif // !defined(_BINARY_TREE_BASE_H_)

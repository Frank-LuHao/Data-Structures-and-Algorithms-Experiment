#if !defined(_TREE_BASE_H_)
#define _TREE_BASE_H_
#include "lk_queue.h"
using namespace std;
template <class  ElemType, class NodePtr>
class TreeBase
{
public:
	TreeBase();
	~TreeBase();
	virtual NodePtr GetRoot() const = 0;	// 返回树的根
	virtual bool Empty() const = 0;									// 判断树是否为空
	virtual bool NodeEmpty(NodePtr cur) const = 0;				// 判断结点cur是否为空
	virtual bool GetElem(NodePtr cur, ElemType& e) const = 0;	// 用e返回结点元素值
	virtual bool SetElem(NodePtr cur, const ElemType& e) = 0;	// 将结cur的值置为e
	virtual NodePtr FirstChild(NodePtr cur) const = 0;	// 返回树结点cur的第一个孩子
	virtual NodePtr RightSibling(NodePtr cur) const = 0;// 返回树结点cur的右兄弟
	virtual NodePtr Parent(NodePtr cur) const = 0;	// 返回树结点cur的双亲
	virtual void PreOrder(void (*visit)(const ElemType&)) const;		// 树的先序遍历
	virtual void PostOrder(void (*visit)(const ElemType&)) const;		// 树的后序遍历
	virtual void LevelOrder(void (*visit)(const ElemType&)) const;		// 树的层次遍历
	virtual int NodeCount() const;								// 返回树的结点个数
	virtual int NodeDegree(const NodePtr cur) const;			// 返回结点cur的度
	virtual int Degree() const;									// 返回树的度
	virtual bool InsertChild(NodePtr cur, int position, const ElemType& e);	// 将数据元素插入为cur的第position个孩子
	virtual bool DeleteChild(NodePtr cur, int position);// 删除cur的第position个棵子树
	virtual int	Height() const;							// 返回树的高
	virtual void DisplayTWithConcaveShape();					//按凹入表示法显示树
protected:
	// 辅助函数模板:
	void DestroyAux(NodePtr& r);		// 销毁以r为根的树
	void PreOrderAux(const NodePtr r, void (*visit)(const ElemType&)) const;	// 先序遍历辅助
	void PostOrderAux(const NodePtr r, void (*visit)(const ElemType&)) const;	// 后序遍历辅助
	int NodeCountAux(const NodePtr r) const;	// 返回以r为根的树的结点个数
	int HeightAux(const NodePtr r) const;		// 返回以r为根的树的高
	int DegreeAux(const NodePtr r) const;		// 返回以r为根的树的度
	void DeleteAux(NodePtr r);		// 删除以r为根的树
	void DisplayTWithConcaveShapeAux(NodePtr r, int level);
	const NodePtr ParentAux(const NodePtr r,const NodePtr cur) const;	// 返回cur的双亲
	virtual ElemType NodeElem(NodePtr cur) const = 0;		// 返回指定节点的元素值
	virtual void ReleaseNode(NodePtr& cur) = 0;				// 将结点cur置空,如果是链式存储删除节点内存
	virtual NodePtr CreateChildNode(NodePtr cur, int  position,const ElemType &e) = 0;//创建cur节点的子节点
	virtual NodePtr RemoveChild(NodePtr parent, int postion) = 0;//把position的节点总parent移除
	virtual void ClearRemovedNode() = 0;//清空已设置删除标记的节点
};
template <class  ElemType, class NodePtr>
TreeBase<ElemType,NodePtr>::TreeBase()
{
}
template <class  ElemType, class NodePtr>
TreeBase<ElemType, NodePtr>::~TreeBase()
{
}


template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::PreOrderAux(const NodePtr r,
	void (*visit)(const ElemType&)) const
	// 操作结果：按先序依次对以r为根的树的每个元素调用函数(*visit)
{
	if (!NodeEmpty(r))
	{	// r非空
		(*visit)(NodeElem(r));			// 访问根结点
		for (NodePtr child = (NodePtr)FirstChild(r); !NodeEmpty(child);
			child = (NodePtr)RightSibling(child))
		{	// 依次先序遍历每棵子树
			PreOrderAux(child, visit);
		}
	}
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::PreOrder(void (*visit)(const ElemType&)) const
// 操作结果：按先序依次对树的每个元素调用函数(*visit)
{
	PreOrderAux(GetRoot(), visit);				// 调用辅助函数实现树的先序遍历
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::PostOrderAux(const NodePtr r, void (*visit)(const ElemType&)) const
// 操作结果：按后序依次对以r为根的树的每个元素调用函数(*visit)
{
	if (!NodeEmpty(r))
	{	// r非空
		for (NodePtr child = FirstChild(r); !NodeEmpty(child);
			child = RightSibling(child))
		{	// 依次后序遍历每棵子树
			PostOrderAux(child, visit);
		}
		(*visit)(NodeElem(r));					// 访问根结点
	}
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::PostOrder(void (*visit)(const ElemType&)) const
// 操作结果：按后序依次对树的每个元素调用函数(*visit)
{
	PostOrderAux(GetRoot(), visit);			// 调用辅助函数实现后序遍历
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::LevelOrder(void (*visit)(const ElemType&)) const
// 操作结果：层次遍历树
{
	LinkQueue<NodePtr> q;	// 队列
	NodePtr cur = GetRoot();		// 从根结点开始进行层次遍历

	if (!NodeEmpty(cur)) q.InQueue(cur);			// 如果根非空,则入队
	while (!q.IsEmpty())
	{	// q非空,说明还有结点未访问
		q.OutQueue(cur);						// 出队
		(*visit)(NodeElem(cur));					// 访问当前结点
		for (NodePtr child = FirstChild(cur); !NodeEmpty(child);
			child = RightSibling(child))
		{	// 依次将cur的孩子入列
			q.InQueue(child);					// 孩子入队
		}
	}
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::NodeCountAux(const NodePtr r) const
// 操作结果：返回以r为根的树的结点个数
{
	if (NodeEmpty(r)) return 0;			// 空树结点个数为0
	else
	{	// 求子树的结点个数之和
		int subTreeNodeCountSum = 0;	// 子树结点个数之和
		for (NodePtr child = FirstChild(r); !NodeEmpty(child);
			child = RightSibling(child))
		{	// 依次求r的子树结点个数并进行累加
			subTreeNodeCountSum += NodeCountAux(child);// 累加子树结点个数
		}
		return 1 + subTreeNodeCountSum;	// 非空树结点个为子树的结点个数之和再加1
	}
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::NodeCount() const
// 操作结果：返回树的结点个数
{
	return NodeCountAux(GetRoot());			// 用辅助函数实现求树结点个数
}


template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::HeightAux(const NodePtr r) const
// 操作结果：返回以r为根的树的高
{
	if (NodeEmpty(r))
	{	// 空树,高为0
		return 0;
	}
	else
	{
		int maxSubTreeHeight = 0;
		for (NodePtr child = FirstChild(r); !NodeEmpty(child);
			child = RightSibling(child))
		{	// 求各子树高的最大值
			int h = HeightAux(child);			// 临时变量存储高
			maxSubTreeHeight = (maxSubTreeHeight < h) ? h : maxSubTreeHeight;
			// 求条子树高的最大值
		}
		return maxSubTreeHeight + 1;			// 树高为各子树的最大值再加1
	}
}


template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::Height() const
// 操作结果：返回树的高
{
	return HeightAux(GetRoot());					// 调用辅助函数实现返回树的高
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::NodeDegree(const NodePtr cur) const
// 操作结果：返回结点cur的度
{
	int d = 0;					// 临时变量,用于对孩子个数进行计数
	for (NodePtr child = FirstChild(cur); !NodeEmpty(child);
		child = RightSibling(child))
	{	// 找查cur的孩子
		d++;					// 对孩子个数进行计数
	}
	return d;
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::DegreeAux(const NodePtr r) const
// 操作结果：返回以r为根的树的度
{

	int d = 0;					// 临时变量,用于结点的孩子个数
	int maxSubTreeDegree = 0;	// 子树度的最大值
	for (NodePtr child = FirstChild(r); !NodeEmpty(child);
		child = RightSibling(child))
	{	// 找查r的孩子
		d++;					// 对孩子个数进行计数
		int subTreeDegree = DegreeAux(child);	// 子树的度
		maxSubTreeDegree = (maxSubTreeDegree < subTreeDegree) ? subTreeDegree : maxSubTreeDegree;
		// 求子树度最大值
	}
	return (d < maxSubTreeDegree) ? maxSubTreeDegree : d;
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::Degree() const
// 操作结果：返回树的度
{
	return DegreeAux(GetRoot());
}

template <class  ElemType, class NodePtr>
const NodePtr TreeBase<ElemType, NodePtr>::ParentAux(const NodePtr r,
	const NodePtr cur) const
	// 操作结果：返回以r为根的树, 结点cur的双亲
{
	if (NodeEmpty(r)) return 0;					// 空树

	NodePtr child;		// 孩子 
	for (child = FirstChild(r); !NodeEmpty(child);
		child = RightSibling(child))
	{	// 找查r的孩子
		if (child == cur) return r;				// cur双亲为r
	}

	for (child = FirstChild(r); !NodeEmpty(child); child = RightSibling(child))
	{	// 在子树上求双亲
		NodePtr tmp;
		tmp = ParentAux(child, cur);			// 在子树上求cur的双亲	
		if (tmp != NULL) return tmp;			// 双亲在子树上
	}

	return 0;								// 未找到双亲
}

template <class  ElemType, class NodePtr>
NodePtr TreeBase<ElemType, NodePtr>::Parent(const NodePtr cur) const
// 操作结果：返回结点cur的双亲
{
	return ParentAux(GetRoot(), cur);				// 用辅助函数求树中结点的双亲
}

template <class  ElemType, class NodePtr>
bool TreeBase<ElemType, NodePtr>::InsertChild(NodePtr cur, int position, const ElemType& e)
// 操作结果：将数据元素插入为cur的第position个孩子,如果插入成功,则返回true,否则返回false
{
	if (NodeEmpty(cur)) return false;								// cur不存在
	if (position <= 0 || position > NodeDegree(cur) + 1) return false;		// position位置错

	NodePtr newNodePtr = CreateChildNode(cur, position,e);// 新结点
	if (NodeEmpty(newNodePtr))
		return false;
	return true;
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::DeleteAux(NodePtr r)
// 操作结果：删除以r为根的树
{
	if (!NodeEmpty(r))
	{	// r非空
		NodePtr child = (NodePtr)FirstChild(r);				// r的首孩子
		while (!NodeEmpty(child))
		{	// 存在孩子child
			NodePtr nextChild = (NodePtr)RightSibling(child);	// r的下一孩子
			DeleteAux(child);									// 删除以child根的子树
			child = nextChild;									// 以nextChild为r的当前孩子
		}
		ReleaseNode(r);											// 释放r
	}
}

template <class  ElemType, class NodePtr>
bool TreeBase<ElemType, NodePtr>::DeleteChild(NodePtr cur, int position)
// 操作结果：删除cur的第position个棵子树,如果删除成功,则返回true,否则返回false
{
	if (NodeEmpty(cur)) return false;								// cur不存在
	if (position <= 0 && position > NodeDegree(cur)) return false;				// 操作失败
	NodePtr pRemovedNode = RemoveChild(cur, position);//把position的孩子节点从cur节点去除
	if (NodeEmpty(pRemovedNode))
		return false;
	DeleteAux(pRemovedNode);							// 物理上删除第position棵子树
	ClearRemovedNode();									//顺序存储的树，删除已标记为删除的节点
	return true;
}
template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::DisplayTWithConcaveShapeAux(NodePtr r, int level)
// 操作结果：按凹入表示法显示树辅助函数，level为层次数，可设根结点的层次数为1
{
	if (!NodeEmpty(r) && NodeCount()>0)
	{	// 存在结点r,才显示r的信息
		cout << endl;									//显示新行	
		for (int temPos = 0; temPos < level - 1; temPos++)
			cout << " ";								//确保在第level列显示结点
		ElemType e;
		GetElem(r, e);							// 取出结点r的元素值
		cout << e;									// 显示结点元素值
		for (NodePtr child = FirstChild(r);!NodeElem(child); child = RightSibling(child))
		{	// 依次显示各棵子树
			DisplayTWithConcaveShapeAux(child, level + 1);
		}
	}
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::DisplayTWithConcaveShape()
// 操作结果：按凹入表示法显示树
{
	DisplayTWithConcaveShapeAux(GetRoot(), 1);// 调用辅助函数实现按凹入表示法显示树
	cout << endl;									// 换行
}

#endif
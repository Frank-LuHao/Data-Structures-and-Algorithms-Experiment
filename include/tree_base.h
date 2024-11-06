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
	virtual NodePtr GetRoot() const = 0;	// �������ĸ�
	virtual bool Empty() const = 0;									// �ж����Ƿ�Ϊ��
	virtual bool NodeEmpty(NodePtr cur) const = 0;				// �жϽ��cur�Ƿ�Ϊ��
	virtual bool GetElem(NodePtr cur, ElemType& e) const = 0;	// ��e���ؽ��Ԫ��ֵ
	virtual bool SetElem(NodePtr cur, const ElemType& e) = 0;	// ����cur��ֵ��Ϊe
	virtual NodePtr FirstChild(NodePtr cur) const = 0;	// ���������cur�ĵ�һ������
	virtual NodePtr RightSibling(NodePtr cur) const = 0;// ���������cur�����ֵ�
	virtual NodePtr Parent(NodePtr cur) const = 0;	// ���������cur��˫��
	virtual void PreOrder(void (*visit)(const ElemType&)) const;		// �����������
	virtual void PostOrder(void (*visit)(const ElemType&)) const;		// ���ĺ������
	virtual void LevelOrder(void (*visit)(const ElemType&)) const;		// ���Ĳ�α���
	virtual int NodeCount() const;								// �������Ľ�����
	virtual int NodeDegree(const NodePtr cur) const;			// ���ؽ��cur�Ķ�
	virtual int Degree() const;									// �������Ķ�
	virtual bool InsertChild(NodePtr cur, int position, const ElemType& e);	// ������Ԫ�ز���Ϊcur�ĵ�position������
	virtual bool DeleteChild(NodePtr cur, int position);// ɾ��cur�ĵ�position��������
	virtual int	Height() const;							// �������ĸ�
	virtual void DisplayTWithConcaveShape();					//�������ʾ����ʾ��
protected:
	// ��������ģ��:
	void DestroyAux(NodePtr& r);		// ������rΪ������
	void PreOrderAux(const NodePtr r, void (*visit)(const ElemType&)) const;	// �����������
	void PostOrderAux(const NodePtr r, void (*visit)(const ElemType&)) const;	// �����������
	int NodeCountAux(const NodePtr r) const;	// ������rΪ�������Ľ�����
	int HeightAux(const NodePtr r) const;		// ������rΪ�������ĸ�
	int DegreeAux(const NodePtr r) const;		// ������rΪ�������Ķ�
	void DeleteAux(NodePtr r);		// ɾ����rΪ������
	void DisplayTWithConcaveShapeAux(NodePtr r, int level);
	const NodePtr ParentAux(const NodePtr r,const NodePtr cur) const;	// ����cur��˫��
	virtual ElemType NodeElem(NodePtr cur) const = 0;		// ����ָ���ڵ��Ԫ��ֵ
	virtual void ReleaseNode(NodePtr& cur) = 0;				// �����cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
	virtual NodePtr CreateChildNode(NodePtr cur, int  position,const ElemType &e) = 0;//����cur�ڵ���ӽڵ�
	virtual NodePtr RemoveChild(NodePtr parent, int postion) = 0;//��position�Ľڵ���parent�Ƴ�
	virtual void ClearRemovedNode() = 0;//���������ɾ����ǵĽڵ�
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
	// ������������������ζ���rΪ��������ÿ��Ԫ�ص��ú���(*visit)
{
	if (!NodeEmpty(r))
	{	// r�ǿ�
		(*visit)(NodeElem(r));			// ���ʸ����
		for (NodePtr child = (NodePtr)FirstChild(r); !NodeEmpty(child);
			child = (NodePtr)RightSibling(child))
		{	// �����������ÿ������
			PreOrderAux(child, visit);
		}
	}
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::PreOrder(void (*visit)(const ElemType&)) const
// ������������������ζ�����ÿ��Ԫ�ص��ú���(*visit)
{
	PreOrderAux(GetRoot(), visit);				// ���ø�������ʵ�������������
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::PostOrderAux(const NodePtr r, void (*visit)(const ElemType&)) const
// ������������������ζ���rΪ��������ÿ��Ԫ�ص��ú���(*visit)
{
	if (!NodeEmpty(r))
	{	// r�ǿ�
		for (NodePtr child = FirstChild(r); !NodeEmpty(child);
			child = RightSibling(child))
		{	// ���κ������ÿ������
			PostOrderAux(child, visit);
		}
		(*visit)(NodeElem(r));					// ���ʸ����
	}
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::PostOrder(void (*visit)(const ElemType&)) const
// ������������������ζ�����ÿ��Ԫ�ص��ú���(*visit)
{
	PostOrderAux(GetRoot(), visit);			// ���ø�������ʵ�ֺ������
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::LevelOrder(void (*visit)(const ElemType&)) const
// �����������α�����
{
	LinkQueue<NodePtr> q;	// ����
	NodePtr cur = GetRoot();		// �Ӹ���㿪ʼ���в�α���

	if (!NodeEmpty(cur)) q.InQueue(cur);			// ������ǿ�,�����
	while (!q.IsEmpty())
	{	// q�ǿ�,˵�����н��δ����
		q.OutQueue(cur);						// ����
		(*visit)(NodeElem(cur));					// ���ʵ�ǰ���
		for (NodePtr child = FirstChild(cur); !NodeEmpty(child);
			child = RightSibling(child))
		{	// ���ν�cur�ĺ�������
			q.InQueue(child);					// �������
		}
	}
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::NodeCountAux(const NodePtr r) const
// ���������������rΪ�������Ľ�����
{
	if (NodeEmpty(r)) return 0;			// ����������Ϊ0
	else
	{	// �������Ľ�����֮��
		int subTreeNodeCountSum = 0;	// ����������֮��
		for (NodePtr child = FirstChild(r); !NodeEmpty(child);
			child = RightSibling(child))
		{	// ������r�������������������ۼ�
			subTreeNodeCountSum += NodeCountAux(child);// �ۼ�����������
		}
		return 1 + subTreeNodeCountSum;	// �ǿ�������Ϊ�����Ľ�����֮���ټ�1
	}
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::NodeCount() const
// ����������������Ľ�����
{
	return NodeCountAux(GetRoot());			// �ø�������ʵ������������
}


template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::HeightAux(const NodePtr r) const
// ���������������rΪ�������ĸ�
{
	if (NodeEmpty(r))
	{	// ����,��Ϊ0
		return 0;
	}
	else
	{
		int maxSubTreeHeight = 0;
		for (NodePtr child = FirstChild(r); !NodeEmpty(child);
			child = RightSibling(child))
		{	// ��������ߵ����ֵ
			int h = HeightAux(child);			// ��ʱ�����洢��
			maxSubTreeHeight = (maxSubTreeHeight < h) ? h : maxSubTreeHeight;
			// ���������ߵ����ֵ
		}
		return maxSubTreeHeight + 1;			// ����Ϊ�����������ֵ�ټ�1
	}
}


template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::Height() const
// ����������������ĸ�
{
	return HeightAux(GetRoot());					// ���ø�������ʵ�ַ������ĸ�
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::NodeDegree(const NodePtr cur) const
// ������������ؽ��cur�Ķ�
{
	int d = 0;					// ��ʱ����,���ڶԺ��Ӹ������м���
	for (NodePtr child = FirstChild(cur); !NodeEmpty(child);
		child = RightSibling(child))
	{	// �Ҳ�cur�ĺ���
		d++;					// �Ժ��Ӹ������м���
	}
	return d;
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::DegreeAux(const NodePtr r) const
// ���������������rΪ�������Ķ�
{

	int d = 0;					// ��ʱ����,���ڽ��ĺ��Ӹ���
	int maxSubTreeDegree = 0;	// �����ȵ����ֵ
	for (NodePtr child = FirstChild(r); !NodeEmpty(child);
		child = RightSibling(child))
	{	// �Ҳ�r�ĺ���
		d++;					// �Ժ��Ӹ������м���
		int subTreeDegree = DegreeAux(child);	// �����Ķ�
		maxSubTreeDegree = (maxSubTreeDegree < subTreeDegree) ? subTreeDegree : maxSubTreeDegree;
		// �����������ֵ
	}
	return (d < maxSubTreeDegree) ? maxSubTreeDegree : d;
}

template <class  ElemType, class NodePtr>
int TreeBase<ElemType, NodePtr>::Degree() const
// ����������������Ķ�
{
	return DegreeAux(GetRoot());
}

template <class  ElemType, class NodePtr>
const NodePtr TreeBase<ElemType, NodePtr>::ParentAux(const NodePtr r,
	const NodePtr cur) const
	// ���������������rΪ������, ���cur��˫��
{
	if (NodeEmpty(r)) return 0;					// ����

	NodePtr child;		// ���� 
	for (child = FirstChild(r); !NodeEmpty(child);
		child = RightSibling(child))
	{	// �Ҳ�r�ĺ���
		if (child == cur) return r;				// cur˫��Ϊr
	}

	for (child = FirstChild(r); !NodeEmpty(child); child = RightSibling(child))
	{	// ����������˫��
		NodePtr tmp;
		tmp = ParentAux(child, cur);			// ����������cur��˫��	
		if (tmp != NULL) return tmp;			// ˫����������
	}

	return 0;								// δ�ҵ�˫��
}

template <class  ElemType, class NodePtr>
NodePtr TreeBase<ElemType, NodePtr>::Parent(const NodePtr cur) const
// ������������ؽ��cur��˫��
{
	return ParentAux(GetRoot(), cur);				// �ø������������н���˫��
}

template <class  ElemType, class NodePtr>
bool TreeBase<ElemType, NodePtr>::InsertChild(NodePtr cur, int position, const ElemType& e)
// ���������������Ԫ�ز���Ϊcur�ĵ�position������,�������ɹ�,�򷵻�true,���򷵻�false
{
	if (NodeEmpty(cur)) return false;								// cur������
	if (position <= 0 || position > NodeDegree(cur) + 1) return false;		// positionλ�ô�

	NodePtr newNodePtr = CreateChildNode(cur, position,e);// �½��
	if (NodeEmpty(newNodePtr))
		return false;
	return true;
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::DeleteAux(NodePtr r)
// ���������ɾ����rΪ������
{
	if (!NodeEmpty(r))
	{	// r�ǿ�
		NodePtr child = (NodePtr)FirstChild(r);				// r���׺���
		while (!NodeEmpty(child))
		{	// ���ں���child
			NodePtr nextChild = (NodePtr)RightSibling(child);	// r����һ����
			DeleteAux(child);									// ɾ����child��������
			child = nextChild;									// ��nextChildΪr�ĵ�ǰ����
		}
		ReleaseNode(r);											// �ͷ�r
	}
}

template <class  ElemType, class NodePtr>
bool TreeBase<ElemType, NodePtr>::DeleteChild(NodePtr cur, int position)
// ���������ɾ��cur�ĵ�position��������,���ɾ���ɹ�,�򷵻�true,���򷵻�false
{
	if (NodeEmpty(cur)) return false;								// cur������
	if (position <= 0 && position > NodeDegree(cur)) return false;				// ����ʧ��
	NodePtr pRemovedNode = RemoveChild(cur, position);//��position�ĺ��ӽڵ��cur�ڵ�ȥ��
	if (NodeEmpty(pRemovedNode))
		return false;
	DeleteAux(pRemovedNode);							// ������ɾ����position������
	ClearRemovedNode();									//˳��洢������ɾ���ѱ��Ϊɾ���Ľڵ�
	return true;
}
template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::DisplayTWithConcaveShapeAux(NodePtr r, int level)
// ����������������ʾ����ʾ������������levelΪ���������������Ĳ����Ϊ1
{
	if (!NodeEmpty(r) && NodeCount()>0)
	{	// ���ڽ��r,����ʾr����Ϣ
		cout << endl;									//��ʾ����	
		for (int temPos = 0; temPos < level - 1; temPos++)
			cout << " ";								//ȷ���ڵ�level����ʾ���
		ElemType e;
		GetElem(r, e);							// ȡ�����r��Ԫ��ֵ
		cout << e;									// ��ʾ���Ԫ��ֵ
		for (NodePtr child = FirstChild(r);!NodeElem(child); child = RightSibling(child))
		{	// ������ʾ��������
			DisplayTWithConcaveShapeAux(child, level + 1);
		}
	}
}

template <class  ElemType, class NodePtr>
void TreeBase<ElemType, NodePtr>::DisplayTWithConcaveShape()
// ����������������ʾ����ʾ��
{
	DisplayTWithConcaveShapeAux(GetRoot(), 1);// ���ø�������ʵ�ְ������ʾ����ʾ��
	cout << endl;									// ����
}

#endif
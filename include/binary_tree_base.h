#if !defined(_BINARY_TREE_BASE_H_)
#define _BINARY_TREE_BASE_H_
#include "lk_queue.h"
#include <iostream>
using namespace std;
template <class  ElemType,class NodePtr>
class BinaryTreeBase
{
public:
	BinaryTreeBase();
	~BinaryTreeBase();
	// �������������������ر���ϵͳĬ�Ϸ�������:
	virtual NodePtr GetRoot() const = 0;						// ���ض������ĸ�
	virtual bool NodeEmpty(NodePtr cur) const = 0;				// �жϽ��cur�Ƿ�Ϊ��
	virtual bool GetItem(NodePtr cur, ElemType& e) const = 0;	// ���ؽ��cur��Ԫ��ֵ
	virtual bool SetElem(NodePtr cur, const ElemType& e) = 0;	// �����cur��ֵ��Ϊe
	virtual bool Empty() const = 0;								// �ж϶������Ƿ�Ϊ��
	virtual NodePtr &LeftChild(NodePtr cur) const = 0;		// ���ض������н��cur������
	virtual NodePtr &RightChild(NodePtr cur) const = 0;	// ���ض������н��cur���Һ���
	virtual NodePtr Parent(NodePtr cur) const = 0;		// ���ض������н��cur��˫��
	virtual bool CreateBinaryTree(ElemType pre[], ElemType in[], int n) = 0;//���������������д���������
	virtual void InsertLeftChild(NodePtr cur, const ElemType& e);		// ��������
	virtual void InsertRightChild(NodePtr cur, const ElemType& e);		// �����Һ���
	virtual void InOrder(void (*visit)(const ElemType&)) const;	// ���������������	
	virtual void PreOrder(void (*visit)(const ElemType&)) const;	// ���������������
	virtual void PostOrder(void (*visit)(const ElemType&)) const;	// �������ĺ������
	virtual void LevelOrder(void (*visit)(const ElemType&)) const;	// �������Ĳ�α���
	virtual int NodeCount() const;									// ��������Ľ�����
	virtual void DeleteLeftChild(NodePtr cur);						// ɾ��������
	virtual void DeleteRightChild(NodePtr cur);						// ɾ�����Ӵ�
	virtual int	Height() const;										// ��������ĸ�
	void DisplayBTWithTreeShape();//��ʾ������
protected:
	// ��������ģ��:
	int NodeCountAux(NodePtr r) const;							// ���ض������Ľ�����
	virtual void DestroyAux(NodePtr& r);								// ������rΪ��������
	void PreOrderAux(NodePtr r, void (*visit)(const ElemType&)) const;	// �������
	void InOrderAux(NodePtr r, void (*visit)(const ElemType&)) const;		// �������
	void PostOrderAux(NodePtr r, void (*visit)(const ElemType&)) const;	// �������
	int HeightAux(NodePtr r) const;							// ���ض������ĸ�
	NodePtr CreateBinaryTreeAux(NodePtr pParent,ElemType pre[], ElemType in[],
		int preLeft, int preRight, int inLeft, int inRight,bool bLeft);//����ǰ����������д���������
	void DisplayBTWithTreeShapeAux(NodePtr r, int level);//��ʾ��������������
	virtual ElemType NodeElem(NodePtr cur) const = 0;		// ����ָ���ڵ��Ԫ��ֵ
	virtual void ReleaseNode(NodePtr& cur) = 0;				// �����cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
	virtual NodePtr CreateChildNode(NodePtr cur, bool bLeft) = 0;//����cur�ڵ���ӽڵ�
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
// ������������������rΪ���Ķ�����
{
	if (!NodeEmpty(r))
	{	// ���ǿ�
		(*visit)(NodeElem(r));			// ���ʸ����
		PreOrderAux(LeftChild(r), visit);	// ����������
		PreOrderAux(RightChild(r), visit);	// ����������
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::PreOrder(void (*visit)(const ElemType&)) const
// ����������������������
{
	PreOrderAux(GetRoot(), visit);
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InOrderAux(NodePtr r, void (*visit)(const ElemType&)) const
// ������������������rΪ���Ķ�����
{
	if (!NodeEmpty(r))
	{	// ���ǿ�
		InOrderAux(LeftChild(r), visit);		// ����������
		(*visit)(NodeElem(r));					// ���ʸ����
		InOrderAux(RightChild(r), visit);		// ����������
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InOrder(void (*visit)(const ElemType&)) const
// ����������������������
{
	InOrderAux(GetRoot(), visit);
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::PostOrderAux(NodePtr r, void (*visit)(const ElemType&)) const
// ������������������rΪ���Ķ�����
{
	if (!NodeEmpty(r))
	{	// ���ǿ�
		PostOrderAux(LeftChild(r), visit);	// ����������
		PostOrderAux(RightChild(r), visit);	// ����������
		(*visit)(NodeElem(r));				// ���ʸ����
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::PostOrder(void (*visit)(const ElemType&)) const
// ����������������������
{
	PostOrderAux(GetRoot(), visit);
}
template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::LevelOrder(void (*visit)(const ElemType&)) const
// �����������α���������
{
	LinkQueue<NodePtr> q;							// ����
	NodePtr cur;
	
	// ��ǰ���
	NodePtr root = GetRoot();
	if (!NodeEmpty(root)) q.InQueue(root);		// ������ǿգ�����ӣ��Ա�Ӹ���㿪ʼ���в�α���
	while (!q.IsEmpty())
	{	// q�ǿ�,˵�����н��δ����
		q.OutQueue(cur);						// ���ӽ��Ϊ��ǰ���ʵĽ��
		(*visit)(NodeElem(cur));				// ����elems[cur].data
		if (!NodeEmpty(LeftChild(cur)))			// ���ӷǿ�
			q.InQueue(LeftChild(cur));			// �������
		if (!NodeEmpty(RightChild(cur)))		// �Һ��ӷǿ�
			q.InQueue(RightChild(cur));			// �Һ������
	}
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::HeightAux(NodePtr r) const
// ���������������rΪ���Ķ������ĸ�
{
	if (NodeEmpty(r))
	{	// �ն�������Ϊ0
		return 0;
	}
	else
	{	// �ǿն�������Ϊ���������ĸߵ����ֵ�ټ�1
		int lHeight, rHeight;
		lHeight = HeightAux(LeftChild(r));		// �������ĸ�
		rHeight = HeightAux(RightChild(r));	// �������ĸ�
		return 1 + (lHeight > rHeight ? lHeight : rHeight);
		// �ǿն�������Ϊ���������ĸߵ����ֵ�ټ�1
	}
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::Height() const
// ������������ض������ĸ�
{
	return HeightAux(GetRoot());
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::NodeCountAux(NodePtr r) const
// ���������������rΪ���Ķ������Ľ�����
{
	if (NodeEmpty(r)) return 0;	// �ն�����������Ϊ0
	else return 1 + NodeCountAux(LeftChild(r)) + NodeCountAux(RightChild(r));
	// �ǿն���������Ϊ���������Ľ�����֮���ټ�1
}

template<class ElemType, class NodePtr>
int BinaryTreeBase<ElemType, NodePtr>::NodeCount() const
// ������������ض������Ľ�����
{
	return NodeCountAux(GetRoot());
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DeleteLeftChild(NodePtr cur)
// ��ʼ������cur�ǿ�
// ���������ɾ��cur������
{
	if (NodeEmpty(cur))
	{	// curΪ��
		return;
	}
	else
	{	// cur�ǿ�
		DestroyAux(LeftChild(cur));	// ɾ��cur������
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DeleteRightChild(NodePtr cur)
// ��ʼ������cur�ǿ�
// ���������ɾ��cur������
{
	if (NodeEmpty(cur))
	{	// curΪ��
		return;
	}
	else
	{	// cur�ǿ�
		DestroyAux(RightChild(cur));	// ɾ��cur������
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DestroyAux(NodePtr& r)
// ���������������r�Ķ�����
{
	if (!NodeEmpty(r))
	{	// r�ǿ�,ʵʩ����
		DestroyAux(LeftChild(r));		// ����������
		DestroyAux(RightChild(r));		// ����������
		ReleaseNode(r);				// ��r�ÿ�
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InsertLeftChild(NodePtr cur, const ElemType& e)
// ��ʼ������cur��cur���ӷǿ�
// �������������eΪcur�����ӣ����cur�����ӷǿգ�������ʧ��
{
	if (NodeEmpty(cur))
	{	// curΪ�գ�����
		return;
	}
	else if (!NodeEmpty(LeftChild(cur)))
		return;//���Ӳ�Ϊ�գ�����
	else
	{	// �����Һ���
		NodePtr nodeChild = CreateChildNode(cur, true);
		if (!NodeEmpty(nodeChild))
		{	// �������ӳɹ�,�ҺϷ�
			SetElem(nodeChild, e);
		}
		return;
	}
}

template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::InsertRightChild(NodePtr cur, const ElemType& e)
// ��ʼ������cur��cur�Һ��ӷǿ�
// �������������eΪcur���Һ��ӣ����cur���Һ��ӷǿգ�������ʧ��
{
	if (NodeEmpty(cur))
	{	//  curΪ�գ�����
		return;
	}
	else if(!NodeEmpty(RightChild(cur)))
		return;//�Һ��Ӳ�Ϊ�գ�����
	else
	{	// �����Һ���
		NodePtr nodeChild = CreateChildNode(cur,false);
		if (!NodeEmpty(nodeChild))
		{	// �����Һ��ӳɹ�,�ҺϷ�
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
	{	// �������޽��,�ն�����
		r = NULL;									// �ն�������Ϊ��
	}
	else
	{	// �������н��,�ǿն�����
		r = CreateChildNode(pParent,bLeft);// ���ɸ����
		SetElem(r, pre[preLeft]);//���ø��ڵ��ֵ
		int mid = inLeft;							// midΪpre[preLeft]��in[]�е�λ��
		while (in[mid] != pre[preLeft])
		{	// ����pre[preLeft]��in[]�е�λ��,Ҳ�������������и���λ��
			mid++;
		}
		CreateBinaryTreeAux(r,  pre, in, preLeft + 1, preLeft + mid - inLeft, inLeft, mid - 1, true);// ����������
		CreateBinaryTreeAux(r,  pre, in, preLeft + mid - inLeft + 1, preRight, mid + 1,inRight,false);	// ����������
	}
	return r;
}
template<class ElemType, class NodePtr>
//�������,��ʾ��������������
void BinaryTreeBase<ElemType, NodePtr>::DisplayBTWithTreeShapeAux(NodePtr r, int level)
{
	if (!NodeEmpty(r))
	{	// ��������ʽ��ֻ��ʽ�ǿ���
		DisplayBTWithTreeShapeAux(RightChild(r), level + 1);//��ʾ������
		cout << endl;					//��ʾ����	
		for (int temPos = 0; temPos < level - 1; temPos++)
			cout << " ";				//ȷ���ڵ�level����ʾ���
		cout << r->data;				//��ʾ���
		DisplayBTWithTreeShapeAux(LeftChild(r), level + 1);//��ʾ������
	}
}
//�������,��ʾ������
template<class ElemType, class NodePtr>
void BinaryTreeBase<ElemType, NodePtr>::DisplayBTWithTreeShape()
{
	DisplayBTWithTreeShapeAux(GetRoot(), 1);
	// ��״��ʾ��bt.GetRoot()Ϊ���Ķ�����
	cout << endl;
}
#endif // !defined(_BINARY_TREE_BASE_H_)

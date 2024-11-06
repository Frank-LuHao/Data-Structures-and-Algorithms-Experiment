#if !defined(_CHILD_SIBLING_TREE_H_)
#define _CHILD_SIBLING_TREE_H_
#include "tree_base.h"
#include "child_sibling_tree_node.h"
// ���ӡ��ֵܱ�ʾ�������ģ��
template <class ElemType>
class ChildSiblingTree :public TreeBase<ElemType, ChildSiblingTreeNode<ElemType>*>
{
public:
	ChildSiblingTree();
	~ChildSiblingTree();
	ChildSiblingTree(const ElemType& e);// ����������Ԫ��eΪ������
	ChildSiblingTree(const ChildSiblingTree<ElemType>& source);	// ���ƹ��캯��ģ��
	ChildSiblingTree(ElemType items[], int parents[], int r, int n);
	// ��������Ԫ��Ϊitems[]����Ӧ���˫��Ϊparents[]�������λ��Ϊr��������Ϊn����
	ChildSiblingTree<ElemType>& operator=(const ChildSiblingTree<ElemType>& source);
	// ���ظ�ֵ�����
	virtual ChildSiblingTreeNode<ElemType>* GetRoot() const;						// �������ĸ�
	virtual bool Empty() const;									// �ж����Ƿ�Ϊ��
	virtual bool NodeEmpty(ChildSiblingTreeNode<ElemType>* cur) const;					// �жϽ��cur�Ƿ�Ϊ��
	virtual bool GetElem(ChildSiblingTreeNode<ElemType>* cur, ElemType& e) const;	// ��e���ؽ��Ԫ��ֵ
	virtual bool SetElem(ChildSiblingTreeNode<ElemType>* cur, const ElemType& e);		// ����cur��ֵ��Ϊe
	virtual ChildSiblingTreeNode<ElemType>* FirstChild(ChildSiblingTreeNode<ElemType>* cur) const;	// ���������cur�ĵ�һ������
	virtual ChildSiblingTreeNode<ElemType>* RightSibling(ChildSiblingTreeNode<ElemType>* cur) const;// ���������cur�����ֵ�
	virtual ChildSiblingTreeNode<ElemType>* Parent(ChildSiblingTreeNode<ElemType>* cur) const;		// ���������cur��˫��
private:
	virtual ElemType NodeElem(ChildSiblingTreeNode<ElemType>* cur) const;		// ����ָ���ڵ��Ԫ��ֵ
	virtual void ReleaseNode(ChildSiblingTreeNode<ElemType>*& cur);				// �����cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
	virtual ChildSiblingTreeNode<ElemType>* CreateChildNode(ChildSiblingTreeNode<ElemType>* cur, int  position, const ElemType& e);//����cur�ڵ���ӽڵ�
	virtual ChildSiblingTreeNode<ElemType>* RemoveChild(ChildSiblingTreeNode<ElemType>* parent, int position);//��position�Ľڵ��parent�Ƴ�
	virtual void ClearRemovedNode();//���������ɾ����ǵĽڵ�
	ChildSiblingTreeNode<ElemType>* ParentAux(ChildSiblingTreeNode<ElemType>* r,ChildSiblingTreeNode<ElemType>* cur) const;	// ����cur��˫�׸�������
	ChildSiblingTreeNode<ElemType>* CopyTreeAux(const ChildSiblingTreeNode<ElemType>* r);	// ��������������
	ChildSiblingTreeNode<ElemType>* CreateTreeGAux(ElemType items[], int parents[], int r, int n);
	// ��������Ԫ��Ϊitems[],��Ӧ���˫��Ϊparents[],�����λ��Ϊr,������Ϊn��
	// �������������ĸ�
	ChildSiblingTreeNode<ElemType>* m_pRoot;//���ڵ�
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
//�����������ȡ���ڵ�
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::GetRoot() const
{
	return m_pRoot;
}
template <class ElemType>
//����������ж����Ƿ�Ϊ��
bool ChildSiblingTree<ElemType>::Empty() const
{
	if (!m_pRoot)
		return true;
	return false;
}
template <class ElemType>
//����������жϽ��cur�Ƿ�Ϊ��
bool ChildSiblingTree<ElemType>::NodeEmpty(ChildSiblingTreeNode<ElemType>* cur) const
{
	if (!cur)
		return true;
	return false;
}
template <class ElemType>
//�����������e���ؽ��Ԫ��ֵ
bool ChildSiblingTree<ElemType>::GetElem(ChildSiblingTreeNode<ElemType>* cur, ElemType& e) const
{
	if (!cur)
		return false;
	e = cur->data;
	return true;
}
template <class ElemType>
//�������������cur��ֵ��Ϊe
bool ChildSiblingTree<ElemType>::SetElem(ChildSiblingTreeNode<ElemType>* cur, const ElemType& e)
{
	if (!cur)
		return false;
	cur->data = e;
	return true;
}
template <class ElemType>
//������������ؽڵ�cur�ĵ�һ������
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::FirstChild(ChildSiblingTreeNode<ElemType>* cur) const
{
	return cur->firstChild;
}
template <class ElemType>
//������������������cur�����ֵ�
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::RightSibling(ChildSiblingTreeNode<ElemType>* cur) const
{
	return cur->rightSibling;
}
template <class ElemType>
//�������������cur��˫�׸�������
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::ParentAux(ChildSiblingTreeNode<ElemType>* r, ChildSiblingTreeNode<ElemType>* cur) const
{
	if (r == NULL) return NULL;					// �ն�����

	ChildSiblingTreeNode<ElemType>* child;		// ���� 
	for (child = FirstChild(r); child != NULL;
		child = RightSibling(child))
	{	// �Ҳ�r�ĺ���
		if (child == cur) return r;				// cur˫��Ϊr
	}

	for (child = FirstChild(r); child != NULL; child = RightSibling(child))
	{	// ����������˫��
		ChildSiblingTreeNode<ElemType>* tmp;
		tmp = ParentAux(child, cur);			// ����������cur��˫��	
		if (tmp != NULL) return tmp;			// ˫����������
	}

	return NULL;								// δ�ҵ�˫��
}

template <class ElemType>
//������������������cur��˫��
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::Parent(ChildSiblingTreeNode<ElemType>* cur) const
{
	return ParentAux(GetRoot(), cur);				// �ø������������н���˫��
}
template <class ElemType>
//�������������ָ���ڵ��Ԫ��ֵ
ElemType ChildSiblingTree<ElemType>::NodeElem(ChildSiblingTreeNode<ElemType>* cur) const
{
	return cur->data;
}
template <class ElemType>
//��������������cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
void ChildSiblingTree<ElemType>::ReleaseNode(ChildSiblingTreeNode<ElemType>*& cur)
{
	if (cur)
	{
		delete cur;
		cur = NULL;
	}
}
template <class ElemType>
//�������������cur�ڵ���ӽڵ�
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
		{//��positionλ�ò���pNode;
			ChildSiblingTreeNode<ElemType>* pRightSibling = pScanNode->rightSibling;
			pScanNode->rightSibling = pNode;
			pNode->rightSibling = pRightSibling;
			bFindPos = TRUE;
			break;//��ֹѭ��
		}
		if (!pScanNode->rightSibling)
			pTailNode = pScanNode;
		pScanNode = pScanNode->rightSibling;
	}
	if (!bFindPos)
	{
		//׷�ӵ�ĩβ
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
//�����������position�Ľڵ��parent�Ƴ�
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
		ChildSiblingTreeNode<ElemType>* pRightSibling = pNode->rightSibling;//������ֵ�
		parent->firstChild = pRightSibling;//˫�׽ڵ�ĵ�һ������Ϊ���ֵ�
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
			pNode = pScanNode->rightSibling;//��ʱpScanNode�����ֵܾ���Ҫ�Ƴ��Ľڵ�
			ChildSiblingTreeNode<ElemType>* pRightSibling = pNode->rightSibling;//����Ƴ��ڵ�����ֵ�
			pScanNode->rightSibling = pRightSibling;//���ֵ������Ƴ�pNode;
			pNode->rightSibling = NULL;
			break;//��ֹѭ��
		}
		pScanNode = pScanNode->rightSibling;
	}
	return pNode;

}
template <class ElemType>
//���������ɾ����ǵĽڵ�,�����ֵ���ʲô������Ҫ��
void ChildSiblingTree<ElemType>::ClearRemovedNode()
{

}
template <class ElemType>
// ����������Ԫ��eΪ������
ChildSiblingTree<ElemType>::ChildSiblingTree(const ElemType& e)
{
	m_pRoot = new ChildSiblingTreeNode<ElemType>;
	m_pRoot->data = e;
}
template <class ElemType>
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::CopyTreeAux(const ChildSiblingTreeNode<ElemType>* r)
// �������������rΪ���������Ƴ��µ���,���������ĸ�
{
	if (r == NULL)
	{	// ���ƿն�����
		return NULL;					// �ն�������Ϊ��	
	}
	else
	{	// ���Ʒǿն�����
		ChildSiblingTreeNode<ElemType>* rt = new ChildSiblingTreeNode<ElemType>(r->data);	// ���Ƹ����
		ChildSiblingTreeNode<ElemType>* subTreeRoot, * cur;
		for (const ChildSiblingTreeNode<ElemType>* child = FirstChild(r); child != NULL;
			child = RightSibling(child))
		{	// ��������
			subTreeRoot = CopyTreeAux(child);	// ��������
			if (rt->firstChild == NULL)
			{	// subTreeRootΪ�׺���
				rt->firstChild = subTreeRoot;	// subTreeRootΪ�׺���	
				cur = subTreeRoot;				// cur���¸��Ƶ������ĸ�
			}
			else
			{	// subTreeRootΪcur�����ֵ�
				cur->rightSibling = subTreeRoot;// subTreeRootΪ���ֵ�
				cur = subTreeRoot;				// cur���¸��Ƶ������ĸ�
			}
		}
		return rt;
	}
}

template <class ElemType>
ChildSiblingTree<ElemType>::ChildSiblingTree(const ChildSiblingTree<ElemType>& source)
// �������������֪�����������������ƹ��캯��ģ��
{
	m_pRoot = CopyTreeAux(source.GetRoot());
}

template <class ElemType>
ChildSiblingTree<ElemType>& ChildSiblingTree<ElemType>::operator=(const ChildSiblingTree<ElemType>& source)
// �������������֪��copy���Ƶ���ǰ���������ظ�ֵ�����
{
	if (&source != this)
	{
		DestroyAux(m_pRoot);							// ������
		m_pRoot = CopyTreeAux(source.GetRoot());
	}
	return *this;
}

template <class ElemType>
ChildSiblingTreeNode<ElemType>* ChildSiblingTree<ElemType>::CreateTreeGAux(ElemType items[], int parents[], int r, int n)
// �����������������Ԫ��Ϊitems[],��Ӧ���˫��Ϊparents[],�����λ��Ϊr,������Ϊn���������������ĸ�
{
	if (r >= 0 && r < n)
	{	// r�Ϸ�
		ChildSiblingTreeNode<ElemType>* rt = new ChildSiblingTreeNode<ElemType>(items[r]);	// ���Ƹ����
		ChildSiblingTreeNode<ElemType>* subTreeRoot, * cur;

		for (int pos = 0; pos < n; pos++)
		{	// ����r�ĺ���
			if (parents[pos] == r)
			{	// ������posΪ��������
				subTreeRoot = CreateTreeGAux(items, parents, pos, n);
				if (rt->firstChild == NULL)
				{	// subTreeRootΪ��һ�������ĸ�
					rt->firstChild = subTreeRoot;		// subTreeRootΪ�׺���	
					cur = subTreeRoot;					// cur���½����������ĸ�
				}
				else
				{	// subTreeRootΪcur�����ֵ�
					cur->rightSibling = subTreeRoot;	// subTreeRootΪ���ֵ�
					cur = subTreeRoot;					// cur���¸��Ƶ������ĸ�
				}
			}
		}
		return rt;
	}
	else return NULL;									// r�Ƿ�����������
}

template <class ElemType>
ChildSiblingTree<ElemType>::ChildSiblingTree(ElemType items[], int parents[], int r, int n)
// �����������������Ԫ��Ϊitems[],��Ӧ���˫��Ϊparents[],�����λ��Ϊr,������Ϊn����
{
	m_pRoot = CreateTreeGAux(items, parents, r, n);	// �ø�������������
}
#endif
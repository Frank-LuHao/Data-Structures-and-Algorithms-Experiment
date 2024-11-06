#if !defined(_THREAD_BINARY_TREE_H_)
#define _THREAD_BINARY_TREE_H_
#include "lk_binary_tree_base.h"
#include "thread_bin_tree_node.h"
typedef enum
{
	NO_THREAD_ORDER,//û�н�������
	PRE_THREAD_ORDER,//��������
	IN_THREAD_ORDER,//��������
	POST_THREAD_ORDER,//��������
}TREAD_ORDER;
template <class ElemType>
class ThreadBinaryTree :public LkBinaryTreeBase<ElemType, ThreadBinTreeNode<ElemType>>
{
public:
	ThreadBinaryTree();
	~ThreadBinaryTree();
	virtual ThreadBinTreeNode<ElemType>* Parent(ThreadBinTreeNode<ElemType>* pCur) const; // ���ض������н��cur��˫��
	virtual ThreadBinTreeNode<ElemType>* LeftChild(ThreadBinTreeNode<ElemType>* pCur) const; // ���ض������н��cur�����ӣ�����
	virtual ThreadBinTreeNode<ElemType>* RightChild(ThreadBinTreeNode<ElemType>* pCur) const; // ���ض������н��cur���Һ��ӣ�����
	virtual void PreOrder(void (*visit)(const ElemType&)) const;	// ���������������
	virtual void InOrder(void (*visit)(const ElemType&)) const;		// ���������������	
	virtual void PostOrder(void (*visit)(const ElemType&)) const;	// �������ĺ������	
	void PreThread();			// ����������������
	void InThread();			// ����������������
	void PostThread();			// ����������������

protected:
	virtual void LinkParentChild(ThreadBinTreeNode<ElemType>* pParent, ThreadBinTreeNode<ElemType>* pChild, bool bLeft);//����˫�׺ͺ���
	void PreThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre);	// ������������������
	void InThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre);	// ������������curΪ���Ķ�������������
	void PostThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre);// ������������curΪ���Ķ�����

private:
	TREAD_ORDER m_order;//��ǰ����������
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
//�������������pCur��˫�׽ڵ�
ThreadBinTreeNode<ElemType>* ThreadBinaryTree<ElemType>::Parent(ThreadBinTreeNode<ElemType>* pCur) const
{
	return pCur->parent;
}
template <class ElemType>
//�������������˫�׽ڵ�ͺ��ӽڵ�
void ThreadBinaryTree<ElemType>::LinkParentChild(ThreadBinTreeNode<ElemType>* pParent, ThreadBinTreeNode<ElemType>* pChild, bool bLeft)
{
	if (!pParent)
		return;
	if (bLeft)
		pParent->leftChild = pChild;
	else
		pParent->rightChild = pChild;
	pChild->parent = pParent;//����˫��ָ��
}
template <class ElemType>
//�������, ���ض������н��cur������
ThreadBinTreeNode<ElemType>* ThreadBinaryTree<ElemType>::LeftChild(ThreadBinTreeNode<ElemType>* pCur) const
{
	if (!pCur)
		return NULL;
	if (pCur->leftTag == CHILD_PTR)//�ж������Ƿ�Ϊ����
		return pCur->leftChild;//��������
	return NULL;
}
template <class ElemType>
//�������, ���ض������н��cur���Һ���
ThreadBinTreeNode<ElemType>* ThreadBinaryTree<ElemType>::RightChild(ThreadBinTreeNode<ElemType>* pCur) const
{
	if (!pCur)
		return NULL;
	if (pCur->rightTag == CHILD_PTR)//�ж��ұ���Ƿ�Ϊ����
		return pCur->rightChild;//�����Һ���
	return NULL;
}
template <class ElemType>
//�����������������������
void ThreadBinaryTree<ElemType>::PreThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre)
{
	if (cur != NULL)
	{	// �����������ʽ����������
		if (LeftChild(cur) == NULL)
		{	// cur������,������ 
			cur->leftChild = pre;				// curǰ��Ϊpre
			cur->leftTag = THREAD_PTR;			// ������־
		}

		if (pre != NULL && RightChild(pre) == NULL)
		{	// pre���Һ���, ������
			pre->rightChild = cur;				// pre���Ϊcur
			pre->rightTag = THREAD_PTR;			// ������־ 
		}

		pre = cur;								// ������һ���ʱ,curΪ��һ����ǰ��

		if (cur->leftTag == CHILD_PTR)
			PreThreadAux(cur->leftChild, pre);	// ������������
		if (cur->rightTag == CHILD_PTR)
			PreThreadAux(cur->rightChild, pre);// ������������
	}
}
template <class ElemType>
//�������������������������
void ThreadBinaryTree<ElemType>::PreThread()
{
	ThreadBinTreeNode<ElemType>* pre = NULL;	// ��ʼ������ʱǰ��Ϊ��
	PreThreadAux(this->GetRoot(), pre);			// ������������rootΪ���Ķ�����
	if (RightChild(pre) == NULL)				// preΪǰ�����������һ�����
		pre->rightTag = THREAD_PTR;				// �����Һ���,����������
	m_order = PRE_THREAD_ORDER;
}
template <class ElemType>
//��������� ���������������
void ThreadBinaryTree<ElemType>::PreOrder(void (*visit)(const ElemType& )) const
{
	if (m_order == PRE_THREAD_ORDER)
	{//����Ѿ���������������������������������
		if (this->GetRoot() != NULL)
		{
			ThreadBinTreeNode<ElemType>* cur = this->GetRoot();
			// �Ӹ���ʼ����,�����Ϊ���������еĵ�һ�����
			while (cur != NULL)
			{
				(*visit)(cur->data);					// ���ʵ�ǰ���

				if (cur->rightTag == THREAD_PTR)
				{	// ����Ϊ���������Ϊcur->rightChild
					cur = cur->rightChild;
				}
				else
				{	// ����Ϊ����
					if (cur->leftTag == CHILD_PTR)
						cur = cur->leftChild;			// cur������,������Ϊ���
					else
						cur = cur->rightChild;			// cur������,���Һ���Ϊ���
				}
			}
		}
	}
	else
	{//������û���������������
		LkBinaryTreeBase < ElemType, ThreadBinTreeNode<ElemType>>::PreOrder(visit);
	}

}
template <class ElemType>
// ���������������������curΪ���Ķ�����,pre��ʾcur��ǰ��
void ThreadBinaryTree<ElemType>::InThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre)
{
	if (cur != NULL)
	{	// �����������ʽ����������
		if (cur->leftTag == CHILD_PTR)
			InThreadAux(cur->leftChild, pre);	// ������������

		if (LeftChild(cur) == NULL)
		{	// cur������,������ 
			cur->leftChild = pre;				// curǰ��Ϊpre
			cur->leftTag = THREAD_PTR;			// ������־
		}
		else
		{	// cur������, �޸ı�־ 
			cur->leftTag = CHILD_PTR;			// ����ָ���־
		}

		if (pre != NULL && RightChild(pre) == NULL)
		{	// pre���Һ���, ������
			pre->rightChild = cur;				// pre���Ϊcur
			pre->rightTag = THREAD_PTR;			// ������־ 
		}
		else if (pre != NULL)
		{	// cur���Һ���, �޸ı�־ 
			pre->rightTag = CHILD_PTR;			// ����ָ���־
		}
		pre = cur;								// ������һ���ʱ,curΪ��һ����ǰ��

		if (cur->rightTag == CHILD_PTR)
			InThreadAux(cur->rightChild, pre);	// ������������
	}
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::InThread()
// �������������������������
{
	ThreadBinTreeNode<ElemType>* pre = NULL;	// ��ʼ������ʱǰ��Ϊ��
	InThreadAux(this->GetRoot(), pre);					// ������������rootΪ���Ķ�����
	if (RightChild(pre) == NULL)				// preΪ�������������һ�����
		pre->rightTag = THREAD_PTR;				// �����Һ���,����������
	m_order = IN_THREAD_ORDER;					//��ǰ������Ϊ����������
}

template <class ElemType>
// ������������������������	
void ThreadBinaryTree<ElemType>::InOrder(void (*visit)(const ElemType&)) const
{
	if (m_order == IN_THREAD_ORDER)
	{//����Ѿ���������������������������������
		if (this->GetRoot() != NULL)
		{
			ThreadBinTreeNode<ElemType>* cur = this->GetRoot();	// �Ӹ���ʼ����

			while (cur->leftTag == CHILD_PTR)			// ���������Ľ�㣬�˽� 
				cur = cur->leftChild;					// ��Ϊ�������еĵ�һ�����
			while (cur != NULL)
			{
				(*visit)(cur->data);					// ���ʵ�ǰ���

				if (cur->rightTag == THREAD_PTR)
				{	// ����Ϊ���������Ϊcur->rightChild
					cur = cur->rightChild;
				}
				else
				{	// ����Ϊ���ӣ�cur�����������Ľ��Ϊ���
					cur = cur->rightChild;				// curָ���Һ���	
					while (cur->leftTag == CHILD_PTR)
						cur = cur->leftChild;			// ����ԭcur�����������Ľ��
				}
			}
		}
	}
	else
	{	//������û���������������
		LkBinaryTreeBase < ElemType, ThreadBinTreeNode<ElemType>>::InOrder(visit);
	}
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::PostThreadAux(ThreadBinTreeNode<ElemType>* cur, ThreadBinTreeNode<ElemType>*& pre)
// ���������������������curΪ���Ķ�����,pre��ʾcur��ǰ��
{
	if (cur != NULL)
	{	// �����������ʽ����������
		if (cur->leftTag == CHILD_PTR)
			PostThreadAux(cur->leftChild, pre);// ������������
		if (cur->rightTag == CHILD_PTR)
			PostThreadAux(cur->rightChild, pre);// ������������

		if (LeftChild(cur) == NULL)
		{	// cur������,������ 
			cur->leftChild = pre;				// curǰ��Ϊpre
			cur->leftTag = THREAD_PTR;			// ������־
		}

		if (pre != NULL && RightChild(pre) == NULL)
		{	// pre���Һ���, ������
			pre->rightChild = cur;				// pre���Ϊcur
			pre->rightTag = THREAD_PTR;			// ������־ 
		}

		pre = cur;								// ������һ���ʱ,curΪ��һ����ǰ��
	}
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::PostThread()
// �������������������������
{
	ThreadBinTreeNode<ElemType>* pre = NULL;	// ��ʼ������ʱǰ��Ϊ��
	PostThreadAux(this->GetRoot(), pre);						// ������������rootΪ���Ķ�����
	if (RightChild(pre) == NULL)					// preΪ�������������һ�����
		pre->rightTag = THREAD_PTR;					// �����Һ���,����������
	m_order = POST_THREAD_ORDER;					//��ǰ������Ϊ����������
}

template <class ElemType>
void ThreadBinaryTree<ElemType>::PostOrder(void (*visit)(const ElemType&)) const
// ����������������ĺ������	
{
	if (m_order == POST_THREAD_ORDER)
	{
		if (this->GetRoot() != NULL)
		{
			ThreadBinTreeNode<ElemType>* cur = this->GetRoot();				// �Ӹ���ʼ����
			while (cur->leftTag == CHILD_PTR || cur->rightTag == CHILD_PTR)
			{	// ���������µĽ��,�˽��Ϊ�������е�һ�����
				if (cur->leftTag == CHILD_PTR) cur = cur->leftChild;	// ��������
				else cur = cur->rightChild;								// ������,�������Һ���
			}

			while (cur != NULL)
			{
				(*visit)(cur->data);									// ���ʵ�ǰ���

				ThreadBinTreeNode<ElemType>* pt = cur->parent;		// ��ǰ����˫�� 
				if (cur->rightTag == THREAD_PTR)
				{	// ����Ϊ����, ���Ϊcur->rightChild
					cur = cur->rightChild;
				}
				else if (cur == this->GetRoot())
				{	// ���cur�Ƕ������ĸ�������Ϊ��
					cur = NULL;
				}
				else if (pt->rightChild == cur || pt->leftChild == cur && pt->rightTag == THREAD_PTR)
				{	// ���cur����˫�׵��Һ��ӻ�����˫�׵���������˫��û������������
					// ���̼�Ϊ˫�׽��
					cur = pt;
				}
				else
				{	// ���cur����˫�׵����ӣ�����˫������������������Ϊ˫�׵��������а���
					// �������ĵ�һ�����ʵĽ�㣬����˫�׵��������������µĽ��
					cur = pt->rightChild;								// curָ��˫�׵��Һ���
					while (cur->leftTag == CHILD_PTR || cur->rightTag == CHILD_PTR)
					{	// ���������µĽ��,�˽��Ϊ�������е�һ�����
						if (cur->leftTag == CHILD_PTR) cur = cur->leftChild;	// ��������
						else cur = cur->rightChild;						// ������,�������Һ���
					}
				}
			}
		}
	}
	else
	{	//������û���ĺ����������
		LkBinaryTreeBase < ElemType, ThreadBinTreeNode<ElemType>>::PostOrder(visit);
	}

}
#endif
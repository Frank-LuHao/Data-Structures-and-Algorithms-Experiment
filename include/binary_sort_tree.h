#if !defined(_BINARY_SORT_TREE_H_)
#define _BINARY_SORT_TREE_H_
#include "binary_tree.h"
template <class ElemType ,class KeyType>
class BinarySortTree:public BinaryTree<ElemType>
{
public:
	BinarySortTree();//���캯��
	~BinarySortTree();//��������
	BinTreeNode<ElemType>* Search(const KeyType& key) const;// ����ֵΪkey������Ԫ��
	bool Insert(const ElemType& e);							// ��������Ԫ��e
	bool Delete(const KeyType& key);						// ɾ���ؼ���Ϊkey������Ԫ��
protected:
	BinTreeNode<ElemType>* SearchAux(const KeyType& key, BinTreeNode<ElemType>*& parent) const;		// ����Ԫ�ص�ֵΪkey������Ԫ��
};
template <class ElemType, class KeyType>
BinarySortTree<ElemType,KeyType>::BinarySortTree()
{
}
template <class ElemType, class KeyType>
BinarySortTree<ElemType, KeyType>::~BinarySortTree()
{
}

template <class ElemType, class KeyType>
BinTreeNode<ElemType>* BinarySortTree<ElemType, KeyType>::SearchAux(const KeyType& key,
	BinTreeNode<ElemType>*& parent) const
// �������: ����ָ��Ԫ�ص�ֵΪkey������Ԫ�ص�ָ�룬��parent������˫��
{
	BinTreeNode< ElemType>* p = this->GetRoot();			// ָ������
	parent = NULL;											// ָ��p��˫��

	while (p != NULL && p->data != key)
	{	// ����Ԫ�ص�ֵΪkey�Ľ��
		if (key < p->data)
		{	// key��С�����������Ͻ��в���
			parent = p;
			p = p->leftChild;
		}
		else
		{	// key�������������Ͻ��в���
			parent = p;
			p = p->rightChild;
		}
	}

	return p;
}
template <class ElemType, class KeyType>
BinTreeNode<ElemType>* BinarySortTree<ElemType, KeyType>::Search(const KeyType& key) const
// �������: ����ָ��ֵΪΪkey������Ԫ�ص�ָ��
{
	BinTreeNode<ElemType>* parent;								// ָ�򱻲��ҽ���˫��
	return SearchAux(key, parent);
}

template <class ElemType, class KeyType>
bool BinarySortTree<ElemType, KeyType>::Insert(const ElemType& e)
// �������: ��������Ԫ��e
{
	BinTreeNode<ElemType>* parent;								// ָ�򱻲��ҽ���˫��

	if (SearchAux(e, parent) == NULL)
	{	// ����ʧ�ܣ�����ɹ�
		BinTreeNode<ElemType>* p;							// ������½��
		p = new BinTreeNode<ElemType>(e);
		if (this->Empty())
		{	// �ն��������½��Ϊ�����
			this->m_pRoot = p;
		}
		else if (e < parent->data)
		{	// e��С��������Ϊparent������
			parent->leftChild = p;
		}
		else
		{	// e���󣬲�����Ϊparent���Һ���
			parent->rightChild = p;
		}

		return true;
	}
	// ���ҳɹ�������ʧ��
	return false;
}

template <class ElemType, class KeyType>
bool BinarySortTree<ElemType, KeyType>::Delete(const KeyType& key)
// �������: ɾ���ؼ���Ϊkey������Ԫ��
{
	BinTreeNode<ElemType>* p, * parent,*tmp;//pΪ��ɾ���ڵ㣬parentΪ��˫�ף�tmpΪ��ʱ����
	p = SearchAux(key, parent);				//���ҹؼ���key�Ľڵ㼰��˫��
	if (p == NULL)
		return false;					// ����ʧ�ܣ�ɾ��ʧ��
	else
	{
		if (p->leftChild != NULL && p->rightChild != NULL)
		{
			// �ؼ���Ϊkey�Ľ�������Һ���	
			parent = p;  tmp = p->rightChild;		// tmp��ָ������������Сֵ��parent����˫��
			while (tmp->leftChild != NULL)
			{	// ������������Сֵ(������)
				parent = tmp;  tmp = tmp->leftChild;
			}
			p->data = tmp->data; 		// ��������С���tmp�����滻p������
			p = tmp;					// tmp��Ϊ�µı�ɾ��㣬pָ��tmp
		}
		// ��ʱpֻ��һ�����ӻ�p��Ҷ��
		BinTreeNode<ElemType>** pp = NULL;//ppΪp˫��ָ��p��ָ���ַ,��ʼ��ʱ��ַָ��NULL,��ʾp��˫��
		if (parent)
		{//��ɾ���ڵ���˫��
			pp =(parent->leftChild == p) ?  &parent->leftChild : &parent->rightChild;//ppָ���󡢻��Һ��ӵĵ�ַ
			if (p->leftChild != NULL) *pp = p->leftChild;//˫�׽ڵ㺢��ָ��p������
			else if (p->rightChild != NULL) *pp = p->rightChild;//˫�׽ڵ㺢��ָ��p���Һ���
			else *pp = NULL;//pΪҶ�ӽڵ㣬˫�׽ڵ㺢��ָ���
		}
		if (p == this->m_pRoot)
			this->m_pRoot = NULL;//�����ڵ㸳��
		delete p;//ɾ���ڵ�p;
		return true;
	}
}
#endif // !defined(_BINARY_SORT_TREE_H_)

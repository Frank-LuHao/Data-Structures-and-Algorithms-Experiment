#if !defined(_BIN_AVL_TREE_H_)
#define _BIN_AVL_TREE_H_
#include "bin_avl_tree_node.h"
#include "lk_binary_tree_base.h"
#include "lk_stack.h"
template <class ElemType, class KeyType>
class BinaryAVLTree:public LkBinaryTreeBase<ElemType, BinAVLTreeNode<ElemType>>
{
public:
	BinaryAVLTree();
	~BinaryAVLTree();
	virtual BinAVLTreeNode<ElemType>* Parent(BinAVLTreeNode<ElemType>* pCur) const;		// ���ض������н��cur��˫��
	BinAVLTreeNode<ElemType>* Search(const KeyType& key) const;							//���ҹؼ���
	bool Insert(const ElemType& e);							// ��������Ԫ��e
	bool Delete(const KeyType& key);						// ɾ���ؼ���Ϊkey������Ԫ��
protected:
	virtual void LinkParentChild(BinAVLTreeNode<ElemType>* pParent, BinAVLTreeNode<ElemType>* pChild, bool bLeft);
	BinAVLTreeNode<ElemType>* ParentAux(BinAVLTreeNode<ElemType>* r, BinAVLTreeNode<ElemType>* cur) const;			// ����cur��˫��
	void DeleteAux(const KeyType& key, BinAVLTreeNode<ElemType>*& p, LinkStack< BinAVLTreeNode<ElemType>*>& s);	// ɾ��pָ��Ľ��
	BinAVLTreeNode<ElemType>* SearchAux(const KeyType& key, BinAVLTreeNode<ElemType>*& parent) const;//���Ҹ�������
	BinAVLTreeNode<ElemType>* SearchAux(const KeyType& key, BinAVLTreeNode<ElemType>*& f,
		LinkStack<BinAVLTreeNode<ElemType>*>& s); // ����ָ��Ԫ��ֵΪkey��Ԫ�ص�ָ�룬��f������˫��
	void LeftRotate(BinAVLTreeNode<ElemType>*& subRoot);	// ����subRootΪ���Ķ�����������
	void RightRotate(BinAVLTreeNode<ElemType>*& subRoot);	// ����subRootΪ���Ķ���������������
	void InsertLeftBalance(BinAVLTreeNode<ElemType>*& subRoot);	// ����subRootΪ���Ķ���������ʱ����ƽ�⴦��
	void InsertRightBalance(BinAVLTreeNode<ElemType>*& subRoot);// ����subRootΪ���Ķ���������ʱ����ƽ�⴦��
	void InsertBalance(const ElemType& e, LinkStack< BinAVLTreeNode<ElemType>*>& s);// �Ӳ�����e���ݲ���·�����л��ݣ�����ƽ�⴦��
	void DeleteLeftBalance(BinAVLTreeNode<ElemType>*& subRoot, bool& isShorter);// ����subRootΪ���Ķ�����ɾ��ʱ����ƽ�⴦��
	void DeleteRightBalance(BinAVLTreeNode<ElemType>*& subRoot, bool& isShorter);// ����subRootΪ���Ķ�����ɾ��ʱ����ƽ�⴦��
	void DeleteBalance(const KeyType& key, LinkStack<BinAVLTreeNode<ElemType>*>& s);// ��ɾ�������ݲ���·�����л��ݣ�����ƽ�⴦��
};
template <class ElemType, class KeyType>
BinaryAVLTree<ElemType,KeyType>::BinaryAVLTree()
{
}
template <class ElemType, class KeyType>
BinaryAVLTree<ElemType, KeyType>::~BinaryAVLTree()
{
}
template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::LinkParentChild(BinAVLTreeNode<ElemType>* pParent, BinAVLTreeNode<ElemType>* pChild, bool bLeft)
{
	if (!pParent)
		return;
	if (bLeft)
		pParent->leftChild = pChild;
	else
		pParent->rightChild = pChild;
}
template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::ParentAux(BinAVLTreeNode<ElemType>* r, BinAVLTreeNode<ElemType>* cur) const
// ���������������rΪ���Ķ�����, ���cur��˫��
{
	if (r == NULL) return NULL;		// �ն�����
	else if (r->leftChild == cur || r->rightChild == cur) return r; // rΪcur��˫��
	else
	{	// ����������˫��
		BinAVLTreeNode<ElemType>* temPtr;	// ��ʱָ��

		temPtr = ParentAux(r->leftChild, cur);	// ������������cur��˫��	
		if (temPtr != NULL) return temPtr;		// ˫������������

		temPtr = ParentAux(r->rightChild, cur);// ������������cur��˫��	
		if (temPtr != NULL) return temPtr;		// ˫������������
		else return NULL;						// ��ʾcur��˫��
	}
}
template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::Parent(BinAVLTreeNode<ElemType>* pCur) const
{
	return ParentAux(this->GetRoot(), pCur);
}

template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::SearchAux(const KeyType& key,	BinAVLTreeNode<ElemType>*& parent) const
	// �������: ����ָ��Ԫ�ص�ֵΪkey������Ԫ�ص�ָ�룬��parent������˫��
{
	BinAVLTreeNode< ElemType>* p = this->GetRoot();			// ָ������
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
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::SearchAux(
	const KeyType& key, BinAVLTreeNode<ElemType>*& f,
	LinkStack<BinAVLTreeNode<ElemType>*>& s)
	// �������: ����ָ��ؼ���Ϊkey��Ԫ�ص�ָ��,��f������˫��
{
	BinAVLTreeNode<ElemType>* p = this->GetRoot();	// ָ��ǰ���
	f = NULL;		// ָ��p��˫��
	while (p != NULL && p->data != key)
	{	// ��Ѱ�ؼ���Ϊkey�Ľ��
		if (key < p->data)
		{	// key��С�����������Ͻ��в���
			f = p;
			s.Push(p);
			p = p->leftChild;
		}
		else
		{	// key�������������Ͻ��в���
			f = p;
			s.Push(p);
			p = p->rightChild;
		}
	}
	return p;
}


template <class ElemType, class KeyType>
BinAVLTreeNode<ElemType>* BinaryAVLTree<ElemType, KeyType>::Search(const KeyType& key) const
// �������: ����ָ��ֵΪΪkey������Ԫ�ص�ָ��
{
	BinAVLTreeNode<ElemType>* parent;								// ָ�򱻲��ҽ���˫��
	return SearchAux(key, parent);
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::LeftRotate(BinAVLTreeNode<ElemType>*& subRoot)
// �������: ����subRootΪ���Ķ�������������������֮��subRootָ���µ�������㣬Ҳ����
//	��ת����ǰ���������ĸ����
{
	BinAVLTreeNode<ElemType>* ptrRChild;		// ��������
	ptrRChild = subRoot->rightChild;			// ptrRChildָ��subRoot�Һ���
	subRoot->rightChild = ptrRChild->leftChild;	// ptrRChild������������ΪsubRoot��������
	ptrRChild->leftChild = subRoot;				// subRoot����ΪptrRChild������
	subRoot = ptrRChild;						// subRootָ���µĸ����
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::RightRotate(BinAVLTreeNode<ElemType>*& subRoot)
// �������: ����subRootΪ���Ķ�������������������֮��subRootָ���µ�������㣬Ҳ����
//	��ת����ǰ���������ĸ����
{
	BinAVLTreeNode<ElemType>* ptrLChild;		// ��������
	ptrLChild = subRoot->leftChild;				// ptrChildָ��subRoot����
	subRoot->leftChild = ptrLChild->rightChild;	// ptrLChild������������ΪsubRoot��������
	ptrLChild->rightChild = subRoot;			// subRoot����ΪptrLChild���Һ���
	subRoot = ptrLChild;						// subRootָ���µĸ����
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::InsertLeftBalance(BinAVLTreeNode<ElemType>*& subRoot)
// �������: ����subRootΪ���Ķ���������ʱ����ƽ�⴦��,��������subRoot���������ϣ������
//	subRootָ���µ��������
{
	BinAVLTreeNode<ElemType>* ptrLChild, * ptrLRChild;// ��������	
	ptrLChild = subRoot->leftChild;	// ptrLChildָ��subRoot����
	switch (ptrLChild->bf)
	{	// ����subRoot����������ƽ����������Ӧ��ƽ�⴦��
	case LH:							// ��������subRoot�����ӵ��������ϣ�������������
		subRoot->bf = ptrLChild->bf = EH;// ƽ�����Ӷ�Ϊ0
		RightRotate(subRoot);			// ����
		break;
	case RH:							// ��������subRoot�����ӵ��������ϣ�������������������
		ptrLRChild = ptrLChild->rightChild;// ptrLRChildָ��subRoot�����ӵ��������ĸ�
		switch (ptrLRChild->bf)
		{	// �޸�subRoot�����ӵ�ƽ������
		case LH:						// ��������ptrLRChild����������
			subRoot->bf = RH;
			ptrLChild->bf = EH;
			break;
		case EH:						// ����ǰptrLRChildΪ��,�����trLRChildָ�������
			subRoot->bf = ptrLChild->bf = EH;
			break;
		case RH:						// ��������ptrLRChild����������
			subRoot->bf = EH;
			ptrLChild->bf = LH;
			break;
		}
		ptrLRChild->bf = 0;
		LeftRotate(subRoot->leftChild);	// ��subRoot����������������
		RightRotate(subRoot);			// ��subRoot����������
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::InsertRightBalance(BinAVLTreeNode<ElemType>*& subRoot)
// �������: ����subRootΪ���Ķ���������ʱ����ƽ�⴦��,��������subRoot����������,�����
//	subRootָ���µ��������
{
	BinAVLTreeNode<ElemType>* ptrRChild, * ptrRLChild;	// ��������
	ptrRChild = subRoot->rightChild;	// ptrRChildָ��subRoot�Һ���
	switch (ptrRChild->bf)
	{	// ����subRoot����������ƽ����������Ӧ��ƽ�⴦��
	case RH:							// ��������subRoot���Һ��ӵ���������,������������
		subRoot->bf = ptrRChild->bf = EH;// ƽ�����Ӷ�Ϊ0
		LeftRotate(subRoot);			// ����
		break;
	case LH:							// ��������subRoot���Һ��ӵ���������,������������������
		ptrRLChild = ptrRChild->leftChild;// ptrRLChildָ��subRoot���Һ��ӵ��������ĸ�
		switch (ptrRLChild->bf)
		{	// �޸�subRoot���Һ��ӵ�ƽ������
		case RH:						// ��������ptrRLChild����������
			subRoot->bf = LH;
			ptrRChild->bf = EH;
			break;
		case EH:						// ����ǰptrRLChildΪ��,�����ptrRLChildָ�������
			subRoot->bf = ptrRChild->bf = EH;
			break;
		case LH:						// ��������ptrRLChild����������
			subRoot->bf = EH;
			ptrRChild->bf = RH;
			break;
		}
		ptrRLChild->bf = 0;
		RightRotate(subRoot->rightChild);// ��subRoot����������������
		LeftRotate(subRoot);			// ��subRoot����������
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::InsertBalance(const ElemType& e,
	LinkStack<BinAVLTreeNode<ElemType>*>& s)
	// �������: �Ӳ���Ԫ��e���ݲ���·�����л��ݣ�����ƽ�⴦��
{
	bool isTaller = true;
	while (!s.Empty() && isTaller)
	{
		BinAVLTreeNode<ElemType>* ptrCurNode, * ptrParent;
		s.Pop(ptrCurNode);			// ȡ����ƽ��Ľ��
		if (s.Empty())
		{	// ptrCurNode��Ϊ����㣬ptrParentΪ��
			ptrParent = NULL;
		}
		else
		{	// ptrCurNode��Ϊ����㣬ȡ��˫��ptrParent
			s.Top(ptrParent);
		}

		if (e < ptrCurNode->data)
		{	// e������ptrCurNode����������
			switch (ptrCurNode->bf)
			{	// ���ptrCurNode��ƽ������
			case LH:						// �����ptrCurNode->bf=2, ����ƽ�⴦��
				if (ptrParent == NULL)
				{	// �ѻ��ݵ������
					InsertLeftBalance(ptrCurNode);
					this->m_pRoot = ptrCurNode;		// ת����ptrCurNodeΪ�¸�
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent��������ƽ�⴦��
					InsertLeftBalance(ptrParent->leftChild);
				}
				else
				{	// ptrParent��������ƽ�⴦��
					InsertLeftBalance(ptrParent->rightChild);
				}
				isTaller = false;
				break;
			case EH:					// �����ptrCurNode->bf=LH
				ptrCurNode->bf = LH;
				break;
			case RH:					// �����ptrCurNode->bf=EH
				ptrCurNode->bf = EH;
				isTaller = false;
				break;
			}
		}
		else
		{	// e������ptrCurNode����������
			switch (ptrCurNode->bf)
			{	// ���ptrCurNode��ƽ���
			case RH:						// �����ptrCurNode->bf=-2, ����ƽ�⴦��
				if (ptrParent == NULL)
				{	// �ѻ��ݵ������
					InsertRightBalance(ptrCurNode);
					this->m_pRoot = ptrCurNode;		// ת����ptrCurNodeΪ�¸�
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent��������ƽ�⴦��
					InsertRightBalance(ptrParent->leftChild);
				}
				else
				{	// ptrParent��������ƽ�⴦��
					InsertRightBalance(ptrParent->rightChild);
				}
				isTaller = false;
				break;
			case EH:						// �����ptrCurNode->bf=RH
				ptrCurNode->bf = RH;
				break;
			case LH:						// �����ptrCurNode->bf=EH
				ptrCurNode->bf = EH;
				isTaller = false;
				break;
			}
		}
	}
}

template <class ElemType, class KeyType>
bool BinaryAVLTree<ElemType, KeyType>::Insert(const ElemType& e)
// �������: ��������Ԫ��e
{
	BinAVLTreeNode<ElemType>* f;
	LinkStack< BinAVLTreeNode<ElemType>*> s;
	if (SearchAux(e, f, s) == NULL)
	{	// ����ʧ��, ����ɹ�
		BinAVLTreeNode<ElemType>* p;		// ������½��
		p = new BinAVLTreeNode<ElemType>(e);// ���ɲ�����
		p->bf = 0;
		if (this->Empty())
		{	// �ն��������½��Ϊ�����
			this->m_pRoot = p;
		}
		else if (e < f->data)
		{	// e��С��������Ϊf������
			f->leftChild = p;
		}
		else
		{	// e���󣬲�����Ϊf���Һ���
			f->rightChild = p;
		}

		InsertBalance(e, s);// ���������ƽ�⴦��
		return true;
	}
	else
	{	// ���ҳɹ�������ʧ��
		return false;
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteLeftBalance(BinAVLTreeNode<ElemType>*&
	subRoot, bool& isShorter)
	// �������: ����subRootΪ���Ķ�����ɾ��ʱ����ƽ�⴦��ɾ��subRoot���������ϵĽ�㣬��
	//	���subRootָ���µ��������
{
	BinAVLTreeNode<ElemType>* ptrRChild, * ptrRLChild;	// ��������
	ptrRChild = subRoot->rightChild;		// ptrRChildָ��subRoot�Һ���
	switch (ptrRChild->bf)
	{	// ����subRoot����������ƽ����������Ӧ��ƽ�⴦��
	case RH:								// �Ҹߣ���������ת
		subRoot->bf = ptrRChild->bf = EH;	// ƽ�����Ӷ�Ϊ0
		LeftRotate(subRoot);				// ����
		isShorter = true;
		break;
	case EH:								// �ȸߣ���������ת
		subRoot->bf = RH;
		ptrRChild->bf = LH;
		LeftRotate(subRoot);				// ����
		isShorter = false;
		break;
	case LH:								// ���,������������
		ptrRLChild = ptrRChild->leftChild;	// ptrRLChildָ��subRoot���Һ��ӵ��������ĸ�
		switch (ptrRLChild->bf)
		{	// �޸�subRoot���Һ��ӵ�ƽ������
		case LH:
			subRoot->bf = EH;
			ptrRChild->bf = RH;
			break;
		case EH:
			subRoot->bf = ptrRChild->bf = EH;
			break;
		case RH:
			subRoot->bf = LH;
			ptrRChild->bf = EH;
			break;
		}
		isShorter = true;
		ptrRLChild->bf = 0;
		RightRotate(subRoot->rightChild);	// ��subRoot����������������
		LeftRotate(subRoot);				// ��subRoot����������
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteRightBalance(BinAVLTreeNode<ElemType>*
	& subRoot, bool& isShorter)
	// �������: ����subRootΪ���Ķ�����ɾ��ʱ����ƽ�⴦��ɾ��subRoot���������ϵĽ�㣬��
	//	���subRootָ���µ��������
{
	BinAVLTreeNode<ElemType>* ptrLChild, * ptrLRChild;
	ptrLChild = subRoot->leftChild;	// ptrLChildָ��subRoot����
	switch (ptrLChild->bf)
	{	// ����subRoot����������ƽ����������Ӧ��ƽ�⴦��
	case LH:							// ��ߣ���������ת
		subRoot->bf = ptrLChild->bf = EH;// ƽ�����Ӷ�Ϊ0
		RightRotate(subRoot);			// ����
		isShorter = true;
		break;
	case EH:							// �ȸߣ���������ת
		subRoot->bf = LH;
		ptrLChild->bf = RH;
		RightRotate(subRoot);			// ����
		isShorter = false;
		break;
	case RH:							// �Ҹߣ�������������
		ptrLRChild = ptrLChild->rightChild;// ptrLRChildָ��subRoot�����ӵ��������ĸ�
		switch (ptrLRChild->bf)
		{	// �޸�subRoot�����ӵ�ƽ������
		case LH:
			subRoot->bf = RH;
			ptrLChild->bf = EH;
			break;
		case EH:
			subRoot->bf = ptrLChild->bf = EH;
			break;
		case RH:
			subRoot->bf = EH;
			ptrLChild->bf = LH;
			break;
		}
		isShorter = true;
		ptrLRChild->bf = 0;
		LeftRotate(subRoot->leftChild);	// ��subRoot����������������
		RightRotate(subRoot);			// ��subRoot����������
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteBalance(const KeyType& key,
	LinkStack<BinAVLTreeNode<ElemType>*>& s)
	// �������: ��ɾ�������ݲ���·�����л��ݣ�����ƽ�⴦��
{
	bool isShorter = true;
	while (!s.Empty() && isShorter)
	{
		BinAVLTreeNode<ElemType>* ptrCurNode, * ptrParent;
		s.Pop(ptrCurNode);					// ȡ����ƽ��Ľ��
		if (s.Empty())
		{	// ptrCurNode��Ϊ����㣬ptrParentΪ��
			ptrParent = NULL;
		}
		else
		{	// ptrCurNode��Ϊ����㣬ȡ��˫��ptrParent
			s.Top(ptrParent);
		}

		if (key < ptrCurNode->data)
		{	// ɾ��ptrCurNode���������ϵĽ��
			switch (ptrCurNode->bf)
			{	// ���ptrCurNode��ƽ���
			case LH:						// ���
				ptrCurNode->bf = EH;
				break;
			case EH:						// �ȸ�
				ptrCurNode->bf = RH;
				isShorter = false;
				break;
			case RH:						// �Ҹ�
				if (ptrParent == NULL)
				{	// �ѻ��ݵ������
					DeleteLeftBalance(ptrCurNode, isShorter);
					this->m_pRoot = ptrCurNode;		// ת����ptrCurNodeΪ�¸�
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent��������ƽ�⴦��
					DeleteLeftBalance(ptrParent->leftChild, isShorter);
				}
				else
				{	// ptrParent��������ƽ�⴦��
					DeleteLeftBalance(ptrParent->rightChild, isShorter);
				}
				break;
			}
		}
		else
		{	// ɾ��ptrCurNode���������ϵĽ��
			switch (ptrCurNode->bf)
			{	// ���ptrCurNode��ƽ���
			case RH: 						// �Ҹ�
				ptrCurNode->bf = EH;
				break;
			case EH: 						// �ȸ�
				ptrCurNode->bf = LH;
				isShorter = false;
				break;
			case LH: 						// ���
				if (ptrParent == NULL)
				{	// �ѻ��ݵ������
					DeleteLeftBalance(ptrCurNode, isShorter);
					this->m_pRoot = ptrCurNode;		// ת����ptrCurNodeΪ�¸�
				}
				else if (ptrParent->leftChild == ptrCurNode)
				{	// ptrParent��������ƽ�⴦��
					DeleteLeftBalance(ptrParent->leftChild, isShorter);
				}
				else
				{	// ptrParent��������ƽ�⴦��
					DeleteLeftBalance(ptrParent->rightChild, isShorter);
				}
				break;
			}
		}
	}
}

template <class ElemType, class KeyType>
void BinaryAVLTree<ElemType, KeyType>::DeleteAux(const KeyType& key,
	BinAVLTreeNode<ElemType>*& p, LinkStack< BinAVLTreeNode<ElemType>*>& s)
	// �������: ɾ��pָ��Ľ��
{
	BinAVLTreeNode<ElemType>* temPtr, * temF;  // ��ʱ����
	if (p->leftChild == NULL && p->rightChild == NULL)
	{	// pΪҶ���
		delete p;
		p = NULL;
		DeleteBalance(key, s);
	}
	else if (p->leftChild == NULL)
	{	// pֻ��������Ϊ��
		temPtr = p;
		p = p->rightChild;
		delete temPtr;
		DeleteBalance(key, s);
	}
	else if (p->rightChild == NULL)
	{	// pֻ���������ǿ�
		temPtr = p;
		p = p->leftChild;
		delete temPtr;
		DeleteBalance(key, s);
	}
	else
	{	// p�����ӷǿ�
		temF = p;
		s.Push(temF);
		temPtr = p->leftChild;
		while (temPtr->rightChild != NULL)
		{	// ����p������������ֱ��ǰ��temPtr����˫��temF��temPtr��������Ϊ��
			temF = temPtr;
			s.Push(temF);
			temPtr = temPtr->rightChild;
		}
		p->data = temPtr->data;// ��temPtrָ�����Ԫ��ֵ��ֵ��pָ�����Ԫ��ֵ

		// ɾ��temPtrָ��Ľ��
		if (temF->rightChild == temPtr)
		{	// ɾ��temF���Һ���
			DeleteAux(key, temF->rightChild, s);
		}
		else
		{	// ɾ��temF������
			DeleteAux(key, temF->leftChild, s);
		}
	}
}

template <class ElemType, class KeyType>
bool BinaryAVLTree<ElemType, KeyType>::Delete(const KeyType& key)
// �������: ɾ���ؼ���Ϊkey�Ľ��
{
	BinAVLTreeNode<ElemType>* p, * f;
	LinkStack< BinAVLTreeNode<ElemType>*> s;
	p = SearchAux(key, f, s);
	if (p == NULL)
	{	// ����ʧ��, ɾ��ʧ��
		return false;
	}
	else
	{	// ���ҳɹ�, ����ɹ�
		if (f == NULL)
		{	// ��ɾ�����Ϊ�����
			DeleteAux(key, this->m_pRoot, s);
		}
		else if (key < f->data)
		{	// key��С, ɾ��f������
			DeleteAux(key, f->leftChild, s);
		}
		else
		{	// key����, ɾ��f���Һ���
			DeleteAux(key, f->rightChild, s);
		}
		return true;
	}
}
#endif // !defined(_BIN_AVL_TREE_H_)

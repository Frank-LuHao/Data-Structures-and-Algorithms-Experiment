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
	SqBinaryTree();//�����2����
	SqBinaryTree(ElemType e,int nSize = DEFAULT_SIZE);//������eΪ����2����
	~SqBinaryTree();
	SqBinaryTree(const SqBinaryTree<ElemType>& source);		// ���ƹ��캯��ģ��
	SqBinaryTree(SqBinTreeNode<ElemType> es[], int r, int size = DEFAULT_SIZE);
	// ��es[]��r��size���������	
	SqBinaryTree<ElemType>& operator=(const SqBinaryTree<ElemType>&source);//����'='�����
	virtual int GetRoot() const ;						// ���ض������ĸ�
	virtual bool NodeEmpty(int cur) const ;				// �жϽ��cur�Ƿ�Ϊ��
	virtual bool GetItem(int cur, ElemType& e) const ;	// ���ؽ��cur��Ԫ��ֵ
	virtual bool SetElem(int cur, const ElemType& e) ;	// �����cur��ֵ��Ϊe
	virtual bool Empty() const ;						// �ж϶������Ƿ�Ϊ��
	virtual int &LeftChild(int cur) const ;		// ���ض������н��cur������
	virtual int &RightChild(int cur) const ;	// ���ض������н��cur���Һ���
	virtual int Parent(int cur) const ;		// ���ض������н��cur��˫��
	virtual bool CreateBinaryTree(ElemType pre[], ElemType in[], int n);//���������������д���������
protected:
	virtual ElemType NodeElem(int cur) const ;		// ����ָ���ڵ��Ԫ��ֵ
	virtual void ReleaseNode(int& cur) ;			// �����cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
	virtual int CreateChildNode(int cur, bool bLeft) ;//����cur�ڵ���ӽڵ�
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
	this->m_pElemData[m_nRoot].tag = USED_NODE;	// ����־
	this->m_pElemData[m_nRoot].data = e;			// ��������Ԫ��ֵ
}
template <class ElemType>
SqBinaryTree<ElemType>::~SqBinaryTree()
{
}

template <class ElemType>
SqBinaryTree<ElemType>& SqBinaryTree<ElemType>::operator=(const SqBinaryTree<ElemType>& source)
// �������������֪������source���Ƶ���ǰ�������������ظ�ֵ�����
{
	if (&source != this)
	{
		this->Clear();				//����˳������������
		int maxSize = source.m_nDataLen;	// ��������
		this->SetBufferSize(maxSize + 1);		//����˳���������С
		// ���ƽ������
		memcpy(this->m_pElemData, source.m_pElemData, sizeof(SqBinTreeNode<ElemType>) * maxSize);
		this->m_nDataLen = maxSize;		//����˳���ĵ�ǰ��Ч���ݳ���
		m_nRoot = source.GetRoot();		// ���ƶ�������
	}
	return *this;
}

template <class ElemType>
SqBinaryTree<ElemType>::SqBinaryTree(const SqBinaryTree<ElemType>& source)
// �������������֪�����������¶������������ƹ��캯��ģ��
{
	*this = source;//����������'='������ֱ�Ӹ�ֵ
}

template <class ElemType>
SqBinaryTree<ElemType>::SqBinaryTree(SqBinTreeNode<ElemType> es[], int r, int size)
// �����������es[]��r������������Ϊsize�Ķ�����, 
	:SqList<SqBinTreeNode<ElemType>>(size)
{
	
	this->m_nDataLen = size;									// ��������
	memcpy(this->m_pElemData, es, sizeof(SqBinTreeNode<ElemType>) * size);// ���ƽ��
	m_nRoot = r;			// ���ƶ�������
}

template <class ElemType>
int SqBinaryTree<ElemType>::GetRoot() const
// ������������ض������ĸ�
{
	return m_nRoot;					// m_nRootΪ��
}



template <class ElemType>
int & SqBinaryTree<ElemType>::LeftChild(int cur) const
// ������������ض��������cur������
{
	int leftChild = 2 * cur;// 2 * curΪ���� 
	return leftChild;
}

template <class ElemType>
int & SqBinaryTree<ElemType>::RightChild(int cur) const
// ������������ض��������cur���Һ���
{
	int rightChild = 2 * cur;// 2 * cur + 1Ϊ�Һ���
	return rightChild;			
}

template <class ElemType>
int SqBinaryTree<ElemType>::Parent(int cur) const
// ������������ض��������cur��˫��
{
	return cur / 2;				// cur / 2Ϊ˫��
}
template <class ElemType>
bool SqBinaryTree<ElemType>::NodeEmpty(int cur) const
// ���������������curΪ��,�򷵻�true,���򷵻�false
{
	if (cur <1 || cur > this->m_nDataLen || this->m_pElemData[cur].tag == EMPTY_NODE) return true;	// ��ʱ���Ϊ��
	else return false;
}
template <class ElemType>
bool SqBinaryTree<ElemType>::GetItem(int cur, ElemType& e) const
// �����������e���ؽ��cur��Ԫ��ֵ,������ɹ�,����true,���򷵻�false
{
	if (NodeEmpty(cur)) return false;		// ����ʧ��
	else
	{
		e = this->m_pElemData[cur].data;				// ��e���ؽ��cur��Ԫ��ֵ
		return true;						// �����ɹ�
	}
}

template <class ElemType>
bool SqBinaryTree<ElemType>::SetElem(int cur, const ElemType& e)
// �����������������ڽ��cur,�򷵻�false,���򷵻�true,�������cur��ֵ����Ϊe
{
	if (NodeEmpty(cur)) return false;		// ����ʧ��
	else
	{	// ���ڽ��cur
		this->m_pElemData[cur].data = e;				// �����cur��ֵ����Ϊe
		return true;						// ����true
	}
}

template <class ElemType>
bool SqBinaryTree<ElemType>::Empty() const
// ����������ж϶������Ƿ�Ϊ��
{
	return NodeEmpty(m_nRoot);
}
template <class ElemType>
//������������ؽڵ�cur������
ElemType SqBinaryTree<ElemType>::NodeElem(int cur) const
{
	return this->m_pElemData[cur].data;
}
template <class ElemType>
//������������ýڵ�curΪ�սڵ�
void SqBinaryTree<ElemType>::ReleaseNode(int& cur)
{
	this->m_pElemData[cur].tag = EMPTY_NODE;
}
template <class ElemType>
//�������,�����ڵ�cur�ĺ��ӽڵ�
int SqBinaryTree<ElemType>::CreateChildNode(int cur, bool bLeft)
{
	if (cur < 0)
	{//�Ǹ��ڵ�

		return 1;
	}
	int nChildNodePtr = 0;
	if (NodeEmpty(cur))
		return 0;//��ǰ�ڵ�Ϊ�սڵ㣬����0
	if (bLeft)
		nChildNodePtr = LeftChild(cur);//���ӽڵ�Ϊ���ӽڵ�
	else 
		nChildNodePtr = RightChild(cur);//���ӽڵ�Ϊ�Һ��ӽڵ�
	if (nChildNodePtr >= this->m_nBufferLen)
	{
		this->SetBufferSize(nChildNodePtr*2);//���·����ڴ�ռ�
		this->m_nDataLen = nChildNodePtr + 1;//���õ�ǰ��Ч���ݳ���
	}
	this->m_pElemData[nChildNodePtr].tag = USED_NODE;//��־����Ϊ��Ч
	return nChildNodePtr;
}
template <class ElemType>
//�����������������������������˳��洢������
bool SqBinaryTree<ElemType>::CreateBinaryTree(ElemType pre[], ElemType in[], int n)
{
	if (!pre || !in || n < 1)
		return false;
	if (!this->SetBufferSize(n + 1))//���¸���n���û�������С
		return false;//���û�����ʧ�ܣ�����
	memset(this->m_pElemData, 0, sizeof(SqBinTreeNode<ElemType>) * (n + 1));//��ն�����
	m_nRoot = this->CreateBinaryTreeAux(-1, pre, in, 0, n - 1, 0, n - 1, false);
	return true;
}


#endif // !defined(_SQ_BINARY_TREE_H_)

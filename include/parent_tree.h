#if !defined(_PARENT_TREE_H_)
#define _PARENT_TREE_H_
#include "tree_base.h"
#include "SqList.h"
#include "parent_tree_node.h"
#include <memory.h>
#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 128							// ȱʡԪ�ظ���
#endif
template <class ElemType>
class ParentTree:public TreeBase<ElemType,int>,private SqList<ParentTreeNode<ElemType>>
{
public:
	ParentTree();
	~ParentTree();
	ParentTree(const ElemType& e, int size = DEFAULT_SIZE);
	// ����������Ԫ��eΪ������
	ParentTree(const ParentTree<ElemType>& source);	// ���ƹ��캯��ģ��
	ParentTree(ElemType items[], int parents[], int r, int n, int size = DEFAULT_SIZE);
	// ��������Ԫ��Ϊitems[]����Ӧ���˫��Ϊparents[]�������λ��Ϊr��������Ϊn����
	ParentTree<ElemType>& operator=(const ParentTree<ElemType>& source);
	// ���ظ�ֵ�����
	virtual int GetRoot() const;						// �������ĸ�
	virtual bool Empty() const;									// �ж����Ƿ�Ϊ��
	virtual bool NodeEmpty(int cur) const;					// �жϽ��cur�Ƿ�Ϊ��
	virtual bool GetElem(const int cur, ElemType& e) const;	// ��e���ؽ��Ԫ��ֵ
	virtual bool SetElem(int cur, const ElemType& e) ;		// ����cur��ֵ��Ϊe
	virtual int FirstChild(const int cur) const ;	// ���������cur�ĵ�һ������
	virtual int RightSibling(const int cur) const ;// ���������cur�����ֵ�
	virtual int Parent(const int cur) const ;		// ���������cur��˫��
private:
	virtual ElemType NodeElem(int cur) const ;		// ����ָ���ڵ��Ԫ��ֵ
	virtual void ReleaseNode(int& cur) ;				// �����cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
	virtual int CreateChildNode(int cur, int  position, const ElemType &e) ;//����cur�ڵ���ӽڵ�
	virtual int RemoveChild(int parent, int position);//��position�Ľڵ��parent�Ƴ�
	virtual void ClearRemovedNode();//���������ɾ����ǵĽڵ�
	int m_nRoot;//��
};
template <class ElemType>
ParentTree<ElemType>::ParentTree()
	:SqList<ParentTreeNode<ElemType>>(DEFAULT_SIZE)
{
	m_nRoot = -1;// ��ʾ�����ڸ�

}
template <class ElemType>
ParentTree<ElemType>::~ParentTree()
{
}

template <class ElemType>
int ParentTree<ElemType>::GetRoot() const
// ����������������ĸ�
{
	return m_nRoot;
}

template <class ElemType>
bool ParentTree<ElemType>::Empty() const
// ����������ж����Ƿ�Ϊ��
{
	return this->m_nDataLen == 0;
}

template <class ElemType>
bool ParentTree<ElemType>::GetElem(int cur, ElemType& e) const
// �����������e���ؽ��curԪ��ֵ,��������ڽ��cur,����false,���򷵻�true
{
	if (cur < 0 || cur >= this->m_nDataLen)
	{	// �����ڽ��cur
		return false;								// ����false
	}
	else
	{	// ���ڽ��cur
		e = this->m_pElemData[cur].data;						// ��e����Ԫ��ֵ
		return true;								// ����true
	}
}

template <class ElemType>
bool ParentTree<ElemType>::SetElem(int cur, const ElemType& e)
// �����������������ڽ��cur,�򷵻�false,���򷵻�true,�������cur��ֵ����Ϊe
{
	if (cur < 0 || cur >= this->m_nDataLen)
	{	// �����ڽ��cur
		return false;								// ����false
	}
	else
	{	// ���ڽ��cur
		this->m_pElemData[cur].data = e;						// �����cur��ֵ����Ϊe
		return true;								// ����true
	}
}

template <class ElemType>
int ParentTree<ElemType>::FirstChild(int cur) const
// �����������cur�޺���,�򷵻�-1,���򷵻������cur�ĵ�һ������,
{

	for (int pos = 0; pos < this->m_nDataLen; pos++)
	{	// �Ҳ��һ������
		if (this->m_pElemData[pos].parent == cur) return pos;	// posΪ��һ������
	}
	return -1;									// �Ҳ�ʧ��,��ʾ�޺���
}
template <class ElemType>
//����������жϽڵ��Ƿ�Ϊ��
bool ParentTree<ElemType>::NodeEmpty(int cur) const
{
	if (cur < 0 || cur >= this->m_nDataLen)
		return true;//�ڵ㲻�ڷ�Χ�ڣ�����true
	return false;
}
template <class ElemType>
int ParentTree<ElemType>::RightSibling(int cur) const
// ���������������cur�����ֵ�,�򷵻�-1,���򷵻�cur�����ֵ�
{
	for (int pos = cur + 1; pos < this->m_nDataLen; pos++)
	{	// ����cur�����ֵ�
		if (this->m_pElemData[cur].parent == this->m_pElemData[pos].parent) return pos;	// posΪcur�����ֵ�
	}
	return -1;									// ����ʧ��,��ʾ�����ֵ�
}

template <class ElemType>
int ParentTree<ElemType>::Parent(int cur) const
// ������������ؽ��cur��˫����
{
	if (cur < 0 || cur >= this->m_nDataLen)
		return -1;								// ���cur������,����-1��ʾ��˫��
	return this->m_pElemData[cur].parent;
}
template <class ElemType>
//��������� ����ָ���ڵ��Ԫ��ֵ
ElemType ParentTree<ElemType>::NodeElem(int cur) const
{
	return this->m_pElemData[cur].data;
}


template <class ElemType>
//��������� ���cur���ڽڵ���ڴ�
void ParentTree<ElemType>::ReleaseNode(int& cur)
{
	if (NodeEmpty(cur))
		return;
	this->m_pElemData[cur].parent = -2;//��ɾ�����
}
template <class ElemType>
//��������� ����cur�ڵ�ĵ�position���ӽڵ�
int ParentTree<ElemType>::CreateChildNode(int cur, int  position, const ElemType& e)
{
	if (NodeEmpty(cur))
		return -1;//cur�ڵ㲻���ڣ�����-1
	if (this->m_nDataLen + 1 >= this->m_nBufferLen)
	{//�����������
		this->SetBufferSize((this->m_nDataLen + 1) * 2);//���·��仺����
	}

	if (position <= 0 || position > this->NodeDegree(cur) + 1) return false;

	int temPos, child;							// ��ʱ���� 
	for (temPos = 1, child = FirstChild(cur); temPos < position && child != -1; child = RightSibling(child), temPos++);
	// ���ҵ�position�����ӵ�λ��
	
	if (child != -1)
	{	// ���ڵ�position�����ӣ���position�����ӵ�λ��Ϊ����λ��
		memmove(this->m_pElemData + child + 1, this->m_pElemData + child, sizeof(ParentTreeNode<ElemType>) * (this->m_nDataLen - child));
		for (int i = child+1; i <= this->m_nDataLen; i++)
		{
			if (this->m_pElemData[i].parent >= child)
				this->m_pElemData[i].parent++;//˫��λ�ô��ڻ����child�ģ�λ�ü�1

		}
		this->m_pElemData[child].data = e;					// �����child�ĵ�����Ԫ��ֵ��Ϊe
		this->m_pElemData[child].parent = cur;				// ���child��˫��Ϊcur
	}
	else
	{	// �����ڵ�pos������,�������
		child = this->m_nDataLen ;						// childΪ���λ��
		this->m_pElemData[child].data = e;					// �����child�ĵ�����Ԫ��ֵ��Ϊe
		this->m_pElemData[child].parent = cur;				// ���child��˫��Ϊcur
	}
	this->m_nDataLen++;										// �����,������Լ�1
	return true;								// �ɹ�

}
template <class ElemType>
//��������� ֻ��Ҫ��ȡparent�ڵ�ĵ�positionλ�õĺ���
int ParentTree<ElemType>::RemoveChild(int parent, int position)
{
	if (NodeEmpty(parent))
		return -1;
	if (position < 0 || position >= this->NodeDegree(parent))
		return -1;
	int temPos, child = -1;							// ��ʱ���� 
	for (temPos = 1, child = FirstChild(parent); temPos < position && child != -1; child = RightSibling(child), temPos++);//����position���ӵ�λ��

	return child;

}

template <class ElemType>
ParentTree<ElemType>::ParentTree(const ElemType& e, int size)
// �������������������Ԫ��eΪ������
	:SqList<ParentTreeNode<ElemType>>(size)
{
	m_nRoot = 0;								// ����λ��
	this->m_nDataLen = 1;						// ������
	this->m_pElemData[m_nRoot].data = e;						// ����������и�ֵ
	this->m_pElemData[m_nRoot].parent = -1;					// ����˫��Ϊ-1
}
template <class ElemType>
void ParentTree<ElemType>::ClearRemovedNode()
// �������������ѱ��Ϊɾ��(˫�׽ڵ�Ϊ-2)�Ľڵ�
{
	int* pnNewPostion = new int[this->m_nDataLen];
	
	//���Ƚ����½ڵ��ԭ�нڵ��ӳ���ϵ
	int i,pos;
	for (i = 0,pos = 0; i < this->m_nDataLen; i++)
	{
		if (this->m_pElemData[i].parent == -2)
		{//�ѱ��Ϊɾ���Ľڵ�
			pnNewPostion[i] = -2;
			continue;
		}
		pnNewPostion[i] = pos;
		pos++;
	}
	
	int idx;
	
	for (i = 0,idx = 0; i < this->m_nDataLen; i++)
	{
		if (this->m_pElemData[i].parent == -2)
			continue;//�ѱ��Ϊɾ�������ݲ�ת��
		if(idx !=i)
			this->m_pElemData[idx] = this->m_pElemData[i];///��û�б��Ϊɾ��������ת�Ƶ���λ��(����i>=idx,�����ھɵ����ݱ����Ƕ�ʧ)
		if(this->m_pElemData[idx].parent>=0 )
			this->m_pElemData[idx].parent = pnNewPostion[this->m_pElemData[idx].parent]; //��˫�׽ڵ��λ��ͨ��ӳ���ϵת�����µ�λ��
		idx++;
	}
	delete []pnNewPostion;
	this->m_nDataLen = idx;

}
template <class ElemType>
ParentTree<ElemType>::ParentTree(const ParentTree<ElemType>& source)
// �������������֪�����������������ƹ��캯��ģ��
{
	*this = source;//����������'='�����
}
template <class ElemType>
ParentTree<ElemType>::ParentTree(ElemType items[], int parents[], int r, int n, int size)
// �����������������Ԫ��Ϊitems[],��Ӧ���˫��Ϊparents[],�����λ��Ϊr,������Ϊn����
{
	if (n > size)
		size = n;
	this->SetBufferSize(size);								// ��������
	for (int pos = 0; pos < n; pos++)
	{	// ���ζԽ������Ԫ�ؼ�˫��λ�ý��и�ֵ
		this->m_pElemData[pos].data = items[pos];			// ����Ԫ��ֵ
		this->m_pElemData[pos].parent = parents[pos];		// ˫��λ��
	}
	m_nRoot = r;									// ��
	this->m_nDataLen = n;									// ������
}


template <class ElemType>
ParentTree<ElemType>& ParentTree<ElemType>::operator=(const ParentTree<ElemType>& source)
// �������������֪��source���Ƶ���ǰ���������ظ�ֵ�����
{
	if (&source != this)
	{
		this->SetBufferSiz(source.m_nBufferLen);					// ��������
		memcpy(this->m_pElemData, source.m_pElemData, sizeof(ParentTreeNode<ElemType>) * source.m_nDataLen);
		m_nRoot = source.m_nRoot;							// ��
		this->m_nDataLen = source.m_nDataLen;				// ������
	}
	return *this;
}


#endif
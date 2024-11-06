#if !defined(_CHILD_PARENT_TREE_H_)
#define _CHILD_PARENT_TREE_H_
#include "tree_base.h"
#include "child_parent_tree_node.h"
#include "SqList.h"
#include <memory.h>
#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 128							// ȱʡԪ�ظ���
#endif
template <class ElemType>
class ChildParentTree :public TreeBase<ElemType,int>,private SqList<ChildParentTreeNode<ElemType>>
{
public:
	ChildParentTree();
	~ChildParentTree();
	ChildParentTree(const ElemType& e, int size = DEFAULT_SIZE);
	// ����������Ԫ��eΪ������
	ChildParentTree(const ChildParentTree<ElemType>& source);	// ���ƹ��캯��ģ��
	ChildParentTree(ElemType items[], int parents[], int r, int n, int size = DEFAULT_SIZE);
	// ��������Ԫ��Ϊitems[]����Ӧ���˫��Ϊparents[]�������λ��Ϊr��������Ϊn����
	ChildParentTree<ElemType>& operator=(const ChildParentTree<ElemType>& source);
	// ���ظ�ֵ�����
	virtual int GetRoot() const;						// �������ĸ�
	virtual bool Empty() const;									// �ж����Ƿ�Ϊ��
	virtual bool NodeEmpty(int cur) const;					// �жϽ��cur�Ƿ�Ϊ��
	virtual bool GetElem(const int cur, ElemType& e) const;	// ��e���ؽ��Ԫ��ֵ
	virtual bool SetElem(int cur, const ElemType& e);		// ����cur��ֵ��Ϊe
	virtual int FirstChild(const int cur) const;	// ���������cur�ĵ�һ������
	virtual int RightSibling(const int cur) const;// ���������cur�����ֵ�
	virtual int Parent(const int cur) const;		// ���������cur��˫��
private:
	virtual ElemType NodeElem(int cur) const;		// ����ָ���ڵ��Ԫ��ֵ
	virtual void ReleaseNode(int& cur);				// �����cur�ÿ�,�������ʽ�洢ɾ���ڵ��ڴ�
	virtual int CreateChildNode(int cur, int  position, const ElemType& e);//����cur�ڵ���ӽڵ�
	virtual int RemoveChild(int parent, int position);//��position�Ľڵ��parent�Ƴ�
	virtual void ClearRemovedNode();//���������ɾ����ǵĽڵ�
	int m_nRoot;//��
};
template <class ElemType>
ChildParentTree<ElemType>::ChildParentTree()
	:SqList< ChildParentTreeNode<ElemType>>(DEFAULT_SIZE)
{
	m_nRoot = -1;// ��ʾ�����ڸ�
}
template <class ElemType>
ChildParentTree<ElemType>::~ChildParentTree()
{
}


template <class ElemType>
int ChildParentTree<ElemType>::GetRoot() const
// ����������������ĸ�
{
	return m_nRoot;
}

template <class ElemType>
bool ChildParentTree<ElemType>::Empty() const
// ����������ж����Ƿ�Ϊ��
{
	return this->m_nDataLen == 0;
}

template <class ElemType>
bool ChildParentTree<ElemType>::GetElem(int cur, ElemType& e) const
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
bool ChildParentTree<ElemType>::SetElem(int cur, const ElemType& e)
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
int ChildParentTree<ElemType>::FirstChild(int cur) const
// �����������cur�޺���,�򷵻�-1,���򷵻������cur�ĵ�һ������,
{
	if (cur < 0 || cur >= this->m_nDataLen)
		return -1;		// ���cur������,����-1��ʾ�޺���
	if (this->m_pElemData[cur].childLkList.IsEmpty())	return -1; //������Ϊ�գ��޺���
	int child;
	this->m_pElemData[cur].childLkList.GetElem(0, child);	// ȡ����һ������
	return child;

}
template <class ElemType>
//����������жϽڵ��Ƿ�Ϊ��
bool ChildParentTree<ElemType>::NodeEmpty(int cur) const
{
	if (cur < 0 || cur >= this->m_nDataLen)
		return true;//�ڵ㲻�ڷ�Χ�ڣ�����true
	return false;
}
template <class ElemType>
int ChildParentTree<ElemType>::RightSibling(int cur) const
// ���������������cur�����ֵ�,�򷵻�-1,���򷵻�cur�����ֵ�
{
	if (NodeEmpty(cur))
		return -1;
	int parent = this->m_pElemData[cur].parent;//�ҵ�˫�׽ڵ�
	if (NodeEmpty(parent))
		return -1; //û��˫�ף�Ҳ��û���ֵܽڵ�
	
	SimpleLkList<int>* pChildLkList = &this->m_pElemData[parent].childLkList;
	pChildLkList->Head();//˫�׽ڵ�ĺ�������ǰλ�ù�λͷ�ڵ�
	int child;
	int nRightSibling = -1;//���ֵܽڵ��ֵΪ-1,��ʾû���ҵ�
	while (pChildLkList->Next(child))
	{
		if (child == cur)
		{
			pChildLkList->Next(nRightSibling);//��ȡ���ֵܽڵ�
			break;
		}
	}

	return nRightSibling;									
}

template <class ElemType>
int ChildParentTree<ElemType>::Parent(int cur) const
// ������������ؽ��cur��˫����
{
	if (cur < 0 || cur >= this->m_nDataLen)
		return -1;								// ���cur������,����-1��ʾ��˫��
	return this->m_pElemData[cur].parent;
}
template <class ElemType>
//��������� ����ָ���ڵ��Ԫ��ֵ
ElemType ChildParentTree<ElemType>::NodeElem(int cur) const
{
	return this->m_pElemData[cur].data;
}


template <class ElemType>
//��������� ���cur���ڽڵ���ڴ�
void ChildParentTree<ElemType>::ReleaseNode(int& cur)
{
	if (NodeEmpty(cur))
		return;
	this->m_pElemData[cur].parent = -2;//��ɾ�����
}
template <class ElemType>
//��������� ����cur�ڵ�ĵ�position���ӽڵ�
int ChildParentTree<ElemType>::CreateChildNode(int cur, int  position, const ElemType& e)
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
		int i;
		//��������
		for (i = this->m_nDataLen; i > child; i--)
		{
			this->m_pElemData[i] = this->m_pElemData[i - 1];
			if (this->m_pElemData[i].parent >= child)
				this->m_pElemData[i].parent + 1;

		}
		//�������ڵ�ĺ��ӽڵ���
		for (i = 0; i <= this->m_nDataLen; i++)
		{
			if (i == child)
				continue;
			SimpleLkList<int>* pChildLkList = &this->m_pElemData[i].childLkList;
			pChildLkList->Head();
			int nPosChild;
			while (pChildLkList->Next(nPosChild))
			{
				if (nPosChild >= child)
				{//�ӽڵ��ڲ���ڵ�λ��֮��,�ӽڵ��λ�ü�1
					pChildLkList->SetCurElem(nPosChild + 1);
				}
			}

		}
		this->m_pElemData[child].data = e;					// �����child�ĵ�����Ԫ��ֵ��Ϊe
		this->m_pElemData[child].parent = cur;				// ���child��˫��Ϊcur
		this->m_pElemData[cur].childLkList.Insert(position, child);//cur�ĺ���������child��λ��
	}
	else
	{	// �����ڵ�pos������,�������
		child = this->m_nDataLen;						// childΪ���λ��
		this->m_pElemData[child].data = e;					// �����child�ĵ�����Ԫ��ֵ��Ϊe
		this->m_pElemData[child].parent = cur;				// ���child��˫��Ϊcur
		this->m_pElemData[cur].childLkList.Insert(position, child);//cur�ĺ���������child��λ��
	}
	this->m_nDataLen++;										// �����,������Լ�1
	return true;								// �ɹ�

}
template <class ElemType>
//��������� ��position�Ľڵ��parent�Ƴ�
int ChildParentTree<ElemType>::RemoveChild(int parent, int position)
{
	if (NodeEmpty(parent))
		return -1;
	if (position < 0 || position >= this->NodeDegree(parent))
		return -1;
	int child = -1;							// ��ʱ���� 
	SimpleLkList<int>* pChildLkList = &this->m_pElemData[parent].childLkList;
	//�ں�������ɾ��position�ĺ���
	if (!pChildLkList->Delete(position, child))
		child = -1;
	return child;

}
template <class ElemType>
void ChildParentTree<ElemType>::ClearRemovedNode()
// �������������ѱ��Ϊɾ��(˫�׽ڵ�Ϊ-2)�Ľڵ�
{
	int* pnNewPostion = new int[this->m_nDataLen];

	//���Ƚ����½ڵ��ԭ�нڵ��ӳ���ϵ
	int i, pos;
	for (i = 0, pos = 0; i < this->m_nDataLen; i++)
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

	for (i = 0, idx = 0; i < this->m_nDataLen; i++)
	{
		if (this->m_pElemData[i].parent == -2)
			continue;//�ѱ��Ϊɾ�������ݲ�ת��
		if (idx != i)
			this->m_pElemData[idx] = this->m_pElemData[i];///��û�б��Ϊɾ��������ת�Ƶ���λ��(����i>=idx,�����ھɵ����ݱ����Ƕ�ʧ)
		if (this->m_pElemData[idx].parent >= 0)
			this->m_pElemData[idx].parent = pnNewPostion[this->m_pElemData[idx].parent]; //��˫�׽ڵ��λ��ͨ��ӳ���ϵת�����µ�λ��
		SimpleLkList<int>* pChildLkList = &this->m_pElemData[idx].childLkList;
		pChildLkList->Head();
		int nChildPos;
		//�������ڵ�ĺ��ӽڵ���
		while (pChildLkList->Next(nChildPos))
		{
			if (pnNewPostion[this->m_pElemData[idx].parent] == -2)
			{//���ӽڵ�Ϊɾ���Ľڵ�
				pChildLkList->DeleteCur(nChildPos);
			}
			else if(nChildPos>=0)
			{ 
				pChildLkList->SetCurElem(pnNewPostion[nChildPos]);//ͨ��ӳ��������µĺ��ӽڵ�λ��
			}
		}
		idx++;
	}
	delete[]pnNewPostion;
	this->m_nDataLen = idx;

}
template <class ElemType>
ChildParentTree<ElemType>::ChildParentTree(const ElemType& e, int size)
// �������������������Ԫ��eΪ������
	:SqList<ChildParentTreeNode<ElemType>>(size)
{
	m_nRoot = 0;								// ����λ��
	this->m_nDataLen = 1;						// ������
	this->m_pElemData[m_nRoot].data = e;						// ����������и�ֵ
	this->m_pElemData[m_nRoot].parent = -1;					// ����˫��Ϊ-1
}

template <class ElemType>
ChildParentTree<ElemType>::ChildParentTree(const ChildParentTree<ElemType>& source)
// �������������֪�����������������ƹ��캯��ģ��
{
	*this = source;//����������'='�����
}
template <class ElemType>
ChildParentTree<ElemType>::ChildParentTree(ElemType items[], int parents[], int r, int n, int size)
// �����������������Ԫ��Ϊitems[],��Ӧ���˫��Ϊparents[],�����λ��Ϊr,������Ϊn����
{
	if (n > size)
		size = n;
	this->SetBufferSize(size);								// ��������
	for (int pos = 0; pos < n; pos++)
	{	// ���ζԽ������Ԫ�ؼ�˫��λ�ý��и�ֵ
		this->m_pElemData[pos].data = items[pos];								// ����Ԫ��ֵ
		this->m_pElemData[pos].parent = parents[pos];							// ˫��λ��
		if (parents[pos] != -1)
		{
			SimpleLkList<int>*& cLkList = this->m_pElemData[parents[pos]].childLkList;	// ˫�׵ĺ�������
			cLkList.Insert(cLkList.Length() + 1, pos);					// ��pos������˫�׵ĺ���������
		}
	}
	m_nRoot = r;									// ��
	this->m_nDataLen = n;									// ������
}


template <class ElemType>
ChildParentTree<ElemType>& ChildParentTree<ElemType>::operator=(const ChildParentTree<ElemType>& source)
// �������������֪��source���Ƶ���ǰ���������ظ�ֵ�����
{
	if (&source != this)
	{
		this->SetBufferSiz(source.m_nBufferLen);					// ��������
		for (int pos = 0; pos < source.m_nDataLen; pos++)
		{	// ���ζԽ������Ԫ�ؼ�˫��λ�ý��и�ֵ
			this->m_pElemData[pos].data = source.m_pElemData[pos].data;				// ����Ԫ��ֵ
			this->m_pElemData[pos].parent = source.m_pElemData[pos].parent;			// ˫��λ��
			this->m_pElemData[pos].childLkList = source.m_pElemData[pos].childLkList;	// ��������
		}
		m_nRoot = source.m_nRoot;							// ��
		this->m_nDataLen = source.m_nDataLen;				// ������
	}
	return *this;
}


#endif
#if !defined(_PARENT_TREE_H_)
#define _PARENT_TREE_H_
#include "tree_base.h"
#include "SqList.h"
#include "parent_tree_node.h"
#include <memory.h>
#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 128							// 缺省元素个数
#endif
template <class ElemType>
class ParentTree:public TreeBase<ElemType,int>,private SqList<ParentTreeNode<ElemType>>
{
public:
	ParentTree();
	~ParentTree();
	ParentTree(const ElemType& e, int size = DEFAULT_SIZE);
	// 建立以数据元素e为根的树
	ParentTree(const ParentTree<ElemType>& source);	// 复制构造函数模板
	ParentTree(ElemType items[], int parents[], int r, int n, int size = DEFAULT_SIZE);
	// 建立数据元素为items[]，对应结点双亲为parents[]，根结点位置为r，结点个数为n的树
	ParentTree<ElemType>& operator=(const ParentTree<ElemType>& source);
	// 重载赋值运算符
	virtual int GetRoot() const;						// 返回树的根
	virtual bool Empty() const;									// 判断树是否为空
	virtual bool NodeEmpty(int cur) const;					// 判断结点cur是否为空
	virtual bool GetElem(const int cur, ElemType& e) const;	// 用e返回结点元素值
	virtual bool SetElem(int cur, const ElemType& e) ;		// 将结cur的值置为e
	virtual int FirstChild(const int cur) const ;	// 返回树结点cur的第一个孩子
	virtual int RightSibling(const int cur) const ;// 返回树结点cur的右兄弟
	virtual int Parent(const int cur) const ;		// 返回树结点cur的双亲
private:
	virtual ElemType NodeElem(int cur) const ;		// 返回指定节点的元素值
	virtual void ReleaseNode(int& cur) ;				// 将结点cur置空,如果是链式存储删除节点内存
	virtual int CreateChildNode(int cur, int  position, const ElemType &e) ;//创建cur节点的子节点
	virtual int RemoveChild(int parent, int position);//把position的节点从parent移除
	virtual void ClearRemovedNode();//清空已设置删除标记的节点
	int m_nRoot;//根
};
template <class ElemType>
ParentTree<ElemType>::ParentTree()
	:SqList<ParentTreeNode<ElemType>>(DEFAULT_SIZE)
{
	m_nRoot = -1;// 表示不存在根

}
template <class ElemType>
ParentTree<ElemType>::~ParentTree()
{
}

template <class ElemType>
int ParentTree<ElemType>::GetRoot() const
// 操作结果：返回树的根
{
	return m_nRoot;
}

template <class ElemType>
bool ParentTree<ElemType>::Empty() const
// 操作结果：判断树是否为空
{
	return this->m_nDataLen == 0;
}

template <class ElemType>
bool ParentTree<ElemType>::GetElem(int cur, ElemType& e) const
// 操作结果：用e返回结点cur元素值,如果不存在结点cur,返回false,否则返回true
{
	if (cur < 0 || cur >= this->m_nDataLen)
	{	// 不存在结点cur
		return false;								// 返回false
	}
	else
	{	// 存在结点cur
		e = this->m_pElemData[cur].data;						// 用e返回元素值
		return true;								// 返回true
	}
}

template <class ElemType>
bool ParentTree<ElemType>::SetElem(int cur, const ElemType& e)
// 操作结果：如果不存在结点cur,则返回false,否则返回true,并将结点cur的值设置为e
{
	if (cur < 0 || cur >= this->m_nDataLen)
	{	// 不存在结点cur
		return false;								// 返回false
	}
	else
	{	// 存在结点cur
		this->m_pElemData[cur].data = e;						// 将结点cur的值设置为e
		return true;								// 返回true
	}
}

template <class ElemType>
int ParentTree<ElemType>::FirstChild(int cur) const
// 操作结果：如cur无孩子,则返回-1,否则返回树结点cur的第一个孩子,
{

	for (int pos = 0; pos < this->m_nDataLen; pos++)
	{	// 找查第一个孩子
		if (this->m_pElemData[pos].parent == cur) return pos;	// pos为第一个孩子
	}
	return -1;									// 找查失败,表示无孩子
}
template <class ElemType>
//操作结果：判断节点是否为空
bool ParentTree<ElemType>::NodeEmpty(int cur) const
{
	if (cur < 0 || cur >= this->m_nDataLen)
		return true;//节点不在范围内，返回true
	return false;
}
template <class ElemType>
int ParentTree<ElemType>::RightSibling(int cur) const
// 操作结果：如果结点cur无右兄弟,则返回-1,否则返回cur的右兄弟
{
	for (int pos = cur + 1; pos < this->m_nDataLen; pos++)
	{	// 查找cur的右兄弟
		if (this->m_pElemData[cur].parent == this->m_pElemData[pos].parent) return pos;	// pos为cur的右兄弟
	}
	return -1;									// 查找失败,表示无右兄弟
}

template <class ElemType>
int ParentTree<ElemType>::Parent(int cur) const
// 操作结果：返回结点cur的双亲子
{
	if (cur < 0 || cur >= this->m_nDataLen)
		return -1;								// 结点cur不存在,返回-1表示无双亲
	return this->m_pElemData[cur].parent;
}
template <class ElemType>
//操作结果： 返回指定节点的元素值
ElemType ParentTree<ElemType>::NodeElem(int cur) const
{
	return this->m_pElemData[cur].data;
}


template <class ElemType>
//操作结果： 清除cur所在节点的内存
void ParentTree<ElemType>::ReleaseNode(int& cur)
{
	if (NodeEmpty(cur))
		return;
	this->m_pElemData[cur].parent = -2;//做删除标记
}
template <class ElemType>
//操作结果： 创建cur节点的第position个子节点
int ParentTree<ElemType>::CreateChildNode(int cur, int  position, const ElemType& e)
{
	if (NodeEmpty(cur))
		return -1;//cur节点不存在，返回-1
	if (this->m_nDataLen + 1 >= this->m_nBufferLen)
	{//缓存区会溢出
		this->SetBufferSize((this->m_nDataLen + 1) * 2);//重新分配缓冲区
	}

	if (position <= 0 || position > this->NodeDegree(cur) + 1) return false;

	int temPos, child;							// 临时变量 
	for (temPos = 1, child = FirstChild(cur); temPos < position && child != -1; child = RightSibling(child), temPos++);
	// 查找第position个孩子的位置
	
	if (child != -1)
	{	// 存在第position个孩子，第position个孩子的位置为插入位置
		memmove(this->m_pElemData + child + 1, this->m_pElemData + child, sizeof(ParentTreeNode<ElemType>) * (this->m_nDataLen - child));
		for (int i = child+1; i <= this->m_nDataLen; i++)
		{
			if (this->m_pElemData[i].parent >= child)
				this->m_pElemData[i].parent++;//双亲位置大于或等于child的，位置加1

		}
		this->m_pElemData[child].data = e;					// 将结点child的的数据元素值赋为e
		this->m_pElemData[child].parent = cur;				// 结点child的双亲为cur
	}
	else
	{	// 不存在第pos个孩子,插在最后
		child = this->m_nDataLen ;						// child为最后位置
		this->m_pElemData[child].data = e;					// 将结点child的的数据元素值赋为e
		this->m_pElemData[child].parent = cur;				// 结点child的双亲为cur
	}
	this->m_nDataLen++;										// 插入后,结点数自加1
	return true;								// 成功

}
template <class ElemType>
//操作结果： 只需要获取parent节点的第position位置的孩子
int ParentTree<ElemType>::RemoveChild(int parent, int position)
{
	if (NodeEmpty(parent))
		return -1;
	if (position < 0 || position >= this->NodeDegree(parent))
		return -1;
	int temPos, child = -1;							// 临时变量 
	for (temPos = 1, child = FirstChild(parent); temPos < position && child != -1; child = RightSibling(child), temPos++);//查找position孩子的位置

	return child;

}

template <class ElemType>
ParentTree<ElemType>::ParentTree(const ElemType& e, int size)
// 操作结果：建立以数据元素e为根的树
	:SqList<ParentTreeNode<ElemType>>(size)
{
	m_nRoot = 0;								// 根的位置
	this->m_nDataLen = 1;						// 结点个数
	this->m_pElemData[m_nRoot].data = e;						// 对数据域进行赋值
	this->m_pElemData[m_nRoot].parent = -1;					// 根的双亲为-1
}
template <class ElemType>
void ParentTree<ElemType>::ClearRemovedNode()
// 操作结果：清空已标记为删除(双亲节点为-2)的节点
{
	int* pnNewPostion = new int[this->m_nDataLen];
	
	//首先建立新节点和原有节点的映射关系
	int i,pos;
	for (i = 0,pos = 0; i < this->m_nDataLen; i++)
	{
		if (this->m_pElemData[i].parent == -2)
		{//已标记为删除的节点
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
			continue;//已标记为删除的数据不转移
		if(idx !=i)
			this->m_pElemData[idx] = this->m_pElemData[i];///把没有标记为删除的数据转移到新位置(由于i>=idx,不存在旧的数据被覆盖丢失)
		if(this->m_pElemData[idx].parent>=0 )
			this->m_pElemData[idx].parent = pnNewPostion[this->m_pElemData[idx].parent]; //把双亲节点的位置通过映射关系转换到新的位置
		idx++;
	}
	delete []pnNewPostion;
	this->m_nDataLen = idx;

}
template <class ElemType>
ParentTree<ElemType>::ParentTree(const ParentTree<ElemType>& source)
// 操作结果：由已知树构造新树——复制构造函数模板
{
	*this = source;//由于重载了'='运算符
}
template <class ElemType>
ParentTree<ElemType>::ParentTree(ElemType items[], int parents[], int r, int n, int size)
// 操作结果：建立数据元素为items[],对应结点双亲为parents[],根结点位置为r,结点个数为n的树
{
	if (n > size)
		size = n;
	this->SetBufferSize(size);								// 最大结点个数
	for (int pos = 0; pos < n; pos++)
	{	// 依次对结点数据元素及双亲位置进行赋值
		this->m_pElemData[pos].data = items[pos];			// 数据元素值
		this->m_pElemData[pos].parent = parents[pos];		// 双亲位置
	}
	m_nRoot = r;									// 根
	this->m_nDataLen = n;									// 结点个数
}


template <class ElemType>
ParentTree<ElemType>& ParentTree<ElemType>::operator=(const ParentTree<ElemType>& source)
// 操作结果：由已知树source复制到当前树——重载赋值运算符
{
	if (&source != this)
	{
		this->SetBufferSiz(source.m_nBufferLen);					// 最大结点个数
		memcpy(this->m_pElemData, source.m_pElemData, sizeof(ParentTreeNode<ElemType>) * source.m_nDataLen);
		m_nRoot = source.m_nRoot;							// 根
		this->m_nDataLen = source.m_nDataLen;				// 结点个数
	}
	return *this;
}


#endif
#if !defined(_REF_GEN_LIST_H_)
#define _REF_GEN_LIST_H_
#include "ref_gen_node.h"
#include "LinkListBase.h"
template <class ElemType>
class RefGenList :private LinkListBase< ElemType, RefGenListNode< ElemType>>
{
public:
	RefGenList();//无参数构造函数
	RefGenList(const RefGenList<ElemType>& source);	// 复制构造函数模板	
	~RefGenList();//析构函数
	RefGenListNode<ElemType>* First() const;	// 返回指向引用数法广义表的第一个元素的指针
	bool IsEmpty() const;//是否为空广义表
	void Push(const ElemType& e);				// 将原子元素e作为表头加入到引用数法广义表最前面
	void Push(RefGenList<ElemType>& subList);// 将子表subList作为表头加入到引用数法广义表最前面
	void Show() const;							// 显示广义表	
	int Depth() const;							//求广义表深度
	void Input();								// 输入广义表	
	RefGenListNode<ElemType>* Next(RefGenListNode<ElemType>* elemPtr) const;// 返回指向elemPtr指向的引用数法广义表元素的后继的指针
	RefGenList<ElemType>& operator =(const RefGenList<ElemType>& source);// 重载赋值运算符
private:
	virtual bool Link(RefGenListNode< ElemType>* pPreNode, RefGenListNode< ElemType>* pNextNode);//重载纯虚函数
	void ClearAux(RefGenListNode< ElemType>* &pHeadNode);//清空链表辅助函数
	void CopyAux(const RefGenListNode<ElemType>* sourceHead, RefGenListNode<ElemType>*& targetHead);
	void ShowAux(RefGenListNode<ElemType>* pHeader) const;
	int DepthAux(const RefGenListNode<ElemType>* pHeader) const;// 计算以pHeader为表头的引用数法广义表的深度
	void CreateAux(RefGenListNode<ElemType>*& first);// 创建以first为首元素结点的引用数法广义表
};
template <class ElemType>
//构造空广义表
RefGenList<ElemType>::RefGenList()
{
	this->m_pNodeHead->m_tag = _HEAD;//头节点类型为_HEAD
	this->m_pNodeHead->m_nRef = 1;//引用数为1
	//特殊析构方式，链表对象析构时，链表中的节点不一定析构
	this->m_bSpecialDetrcutiom = true;
}
template <class ElemType>
// 复制构造函数模板
RefGenList<ElemType>::RefGenList(const RefGenList<ElemType>& source)
{
	this->m_bSpecialDetrcutiom = true;
	CopyAux(source.m_pNodeHead, this->m_pNodeHead);
}
template <class ElemType>
// 计算以pHeader为表头的引用数法广义表的深度
int RefGenList<ElemType>::DepthAux(const RefGenListNode<ElemType>* pHeader) const
{
	if (pHeader->m_pNext == NULL) return 1;	// 空表，广义表的深度为1
	int subMaxDepth = 0;			// 子表最大深度
	for (RefGenListNode<ElemType>* temPtr = pHeader->m_pNext; temPtr != NULL;
		temPtr = temPtr->m_pNext)//遍历表节点
	{// 求子表的最大深度
		if (temPtr->m_tag == _LIST)
		{// 节点是子表
			int curSubDepth = DepthAux(temPtr->m_pSubLink);	// 求子表深度，递归调用
			if (subMaxDepth < curSubDepth) subMaxDepth = curSubDepth;
		}
	}
	return subMaxDepth + 1;		// 引用数法广义表深度为子表最大深度加1
}

template <class ElemType>
int RefGenList<ElemType>::Depth() const
// 操作结果：返回引用数法广义表深度
{
	return DepthAux(this->m_pNodeHead);
}
template <class ElemType>
//显示广义表辅助函数
void RefGenList<ElemType>::ShowAux(RefGenListNode<ElemType>* pHeader) const
{
	bool frist = true;
	cout << "(";								// 引用数法广义表以(开始
	for (RefGenListNode<ElemType>* temPtr = pHeader->m_pNext; temPtr != NULL; temPtr = temPtr->m_pNext)
	{	// 依次处理引用数法广义表各元素 
		if (frist) frist = false;				// 第一个元素
		else cout << ",";						// 不同元素这间用逗号隔开
		if (temPtr->m_tag == _ATOM)
		{	// 原子结点
			cout << temPtr->m_TData;
		}
		else
		{	// 表结点
			ShowAux(temPtr->m_pSubLink);
		}
	}
	cout << ")";								// 引用数法广义表以)结束

}
template<class ElemType>
RefGenList<ElemType>& RefGenList<ElemType>::operator =(const RefGenList<ElemType>& source)
// 操作结果：将引用数法广义表source赋值给当前引用数法广义表——重载赋值运算符
{
	if (&source != this)
	{
		ClearAux(this->m_pNodeHead);						// 清空当前引用数法广义表
		CopyAux(source.m_pNodeHead, this->m_pNodeHead);			// 复制引用数法广义表
	}
	return *this;
}

// 显示广义表	
template <class ElemType>
void RefGenList<ElemType>::Show() const
{
	ShowAux(this->m_pNodeHead);
}
template <class ElemType>
RefGenList<ElemType>::~RefGenList()
{
	ClearAux(this->m_pNodeHead);
}
template <class ElemType>
void RefGenList<ElemType>::CopyAux(const RefGenListNode<ElemType>* sourceHead,
	RefGenListNode<ElemType>*& targetHead)
	// 初始条件: 以sourceHead为头结点的引用数法广义表为非递归引用数法广义表
	// 操作结果: 将以sourceHead为头结点的引用数法广义表复制成以targetHead为头结点的引用数法
	//	广义表
{
	targetHead = new RefGenListNode<ElemType>(_HEAD);		// 复制头结点
	RefGenListNode<ElemType>* targetPtr = targetHead;		// targetHead的当前结点
	targetHead->m_nRef = 1;									// 引用数为1
	for (RefGenListNode<ElemType>* temPtr = sourceHead->m_pNext; temPtr != NULL;
		temPtr = temPtr->m_pNext)
	{	// 扫描引用数法广义表sourceHead的顶层
		targetPtr = targetPtr->m_pNext = new RefGenListNode<ElemType>(temPtr->tag);
		// 生成新结点
		if (temPtr->m_tag == _LIST)
		{	// 子表
			CopyAux(temPtr->m_pSubLink, targetPtr->m_pSubLink);// 复制子表
		}
		else
		{	// 原子结点
			targetPtr->m_TData = temPtr->m_TData;		// 复制原子结点
		}
	}
}
template <class ElemType>
RefGenListNode<ElemType>* RefGenList<ElemType>::Next(RefGenListNode<ElemType>* elemPtr) const
// 操作结果：返回指向elemPtr指向的引用数法广义表元素的后继的指针
{
	return elemPtr->nextLink;
}
template <class ElemType>
bool RefGenList<ElemType>::Link(RefGenListNode< ElemType>* pPreNode, RefGenListNode< ElemType>* pNextNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pNextNode;
	return true;
}

template <class ElemType>
RefGenListNode<ElemType>* RefGenList<ElemType>::First() const
// 操作结果：返回指向引用数法广义表的第一个元素的指针
{
	return this->m_pNodeHead->m_pNext;
}

template <class ElemType>
// 将原子元素e作为表头加入到引用数法广义表最前面
void RefGenList<ElemType>::Push(const ElemType& e)
{
	if (!this->Insert(0, e))
		return;
	this->m_pNodeCur->m_tag = _ATOM;//当前插入的元素类型为原子类型:_ATOM
	
}
template <class ElemType>
// 将子表subList作为表头加入到引用数法广义表最前面
void RefGenList<ElemType>::Push(RefGenList<ElemType>& subList)
{
	if (!this->Insert(0, 0))
	{//插入一个元素失败
		return;
	}
	RefGenListNode<ElemType>* pCurNodePtr = this->m_pNodeCur;//刚才插入的元素
	pCurNodePtr->m_pSubLink = subList.m_pNodeHead;				// 子表
	subList.m_pNodeHead->m_nRef++;						// subList引用数自加1
	pCurNodePtr->m_tag = _LIST;//当前插入的元素类型为子表：_LIST
}
template <class ElemType>
//清空广义表辅助函数
void RefGenList<ElemType>::ClearAux(RefGenListNode< ElemType>*& pHeadNode)
{
	if (!pHeadNode || pHeadNode->m_tag != _HEAD)
		return;//头节点类型才是广义表的起始节点
	pHeadNode->m_nRef--;//引用数自减1
	if (pHeadNode->m_nRef == 0)
	{//当链表的引用数为0时，才能清空表中的节点
		RefGenListNode<ElemType>* pNode = pHeadNode->m_pNext;//获取首元结点
		while (pNode)
		{//循环清空节点
			RefGenListNode<ElemType>* pNodeNext = pNode->m_pNext;//获取待删除节点的后继
			if (pNode->m_tag == _LIST)//判断节点类型是否为:_LIST
				ClearAux(pNode->m_pSubLink);//清空子表,递归调用
			delete pNode;//删除节点
			pNode = pNodeNext;//pNode指向后继
		}
		delete pHeadNode;//释放掉头节点
		pHeadNode = NULL;
	}
}


template<class ElemType>
void RefGenList<ElemType>::CreateAux(RefGenListNode<ElemType>*& first)
// 操作结果：创建以first为首元素结点的引用数法广义表
{
	char ch;									// 字符变量
	cin >> ch;									// 读入字符
	switch (ch)
	{
	case ')':									// 引用数法广义表建立完毕
		return;									// 结束
	case '(':									// 子表
		// 表头为子表
		first = new RefGenListNode<ElemType>(_LIST);// 生成表结点

		RefGenListNode<ElemType>* pSubHead;		// 子表指针
		pSubHead = new RefGenListNode<ElemType>(_HEAD);// 生成子表的头结点
		pSubHead->m_nRef = 1;						// 引用数为1	
		first->m_pSubLink = pSubHead;				// subHead为子表
		CreateAux(pSubHead->m_pNext);			// 递归建立子表

		cin >> ch;								// 跳过','
		if (ch != ',') cin.putback(ch);			// 如不是','，则将ch回退到输入流
		CreateAux(first->m_pNext);			// 建立引用数法广义表下一结点
		break;
	default:									// 原子
		// 表头为原子
		cin.putback(ch);						// 将ch回退到输入流
		ElemType amData;						// 原子结点数据
		cin >> amData;							// 输入原子结点数据
		first = new RefGenListNode<ElemType>(_ATOM);// 生成原表结点
		first->m_TData = amData;					// 原子结点数据

		cin >> ch;								// 跳过','
		if (ch != ',') cin.putback(ch);			// 如不是','，则将ch回退到输入流
		CreateAux(first->m_pNext);			// 建立引用数法广义表下一结点
		break;
	}
}
template<class ElemType>
bool RefGenList<ElemType>::IsEmpty() const
{
	return LinkListBase< ElemType, RefGenListNode< ElemType>>::IsEmpty();//调用父类方法
}
template<class ElemType>
void RefGenList<ElemType>::Input()
// 操作结果：输入广义表
{
	if (this->m_pNodeHead)
	{
		ClearAux(this->m_pNodeHead);
	}
	char ch;									// 字符变量
	this->m_pNodeHead = new RefGenListNode<ElemType>(_HEAD);	// 生成引用数法广义表头结点
	this->m_pNodeHead->m_nRef = 1;								// 引用数为1

	cin >> ch;									// 读入第一个'('
	RefGenList<ElemType>::CreateAux(this->m_pNodeHead ->m_pNext);
	// 创建以m_pNodeHead ->m_pNext为表头的引用数法广义表
}
#endif
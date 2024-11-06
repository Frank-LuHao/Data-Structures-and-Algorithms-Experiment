#if !defined(_LK_QUEUE_H_)
#define  _LK_QUEUE_H_
#include "SimpleLkList.h"

template <class ElemType>
class LinkQueue :private SimpleLkList <ElemType>
{
public:
	LinkQueue();//构造函数
	LinkQueue(const LinkQueue<ElemType>& source);//拷贝构造函数
	~LinkQueue();//析构函数
	int Length() const;//获取链式队列中元素个数
	bool IsEmpty() const;//判断链式队列是否为空
	void Clear();//清空链式队列
	void Traverse(bool (*visit)(const ElemType& e))const;//遍历链式队列
	bool OutQueue(ElemType& e);	// 出队操作
	bool OutQueue();		// 出队操作
	bool GetHead(ElemType& e) const;	// 取队头操作
	bool InQueue(const ElemType& e);	// 入队
};


//链式队列的实现代码
template <class ElemType>
LinkQueue<ElemType>::LinkQueue()
{

}
template <class ElemType>
LinkQueue<ElemType>::LinkQueue(const LinkQueue<ElemType>& source)
{
	*this = source;//基类中重载了'='，调用赋值运算，构造对象
}
template <class ElemType>
LinkQueue<ElemType>::~LinkQueue()
{
}

template <class ElemType>
int LinkQueue<ElemType>::Length() const
{
	return SimpleLkList<ElemType>::Length();//调用父类的方法
}
template <class ElemType>
void LinkQueue<ElemType>::Clear()
{
	return SimpleLkList<ElemType>::Clear();//调用父类的方法
}
template <class ElemType>
bool LinkQueue<ElemType>::IsEmpty()const
{
	return SimpleLkList<ElemType>::IsEmpty();//调用父类的方法
}

template <class ElemType>
void LinkQueue<ElemType>::Traverse(bool (*visit)(const ElemType& e)) const
{
	return SimpleLkList<ElemType>::Traverse(visit);//调用父类的方法
}
template <class ElemType>
bool LinkQueue<ElemType>::OutQueue(ElemType& e)
{
	if (this->IsEmpty())
		return false;//保护性检查，空队列时返回失败
	return this->Delete(0, e);//调用父类方法，在头部删除数据
}
template <class ElemType>
bool LinkQueue<ElemType>::OutQueue()
{
	if (this->IsEmpty())
		return false;//保护性检查，空队列时返回失败
	ElemType e;
	return this->Delete(0, e);//调用父类方法，在头部删除数据
}
template <class ElemType>
bool LinkQueue<ElemType>::GetHead(ElemType& e) const
{
	if (this->IsEmpty())
		return false;//保护性检查，空队列时返回失败
	return this->GetElem(0,e);//获取队头（首元结点）的数据
}
template <class ElemType>
bool LinkQueue<ElemType>::InQueue(const ElemType& e)
{
	this->AddTail(e);//调用父类方法，在队尾（尾节点）追加数据
	return true;
}
#endif
#if !defined(_LK_STACK_H_)
#define  _LK_STACK_H_
#include "SimpleLkList.h"

template <class ElemType>
class LinkStack :private SimpleLkList <ElemType>
{
public:
	LinkStack();//构造函数
	LinkStack(const LinkStack<ElemType>& source);//拷贝构造函数
	~LinkStack();//析构函数
	//重载的接口
	int Length() const;//获取链式栈中元素个数
	bool IsEmpty() const;//判断链式栈是否为空
	void Clear();//清空链式栈
	void Traverse(bool (*visit)(const ElemType& e))const;//遍历链式栈
	//个性化接口
	bool Push(const ElemType& e);	// 入栈
	bool Top(ElemType& e) const;		// 返回栈顶
	bool Pop(ElemType& e);	// 出栈
	bool Pop();			// 出栈
};

//顺序表实现代码
template <class ElemType>
LinkStack<ElemType>::LinkStack()
{

}
template <class ElemType>
LinkStack<ElemType>::LinkStack(const LinkStack<ElemType>& source)
{
	*this = source;//基类中重载了'='，调用赋值运算，构造对象
}
template <class ElemType>
LinkStack<ElemType>::~LinkStack()
{
}

template <class ElemType>
int LinkStack<ElemType>::Length() const
{
	return SimpleLkList<ElemType>::Length();//调用父类的方法
}
template <class ElemType>
void LinkStack<ElemType>::Clear()
{
	return SimpleLkList<ElemType>::Clear();//调用父类的方法
}
template <class ElemType>
bool LinkStack<ElemType>::IsEmpty()const
{
	return SimpleLkList<ElemType>::IsEmpty();//调用父类的方法
}

template <class ElemType>
void LinkStack<ElemType>::Traverse(bool (*visit)(const ElemType& e)) const
{
	return SimpleLkList<ElemType>::Traverse(visit);//调用父类的方法
}
template <class ElemType>
bool LinkStack<ElemType>::Push(const ElemType& e)
{
	this->Insert(0,e);////调用父类的方法，采用头插法插入数据
	return true;
}
template <class ElemType>
bool LinkStack<ElemType>::Top(ElemType& e) const
{
	if (this->IsEmpty())
		return false;//链表为空时，返回false
	return this->GetElem(0, e);//调用父类的方法，获取栈顶元素值
}
template <class ElemType>
bool LinkStack<ElemType>::Pop(ElemType& e)
{
	if (this->IsEmpty())
		return false;//链表为空时，返回false
	return this->Delete(0, e);//调用父类的方法，获取并删除栈顶元素值
}
template <class ElemType>
bool LinkStack<ElemType>::Pop()
{
	if (this->IsEmpty())
		return false;//链表为空时，返回false
	ElemType e;
	return this->Delete(0, e);//调用父类的方法，删除栈顶元素值
}
#endif
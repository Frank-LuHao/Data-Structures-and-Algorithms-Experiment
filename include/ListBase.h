#if !defined(_LIST_BASE_H_)
#define  _LIST_BASE_H_
#include <memory.h>
template <class ElemType>  class ListBase
{
public:
	virtual int Length() const = 0;//获取线性表中元素个数
	virtual bool IsEmpty() const = 0;//判断线性表是否为空
	virtual void Clear() = 0;//清空线性表
	virtual bool GetElem(int position, ElemType& e) const =0;//获取指定位置元素
	virtual bool SetElem(int position, const ElemType e) = 0;//设置指定位置元素
	virtual bool Delete(int position,ElemType& e) = 0;//删除指定位置元素
	virtual bool Insert(int position, const ElemType e) = 0;//在指定位置插入元素
	virtual void Traverse(bool (*visit)(const ElemType&))const = 0;//遍历线性表
	virtual int AddTail(const ElemType e) = 0;	//添加数据到线性表末尾

};
#endif
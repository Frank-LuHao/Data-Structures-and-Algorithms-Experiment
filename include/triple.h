#if !defined(__TRIPLE_H__)
#define __TRIPLE_H__
typedef union
{
	unsigned int key;//比对时的关键字
#if defined(IS_BIG_ENDIAN)
	//大端模式
	struct
	{
		unsigned short row;//行号
		unsigned short col;//列号
	}pos;
#else
	//小端模式
	struct
	{
		unsigned short col;//列号
		unsigned short row;//行号
	}pos;
#endif 
}ElemPos;
// 三元组类模板
template<class ElemType>
struct Triple
{
	// 数据成员:
	ElemPos ePos;						// 非零元素的行下标与列下标
	ElemType value;						// 非零元素的值

	// 构造函数模板:
	Triple();							// 无参数的构造函数
	Triple(int r, int c, const ElemType& v);	// 已知数据成员建立三元组
};

// 三元组类模板的实现部分
template<class ElemType>
Triple<ElemType>::Triple()
// 操作结果：构造空三元组
{
}

template<class ElemType>
Triple<ElemType>::Triple(int r, int c, const ElemType& v)
// 操作结果：由已知数数据成员构造三元组
{
	ePos.pos.col = c; //列号赋值
	ePos.pos.row = r; //行号赋值
	value = v;		// 非零元素值
}

#endif
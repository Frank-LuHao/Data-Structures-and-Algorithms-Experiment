#if !defined(_FIND_UNION_SET_H_)
#define _FIND_UNION_SET_H_
#include <stdlib.h>
class FindUnionSet
{
public:
	FindUnionSet(int nNums);
	~FindUnionSet();
	int Find(int x);//查找根节点编号
	bool IsSameTree(int x, int y);//查找x,y是否有同一个根节点
	bool Union(int x, int y);//合并x,y节点到同一个根节点
private:
	int* m_pParent;//父节点数组
	int m_nNums;//节点数
};

FindUnionSet::FindUnionSet(int nNums)
{
	m_pParent = new int[nNums];//创建双亲数组
	m_nNums = nNums;
	for (int i = 0; i < nNums; i++)
	{
		m_pParent[i] = i;//每个节点父节点初始化为自己
	}
}

int FindUnionSet::Find(int x)
//操作结果:查找节点x的根节点编号
{
	while (x >= 0 && x < m_nNums && m_pParent[x] != x)
	{//数据合法，且x的双亲不是自己时循环
		x = m_pParent[x];	//x向双亲回溯
	}
	if (x < 0 || x >= m_nNums)
		return -1;	//数据不合法，返回-1表示错误
	return x;
}

FindUnionSet::~FindUnionSet()
{
	if (m_pParent)
	{
		delete m_pParent;
		m_pParent = NULL;
	}
	
}

bool FindUnionSet::Union(int x,int y)
//操作结果:合并x,y节点到同一个根节点
{
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)	return false; //数据有效性检查
	if (x == y)	return true;	//本身就相等
	x = Find(x);			//取得x的根节点编号
	y = Find(y);			//取得y的根节点编号
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)	return false;//数据有效性检查
	if (x == y)	return true; 	//x,y根节点本身就相等，不必合并
	if (y > x)
	{//交换，保证y<=x
		int temp = x;
		x = y;	y = temp;
	}
	m_pParent[x] = y;		//两个数中根节点较大的合并到根节点较小的树
	return true;
}

bool FindUnionSet::IsSameTree(int x, int y)
//操作结果查找x,y是否有同一个根节点
{
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)
		return false;
	if (x == y)
		return true;
	x = Find(x);//取得x的根节点编号
	y = Find(y);//取得y的根节点编号
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)
		return false;
	if (x == y)
		return true;
	return false;
}
#endif // !defined(_FIND_UNION_SET_H_)

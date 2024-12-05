#if !defined(_FIND_UNION_SET_H_)
#define _FIND_UNION_SET_H_
#include <stdlib.h>
class FindUnionSet
{
public:
	FindUnionSet(int nNums);
	~FindUnionSet();
	int Find(int x);//���Ҹ��ڵ���
	bool IsSameTree(int x, int y);//����x,y�Ƿ���ͬһ�����ڵ�
	bool Union(int x, int y);//�ϲ�x,y�ڵ㵽ͬһ�����ڵ�
private:
	int* m_pParent;//���ڵ�����
	int m_nNums;//�ڵ���
};

FindUnionSet::FindUnionSet(int nNums)
{
	m_pParent = new int[nNums];//����˫������
	m_nNums = nNums;
	for (int i = 0; i < nNums; i++)
	{
		m_pParent[i] = i;//ÿ���ڵ㸸�ڵ��ʼ��Ϊ�Լ�
	}
}

int FindUnionSet::Find(int x)
//�������:���ҽڵ�x�ĸ��ڵ���
{
	while (x >= 0 && x < m_nNums && m_pParent[x] != x)
	{//���ݺϷ�����x��˫�ײ����Լ�ʱѭ��
		x = m_pParent[x];	//x��˫�׻���
	}
	if (x < 0 || x >= m_nNums)
		return -1;	//���ݲ��Ϸ�������-1��ʾ����
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
//�������:�ϲ�x,y�ڵ㵽ͬһ�����ڵ�
{
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)	return false; //������Ч�Լ��
	if (x == y)	return true;	//��������
	x = Find(x);			//ȡ��x�ĸ��ڵ���
	y = Find(y);			//ȡ��y�ĸ��ڵ���
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)	return false;//������Ч�Լ��
	if (x == y)	return true; 	//x,y���ڵ㱾�����ȣ����غϲ�
	if (y > x)
	{//��������֤y<=x
		int temp = x;
		x = y;	y = temp;
	}
	m_pParent[x] = y;		//�������и��ڵ�ϴ�ĺϲ������ڵ��С����
	return true;
}

bool FindUnionSet::IsSameTree(int x, int y)
//�����������x,y�Ƿ���ͬһ�����ڵ�
{
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)
		return false;
	if (x == y)
		return true;
	x = Find(x);//ȡ��x�ĸ��ڵ���
	y = Find(y);//ȡ��y�ĸ��ڵ���
	if (x < 0 || x >= m_nNums || y < 0 || y >= m_nNums)
		return false;
	if (x == y)
		return true;
	return false;
}
#endif // !defined(_FIND_UNION_SET_H_)

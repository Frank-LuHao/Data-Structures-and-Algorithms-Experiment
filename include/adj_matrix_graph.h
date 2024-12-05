#if !defined(_ADJ_MATRIX_GRAPH_H_)
#define _ADJ_MATRIX_GRAPH_H_
#include "graph_base.h"
#include "matrix.h"
using namespace std;
template <class ElemType, class EdgeWType>
class AdjMatrixGraph :public GraphBase<ElemType, ElemType, EdgeWType>
{
public:
	AdjMatrixGraph(ElemType vex[], int nVexNum, GRAPH_TYPE tGraph);//构造函数
	~AdjMatrixGraph();//析构函数
	//重载基类中定义的纯虚函数
	virtual int FirstAdjVex(int v, EdgeWType& weight) const ;			// 返回顶点v的第一个邻接点，并获得该边的权重
	virtual int NextAdjVex(int v1, int v2, EdgeWType& weight) const;	// 返回顶点v1的相对于v2的下一个邻接点，并获得该边权重	 
	virtual void InsertEdge(int v1, int v2, EdgeWType weight) ;			// 插入顶点为v1和v2,权为weight的边			 
	virtual void DeleteEdge(int v1, int v2) ;							// 删除顶点为v1和v2的边		
	AdjMatrixGraph(const AdjMatrixGraph<ElemType, EdgeWType>& source);	// 复制构造函数
	AdjMatrixGraph<ElemType, EdgeWType>& operator =(const AdjMatrixGraph<ElemType, EdgeWType>& source);//重载赋值运算符
	bool Floyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const;		//Floyd算法生成最短路径
	bool PrintFloyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const;		//打印输出Floyd算法生成的最短路径
private:
	//重载基类中定义的私有纯虚函数
	virtual void GetNodeElem(const ElemType& node, ElemType& e) const;//根据节点类型获取原子数据
	virtual void SetNodeElem(ElemType& node, const  ElemType& e);	//把原子数据存入到节点类型数据中
	EdgeWType noEdge;
	Matrix<EdgeWType> m_matrixEdge;//边矩阵
};
template <class ElemType, class EdgeWType>
AdjMatrixGraph<ElemType, EdgeWType>::AdjMatrixGraph(ElemType vex[], int nVexNum, GRAPH_TYPE tGraph)
	: GraphBase<ElemType, ElemType, EdgeWType>(vex, nVexNum, tGraph), m_matrixEdge(nVexNum,nVexNum)
{
	noEdge = 0;//无边的权值
	if (tGraph == DIR_NETWORK || tGraph == UNDIR_NETWORK)
	{//是网，无边的权值为无穷大
		noEdge = this->InfniteWeight();
	}

	int col, row;
	for (row = 0; row < nVexNum; row++)
	{//设置矩阵初值为无边状态
		EdgeWType* pMatrixAddr = m_matrixEdge.ElemAddress(row, 0);//获取矩阵起始第row行元素起始地址
		for (col = 0; col < nVexNum; col++)
		{
			pMatrixAddr[col] = noEdge;
		}
	}
}
template <class ElemType, class EdgeWType>
AdjMatrixGraph<ElemType, EdgeWType>::AdjMatrixGraph(const AdjMatrixGraph<ElemType, EdgeWType>& source)
{
	*this = source;//由于重载了'='运算符
}
template <class ElemType, class EdgeWType>
AdjMatrixGraph<ElemType, EdgeWType>::~AdjMatrixGraph()
{

}
template <class ElemType, class EdgeWType>
void AdjMatrixGraph<ElemType, EdgeWType>::GetNodeElem(const ElemType& node, ElemType& e) const
{
	e = node;
}
template <class ElemType, class EdgeWType>
void AdjMatrixGraph<ElemType, EdgeWType>::SetNodeElem(ElemType& node, const  ElemType& e)
{
	node = e;
}

template <class ElemType, class EdgeWType>
//操作结果：插入顶点为v1和v2,权为weight的边
void AdjMatrixGraph<ElemType, EdgeWType>::InsertEdge(int v1, int v2, EdgeWType weight)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//顶点不正确，返回
	if (this->m_type == UNDIR_GRAPH || this->m_type == DIR_GRAPH)
		weight = 1;//对于图，权为1
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//是无向网或无向图，是对称矩阵
		if (m_matrixEdge[v1][v2] == noEdge && m_matrixEdge[v2][v1] == noEdge)
		{
			m_matrixEdge[v1][v2] = weight;
			m_matrixEdge[v2][v1] = weight;
			this->m_nEdgeNum++;//边的数目加1
		}
	}
	else
	{//有向图或网
		if (m_matrixEdge[v1][v2] == noEdge)
		{//无边
			m_matrixEdge[v1][v2] = weight;
			this->m_nEdgeNum++;//边的数目加1
		}
	}
}
template <class ElemType, class EdgeWType>
// 操作结果：删除顶点为v1和v2的边
void AdjMatrixGraph<ElemType, EdgeWType>::DeleteEdge(int v1, int v2)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//顶点不正确，返回
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//是无向网或无向图，是对称矩阵

		if (m_matrixEdge[v1][v2] != noEdge || m_matrixEdge[v2][v1] != noEdge)
		{
			m_matrixEdge[v1][v2] = noEdge;
			m_matrixEdge[v2][v1] = noEdge;
			this->m_nEdgeNum--;//边的数目减1
		}
	}
	else
	{
		//有向图或网
		if (m_matrixEdge[v1][v2] != noEdge)
		{//有边
			m_matrixEdge[v1][v2] = noEdge;
			this->m_nEdgeNum--;//边的数目减1
		}
	}
}
template <class ElemType, class EdgeWType>
// 操作结果：返回顶点v的第一个邻接点，并获得该边的权重
int AdjMatrixGraph<ElemType, EdgeWType>::FirstAdjVex(int v, EdgeWType& weight) const
{
	if (v < 0 || v >= this->m_nVexNum )
		return -1;//顶点不正确，返回失败（顶点序号为-1）
	EdgeWType* pVAdr = m_matrixEdge.ElemAddress(v, 0);//获取矩阵起始第v行，0列元素地址
	for (int cur = 0; cur <this->m_nVexNum; cur++)
	{	// 查找邻接点
		if (pVAdr[cur] != noEdge)
		{
			weight = pVAdr[cur];
			return cur;
		}
	}
	return -1;
}
template <class ElemType, class EdgeWType>
// 操作结果：返回顶点v1的相对于v2的下一个邻接点，并获得该边权重
int AdjMatrixGraph<ElemType, EdgeWType>::NextAdjVex(int v1, int v2, EdgeWType& weight) const
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return -1;//顶点不正确，返回
	if (v1 == v2)
		return -1;//v1不能为v2
	EdgeWType* pVAdr = m_matrixEdge.ElemAddress(v1, 0);//获取矩阵起始第v行，0列元素地址
	for (int cur = v2+1; cur < this->m_nVexNum; cur++)
	{	// 查找邻接点
		if (pVAdr[cur] != noEdge)
		{
			weight = pVAdr[cur];
			return cur;
		}
	}
	return -1;
}

template <class ElemType, class EdgeWType>
//操作结果：重载赋值运算符
AdjMatrixGraph<ElemType, EdgeWType>& AdjMatrixGraph<ElemType, EdgeWType>::operator =(const AdjMatrixGraph<ElemType, EdgeWType>& source)
{
	int nVexNum = source.GetVexNum();
	GraphBase<ElemType, ElemType, EdgeWType>::operator=(source);//调用基类的赋值运算符，复制顶点数组等
	noEdge = source.noEdge;		//复制无边权值
	m_matrixEdge = source.m_matrixEdge;//复制边矩阵
	return *this;

}
template <class ElemType, class EdgeWType>
//操作结果：Floyd算法生成最短路径
//参数: matrixD-D矩阵
//		matrixPre-pre矩阵
//返回：是否成功
bool AdjMatrixGraph<ElemType, EdgeWType>::Floyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const
{
	if (this->m_type < UNDIR_NETWORK)
		return false;//只有网才能生成最短路径

	int nVexNum = this->m_nVexNum;
	if (matrixD.GetCols() != nVexNum || matrixD.GetRows() != nVexNum)
	{//如果D矩阵尺寸不对，调整尺寸
		matrixD.ReSize(nVexNum, nVexNum);
	}
	if (matrixPre.GetCols() != nVexNum || matrixPre.GetRows() != nVexNum)
	{//如果Pre矩阵尺寸不对，调整尺寸
		matrixPre.ReSize(nVexNum, nVexNum);
	}

	int i, j, k;
	for (i = 0; i < nVexNum; ++i) {
		for (j = 0; j < nVexNum; ++j) {
			matrixD[i][j] = (i == j) ? 0 : m_matrixEdge[i][j];
			matrixPre[i][j] = (m_matrixEdge[i][j] == noEdge) ? -1 : i;
		}
	}
	for (k = 0; k < nVexNum; ++k)
		for (i = 0; i < nVexNum; ++i)
			for (j = 0; j < nVexNum; ++j)
				if (matrixD[i][k] != noEdge && matrixD[k][j] != noEdge
					&& matrixD[i][k] + matrixD[k][j] < matrixD[i][j]) {
					matrixD[i][j] = matrixD[i][k] + matrixD[k][j];
					matrixPre[i][j] = matrixPre[k][j];
				}
	return true;

}
template <class ElemType, class EdgeWType>
//操作结果：打印输出Floyd算法生成的最短路径
//参数: matrixD-D矩阵
//		matrixPre-pre矩阵
//返回：是否成功
bool AdjMatrixGraph<ElemType, EdgeWType>::PrintFloyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const
{
	int nVexNum = this->m_nVexNum;
	if (matrixD.GetCols() != nVexNum || matrixD.GetRows() != nVexNum ||
		matrixPre.GetCols() != nVexNum || matrixPre.GetRows() != nVexNum)
		return false;				//如果矩阵尺寸不对，返回失败
	int i, j, k;					//定义临时变量
	cout << "shortest path：" << endl;
	for (i = 0; i < nVexNum; i++)
		for (j = 0; j < nVexNum; j++)
		{
			if (matrixD[i][j] == noEdge)
				continue;				//无路径，继续
			//打印<Vi,Vj>最短路径长度
			cout << '<' << this->m_pNodeTabel[i] << ','this->m_pNodeTabel[j] << ">:" << m_matrixEdge[i][j] << endl;
			cout << this->m_pNodeTabel[i] << "->";	//打印Vi->
			k = i;				//前驱从i开始,查找Vj的前驱
			while (matrixPre[k][j] != k)		//Pre矩阵中，前驱为自己时，已查到路径结束位置
			{
				cout << this->m_pNodeTabel[k] << "->";	//打印前驱->
				k = matrixPre[k][j];			//变更前驱
			}
			cout << this->m_pNodeTabel[j] << endl;	//打印Vj
		}
	return true;
}

#endif
#if !defined(_ADJ_MATRIX_GRAPH_H_)
#define _ADJ_MATRIX_GRAPH_H_
#include "graph_base.h"
#include "matrix.h"
using namespace std;
template <class ElemType, class EdgeWType>
class AdjMatrixGraph :public GraphBase<ElemType, ElemType, EdgeWType>
{
public:
	AdjMatrixGraph(ElemType vex[], int nVexNum, GRAPH_TYPE tGraph);//���캯��
	~AdjMatrixGraph();//��������
	//���ػ����ж���Ĵ��麯��
	virtual int FirstAdjVex(int v, EdgeWType& weight) const ;			// ���ض���v�ĵ�һ���ڽӵ㣬����øñߵ�Ȩ��
	virtual int NextAdjVex(int v1, int v2, EdgeWType& weight) const;	// ���ض���v1�������v2����һ���ڽӵ㣬����øñ�Ȩ��	 
	virtual void InsertEdge(int v1, int v2, EdgeWType weight) ;			// ���붥��Ϊv1��v2,ȨΪweight�ı�			 
	virtual void DeleteEdge(int v1, int v2) ;							// ɾ������Ϊv1��v2�ı�		
	AdjMatrixGraph(const AdjMatrixGraph<ElemType, EdgeWType>& source);	// ���ƹ��캯��
	AdjMatrixGraph<ElemType, EdgeWType>& operator =(const AdjMatrixGraph<ElemType, EdgeWType>& source);//���ظ�ֵ�����
	bool Floyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const;		//Floyd�㷨�������·��
	bool PrintFloyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const;		//��ӡ���Floyd�㷨���ɵ����·��
private:
	//���ػ����ж����˽�д��麯��
	virtual void GetNodeElem(const ElemType& node, ElemType& e) const;//���ݽڵ����ͻ�ȡԭ������
	virtual void SetNodeElem(ElemType& node, const  ElemType& e);	//��ԭ�����ݴ��뵽�ڵ�����������
	EdgeWType noEdge;
	Matrix<EdgeWType> m_matrixEdge;//�߾���
};
template <class ElemType, class EdgeWType>
AdjMatrixGraph<ElemType, EdgeWType>::AdjMatrixGraph(ElemType vex[], int nVexNum, GRAPH_TYPE tGraph)
	: GraphBase<ElemType, ElemType, EdgeWType>(vex, nVexNum, tGraph), m_matrixEdge(nVexNum,nVexNum)
{
	noEdge = 0;//�ޱߵ�Ȩֵ
	if (tGraph == DIR_NETWORK || tGraph == UNDIR_NETWORK)
	{//�������ޱߵ�ȨֵΪ�����
		noEdge = this->InfniteWeight();
	}

	int col, row;
	for (row = 0; row < nVexNum; row++)
	{//���þ����ֵΪ�ޱ�״̬
		EdgeWType* pMatrixAddr = m_matrixEdge.ElemAddress(row, 0);//��ȡ������ʼ��row��Ԫ����ʼ��ַ
		for (col = 0; col < nVexNum; col++)
		{
			pMatrixAddr[col] = noEdge;
		}
	}
}
template <class ElemType, class EdgeWType>
AdjMatrixGraph<ElemType, EdgeWType>::AdjMatrixGraph(const AdjMatrixGraph<ElemType, EdgeWType>& source)
{
	*this = source;//����������'='�����
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
//������������붥��Ϊv1��v2,ȨΪweight�ı�
void AdjMatrixGraph<ElemType, EdgeWType>::InsertEdge(int v1, int v2, EdgeWType weight)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//���㲻��ȷ������
	if (this->m_type == UNDIR_GRAPH || this->m_type == DIR_GRAPH)
		weight = 1;//����ͼ��ȨΪ1
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//��������������ͼ���ǶԳƾ���
		if (m_matrixEdge[v1][v2] == noEdge && m_matrixEdge[v2][v1] == noEdge)
		{
			m_matrixEdge[v1][v2] = weight;
			m_matrixEdge[v2][v1] = weight;
			this->m_nEdgeNum++;//�ߵ���Ŀ��1
		}
	}
	else
	{//����ͼ����
		if (m_matrixEdge[v1][v2] == noEdge)
		{//�ޱ�
			m_matrixEdge[v1][v2] = weight;
			this->m_nEdgeNum++;//�ߵ���Ŀ��1
		}
	}
}
template <class ElemType, class EdgeWType>
// ���������ɾ������Ϊv1��v2�ı�
void AdjMatrixGraph<ElemType, EdgeWType>::DeleteEdge(int v1, int v2)
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return;//���㲻��ȷ������
	if (this->m_type == UNDIR_NETWORK || this->m_type == UNDIR_GRAPH)
	{//��������������ͼ���ǶԳƾ���

		if (m_matrixEdge[v1][v2] != noEdge || m_matrixEdge[v2][v1] != noEdge)
		{
			m_matrixEdge[v1][v2] = noEdge;
			m_matrixEdge[v2][v1] = noEdge;
			this->m_nEdgeNum--;//�ߵ���Ŀ��1
		}
	}
	else
	{
		//����ͼ����
		if (m_matrixEdge[v1][v2] != noEdge)
		{//�б�
			m_matrixEdge[v1][v2] = noEdge;
			this->m_nEdgeNum--;//�ߵ���Ŀ��1
		}
	}
}
template <class ElemType, class EdgeWType>
// ������������ض���v�ĵ�һ���ڽӵ㣬����øñߵ�Ȩ��
int AdjMatrixGraph<ElemType, EdgeWType>::FirstAdjVex(int v, EdgeWType& weight) const
{
	if (v < 0 || v >= this->m_nVexNum )
		return -1;//���㲻��ȷ������ʧ�ܣ��������Ϊ-1��
	EdgeWType* pVAdr = m_matrixEdge.ElemAddress(v, 0);//��ȡ������ʼ��v�У�0��Ԫ�ص�ַ
	for (int cur = 0; cur <this->m_nVexNum; cur++)
	{	// �����ڽӵ�
		if (pVAdr[cur] != noEdge)
		{
			weight = pVAdr[cur];
			return cur;
		}
	}
	return -1;
}
template <class ElemType, class EdgeWType>
// ������������ض���v1�������v2����һ���ڽӵ㣬����øñ�Ȩ��
int AdjMatrixGraph<ElemType, EdgeWType>::NextAdjVex(int v1, int v2, EdgeWType& weight) const
{
	if (v1 < 0 || v2 < 0 || v1 >= this->m_nVexNum || v2 >= this->m_nVexNum)
		return -1;//���㲻��ȷ������
	if (v1 == v2)
		return -1;//v1����Ϊv2
	EdgeWType* pVAdr = m_matrixEdge.ElemAddress(v1, 0);//��ȡ������ʼ��v�У�0��Ԫ�ص�ַ
	for (int cur = v2+1; cur < this->m_nVexNum; cur++)
	{	// �����ڽӵ�
		if (pVAdr[cur] != noEdge)
		{
			weight = pVAdr[cur];
			return cur;
		}
	}
	return -1;
}

template <class ElemType, class EdgeWType>
//������������ظ�ֵ�����
AdjMatrixGraph<ElemType, EdgeWType>& AdjMatrixGraph<ElemType, EdgeWType>::operator =(const AdjMatrixGraph<ElemType, EdgeWType>& source)
{
	int nVexNum = source.GetVexNum();
	GraphBase<ElemType, ElemType, EdgeWType>::operator=(source);//���û���ĸ�ֵ����������ƶ��������
	noEdge = source.noEdge;		//�����ޱ�Ȩֵ
	m_matrixEdge = source.m_matrixEdge;//���Ʊ߾���
	return *this;

}
template <class ElemType, class EdgeWType>
//���������Floyd�㷨�������·��
//����: matrixD-D����
//		matrixPre-pre����
//���أ��Ƿ�ɹ�
bool AdjMatrixGraph<ElemType, EdgeWType>::Floyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const
{
	if (this->m_type < UNDIR_NETWORK)
		return false;//ֻ���������������·��

	int nVexNum = this->m_nVexNum;
	if (matrixD.GetCols() != nVexNum || matrixD.GetRows() != nVexNum)
	{//���D����ߴ粻�ԣ������ߴ�
		matrixD.ReSize(nVexNum, nVexNum);
	}
	if (matrixPre.GetCols() != nVexNum || matrixPre.GetRows() != nVexNum)
	{//���Pre����ߴ粻�ԣ������ߴ�
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
//�����������ӡ���Floyd�㷨���ɵ����·��
//����: matrixD-D����
//		matrixPre-pre����
//���أ��Ƿ�ɹ�
bool AdjMatrixGraph<ElemType, EdgeWType>::PrintFloyd(Matrix<EdgeWType>& matrixD, Matrix<int>& matrixPre) const
{
	int nVexNum = this->m_nVexNum;
	if (matrixD.GetCols() != nVexNum || matrixD.GetRows() != nVexNum ||
		matrixPre.GetCols() != nVexNum || matrixPre.GetRows() != nVexNum)
		return false;				//�������ߴ粻�ԣ�����ʧ��
	int i, j, k;					//������ʱ����
	cout << "shortest path��" << endl;
	for (i = 0; i < nVexNum; i++)
		for (j = 0; j < nVexNum; j++)
		{
			if (matrixD[i][j] == noEdge)
				continue;				//��·��������
			//��ӡ<Vi,Vj>���·������
			cout << '<' << this->m_pNodeTabel[i] << ','this->m_pNodeTabel[j] << ">:" << m_matrixEdge[i][j] << endl;
			cout << this->m_pNodeTabel[i] << "->";	//��ӡVi->
			k = i;				//ǰ����i��ʼ,����Vj��ǰ��
			while (matrixPre[k][j] != k)		//Pre�����У�ǰ��Ϊ�Լ�ʱ���Ѳ鵽·������λ��
			{
				cout << this->m_pNodeTabel[k] << "->";	//��ӡǰ��->
				k = matrixPre[k][j];			//���ǰ��
			}
			cout << this->m_pNodeTabel[j] << endl;	//��ӡVj
		}
	return true;
}

#endif
#if !defined(_GRAPH_BASE_H_)
#define _GRAPH_BASE_H_
#include <stdlib.h>
#include <memory.h>
#include<iostream>
#include "lk_stack.h"
#include "FindUnionSet.h"
#include "min_priority_heap_queue.h"
#include "SqList.h"
#include "lk_queue.h"
using namespace std;
template <class EdgeWType>
struct GraphEdge {			// ���ı߽������
	int vex1, vex2;
	EdgeWType weight;		// �ߵ���Ԫ��(ʼ�㣬�յ㣬Ȩֵ)
	bool operator<(const  GraphEdge& e) const {  						// ʹ�����ȼ�������Ҫ����<
		return weight < e.weight;
	}
	bool operator<=(const  GraphEdge& e) const {  						// ʹ�����ȼ�������Ҫ����<
		return weight <= e.weight;
	}
	bool operator>(const  GraphEdge& e) const {  						// ʹ�����ȼ�������Ҫ����<
		return weight > e.weight;
	}
	bool operator>=(const  GraphEdge& e) const {  						// ʹ�����ȼ�������Ҫ����<
		return weight >= e.weight;
	}

};
enum GRAPH_TYPE {//����ͼ������
	UNDIR_GRAPH = 0,//����ͼ
	DIR_GRAPH,//����ͼ
	UNDIR_NETWORK,//������
	DIR_NETWORK,//������
};
template <class ElemType,class NodeType,class EdgeWType>
class GraphBase
{

public:
	GraphBase(ElemType e[],int nVexNum, GRAPH_TYPE tGraph);
	~GraphBase();
	virtual int FirstAdjVex(int v, EdgeWType& weight) const = 0;			// ���ض���v�ĵ�һ���ڽӵ㣬����øñߵ�Ȩ��
	virtual int NextAdjVex(int v1, int v2, EdgeWType &weight) const = 0;	// ���ض���v1�������v2����һ���ڽӵ㣬����øñ�Ȩ��	 
	virtual void InsertEdge(int v1, int v2,EdgeWType weight) =0;			// ���붥��Ϊv1��v2,ȨΪweight�ı�			 
	virtual void DeleteEdge(int v1, int v2) =0;			// ɾ������Ϊv1��v2�ı�		
	virtual EdgeWType InfniteWeight();				// �ߵ������Ȩֵ
	bool GetTag(int v) const;							// ���ض���v�ı�־		 
	void SetTag(int v, bool val) ;						// ���ö���v�ı�־Ϊval	
	int GetVexNum() const;								// ���ض������		
	int GetEdgeNum() const;								// ���ر�������			 
	void DFSTraverse(void (*visit)(const ElemType& e)) const;	// ��ͼ����������ȱ���
	void BFSTraverse(void (*visit)(const ElemType&)) const;	// ��ͼ���й�����ȱ���
	bool GetElem(int v, ElemType& e) const;				// ��e���ض���v��Ԫ��ֵ	
	bool SetElem(int v, const ElemType& e);				// ���ö���v��Ԫ��ֵΪe
	bool Prim();										//������ķ�㷨����С������
	bool Kruskal();										//�ÿ�³˹�����㷨������С������
	GraphEdge <EdgeWType>* GetMstEdge(int& nMsgEdgeNum) const;	//�����С������
	bool TopSort(SqList<int>& lstSortedVex) const;				//��������
	bool CriticalPath(SqList<GraphEdge<EdgeWType>>& lstCritalPathEdge) const;		//�����Ĺؼ�·��
	bool Dijkstra(int start, EdgeWType D[], int pre[], int nBufSize); //�õϽ�˹�����㷨�����·��
	GraphBase<ElemType, NodeType, EdgeWType>& operator =(const GraphBase<ElemType, NodeType, EdgeWType>& source);//���ظ�ֵ�����
	int InsertEdges(const GraphEdge<EdgeWType> edge[], int nEdgeNums);		//���ݱ����в��������
protected:
	GraphEdge <EdgeWType>* m_pMstEdge;				// ��С�������ı߼�
	// ��������ģ��:
	void DFSAux(int v, void (*visit)(const ElemType& e)) const;		// �Ӷ���v�������������������ͼ
	void BFSAux(int v, void (*visit)(const ElemType&)) const;		// �ӵڶ���v�������й����������ͼ
	virtual void GetNodeElem(const NodeType& node,ElemType &e) const =0;//���ݽڵ����ͻ�ȡԭ������
	virtual void SetNodeElem(NodeType& node,const  ElemType& e)  = 0;	//��ԭ�����ݴ��뵽�ڵ�����������
	void ClearTag();//��ն�����ʱ��
	void ReSize(int nVexNum);//���·���洢�ռ�
	void StatInDegree(int inDegree[]) const;							//ͳ�ƶ������
	bool* m_pbVistedTabel;//�ڵ���ʱ�־��
	NodeType* m_pNodeTabel;//�ڵ��
	int m_nVexNum;//�ڵ���Ŀ
	int m_nEdgeNum;//����
	int m_nMstEdgeNum;//��С�������߸���
	GRAPH_TYPE m_type;//ͼ������
};
template <class ElemType, class NodeType, class EdgeWType>
//�����������ԭ�������������й���ͼ
GraphBase<ElemType,NodeType, EdgeWType>::GraphBase(ElemType e[],int nVexNum, GRAPH_TYPE tGraph)
{
	m_pNodeTabel = new NodeType[nVexNum];		//������������
	m_pbVistedTabel = new bool[nVexNum];		//�������ʱ�־����
	m_pMstEdge = NULL;							//��С��������ʼ��Ϊ��
	for (int i = 0; i < nVexNum; i++)				
		SetElem(i, e[i]);						//�����ڵ�ԭ������
	memset(m_pbVistedTabel, 0, sizeof(bool) * nVexNum);	//��շ��ʱ�־
	m_nVexNum = nVexNum;						//��ֵ������
	m_nEdgeNum = 0;								//������ʼ��Ϊ0
	m_nMstEdgeNum = 0;							//��С������������ʼ��Ϊ0
	m_type = tGraph;							//����ͼ�����ԣ������������ȣ�
}

template <class ElemType, class NodeType, class EdgeWType>
GraphBase<ElemType, NodeType, EdgeWType>::~GraphBase()
{
	if (m_pNodeTabel)
	{
		delete[]m_pNodeTabel;
		m_pNodeTabel = NULL;
	}
	if (m_pbVistedTabel)
	{
		delete[]m_pbVistedTabel;
		m_pbVistedTabel = NULL;
	}
	if (m_pMstEdge)
	{
		delete []m_pMstEdge;
		m_pMstEdge = NULL;

	}
	m_nVexNum = 0;
	m_nEdgeNum = 0;
	m_nMstEdgeNum = 0;
}

template <class ElemType, class NodeType, class EdgeWType>
//������������·��䶥��洢�ռ�
void GraphBase<ElemType, NodeType, EdgeWType>::ReSize(int nVexNum)
{
	if (m_pNodeTabel)
	{
		delete[]m_pNodeTabel;
	}
	if (m_pbVistedTabel)
	{
		delete[]m_pbVistedTabel;
	}
	if (m_pMstEdge)
	{
		delete[]m_pMstEdge;
		m_pMstEdge = NULL;

	}
	m_pNodeTabel = new NodeType[nVexNum];
	m_pbVistedTabel = new bool[nVexNum];
	memset(m_pbVistedTabel, 0, sizeof(bool) * nVexNum);//��շ��ʱ�־
	m_nVexNum = nVexNum;
	m_nEdgeNum = 0;
	m_nMstEdgeNum = 0;
}
template <class ElemType, class NodeType, class EdgeWType>
//������������ݱ����в��������
// ������edge-������
//		nEdge-�������С
//���أ���ǰ����
int GraphBase<ElemType, NodeType, EdgeWType>::InsertEdges(const GraphEdge<EdgeWType> edge[], int nEdgeNums)
{
	int i;
	for (i = 0; i < nEdgeNums; i++)
		InsertEdge(edge[i].vex1, edge[i].vex2, edge[i].weight);
	return m_nEdgeNum;
}
	
template <class ElemType, class NodeType, class EdgeWType>
//��������� ��e���ض���v��Ԫ��ֵ
bool GraphBase<ElemType, NodeType, EdgeWType>::GetElem(int v, ElemType& e) const
{
	if (v < 0 || v >= m_nVexNum || !m_pNodeTabel)
		return false;
	GetNodeElem(m_pNodeTabel[v],e);
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
//��������� ����v��Ԫ��ֵe
bool GraphBase<ElemType, NodeType, EdgeWType>::SetElem(int v, const ElemType& e)
{
	if (v < 0 || v >= m_nVexNum || !m_pNodeTabel)
		return false;
	SetNodeElem(m_pNodeTabel[v], e);
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::ClearTag()
{
	if (!m_pbVistedTabel)
		return;
	memset(m_pbVistedTabel, 0, sizeof(bool) * m_nVexNum);//������ʱ������ڴ�ռ�����
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::DFSAux(int v, void (*visit)(const ElemType& e)) const
// ��ʼ����������ͼ
// ����������Ӷ���v�������������������ͼ���ݹ����ʱ�����ͨ��ͼ�ķ���
{
	SetTag(v, true);							// ���÷��ʱ�־
	ElemType e;									// ��ʱ����
	GetElem(v, e);								// ȡ����v������Ԫ��
	(*visit)(e);								// ���ʶ���v������Ԫ��
	EdgeWType weight;							// �ߵ�Ȩֵ��ʱ����
	for (int w = FirstAdjVex(v,weight); w != -1; w = NextAdjVex(v, w, weight))
	{	// ��v����δ���ʹ����ڽӶ���w�ݹ����DFSAux
		if (!GetTag(w))	DFSAux(w, visit);
	}
}
template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::DFSTraverse(void (*visit)(const ElemType& e)) const
// ��ʼ����������ͼ
// �����������ͼ����������ȱ���
{
	int v;
	ClearTag();//��ն�����ʱ��
	
	for (v = 0; v < m_nVexNum; v++)
	{	// ����δ���ʵĶ��㰴DFS���������������
		if (!GetTag(v))	DFSAux(v, visit);
	}
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::BFSAux(int v, void (*visit)(const ElemType&)) const
// ��ʼ����������ͼ
// ����������ӵڶ���v�������й����������ͼ
{
	SetTag(v, true);						// ���÷��ʱ�־
	ElemType e;								// ��ʱ����
	GetElem(v, e);							// ȡ����v������Ԫ��
	(*visit)(e);							// ���ʶ���v������Ԫ��
	LinkQueue<int> q;						// �������
	q.InQueue(v);							// v���
	EdgeWType weight ;
	while (!q.IsEmpty())
	{	// ����q�ǿ�, ����ѭ��
		int u, w;							// ��ʱ����
		q.OutQueue(u);						// ����
		for (w = FirstAdjVex(u, weight); w >= 0; w = NextAdjVex(u, w, weight))
		{	// ��u��δ���ʹ����ڽӶ���w���з���
			if (!GetTag(w))
			{	// ��w���з���
				SetTag(w, true);			// ���÷��ʱ�־
				GetElem(w, e);				// ȡ����w������Ԫ��
				(*visit)(e);				// ���ʶ���w������Ԫ��
				q.InQueue(w);				// w���
			}
		}
	}
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::BFSTraverse(void (*visit)(const ElemType&)) const
// ��ʼ����������ͼ
// �����������ͼ���й�����ȱ���
{
	ClearTag();//��ն�����ʱ��
	int v;
	for (v = 0; v < m_nVexNum; v++)
	{	// ����δ���ʵĶ��㰴BFS���������������
		if (!GetTag(v)) BFSAux(v, visit);
	}
}


template <class ElemType, class NodeType, class EdgeWType>
int GraphBase<ElemType, NodeType, EdgeWType>::GetVexNum() const
// ������������ض������			 
{
	return m_nVexNum;
}
template <class ElemType, class NodeType, class EdgeWType>
// ������������ر���	
int GraphBase<ElemType, NodeType, EdgeWType>::GetEdgeNum() const
{
	return m_nEdgeNum;
}
template <class ElemType, class NodeType, class EdgeWType>
bool GraphBase<ElemType, NodeType, EdgeWType>::GetTag(int v) const
// ������������ض���v�ı�־		 
{
	if (v < 0 || v >= m_nVexNum)
	{	// �����쳣
		cout << "v���Ϸ�!" << endl;					// ��ʾ��Ϣ
		exit(12);									// �˳�����
	}
	
	return m_pbVistedTabel[v];// ���ض���v�ı�־
}

template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::SetTag(int v, bool val)
// ������������ö���v�ı�־Ϊval		 
{
	if (v < 0 || v >= m_nVexNum)
	{	// �����쳣
		cout << "v���Ϸ�!" << endl;					// ��ʾ��Ϣ
		exit(13);									// �˳�����
	}

	m_pbVistedTabel[v]=val;									// ���ö���v�ı�־Ϊval
}
template <class ElemType, class NodeType, class EdgeWType>
//�������:��Prim�㷨������С������
bool GraphBase<ElemType, NodeType, EdgeWType>::Prim() 
{
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 || m_type != UNDIR_NETWORK)
		return false;//�����ڶ����ߣ��Լ�����������������ʧ��
	struct Dist {
		int  adjVex;		// ��С���۱������Ķ�����(<�����±�,adjVex>��ɵı�,����adjVex����Vnew��)
		EdgeWType  lowCost;	// ��С����
	}*D = new Dist[m_nVexNum];//DΪVnew�ĺ�ѡ���㼯�ϣ��������lowCostΪ�������ö���Ϊ�Ǻ�ѡ����
	
	ClearTag();//��ն�����ʱ��
	if (m_pMstEdge)
		delete[]m_pMstEdge;//����������С�������ߣ�ɾ������
	m_pMstEdge = new GraphEdge<EdgeWType>[m_nVexNum];
	EdgeWType minCost;	//��С������ʱ����
	int u,w, i, j, count = 0;
	EdgeWType InfniteW = InfniteWeight();//��ȡ��Ȩֵ�������ֵ
	for (i = 0; i < m_nVexNum; ++i) {
		
		D[i].lowCost = InfniteW;//��ʼ��ʱ������С���۶���ֵΪ�����
	}
	// ��ʼ��Vnew����
	u = 0; 
	SetTag(u,true) ;  	
	EdgeWType weight;
	for (i = 1; i < m_nVexNum; ++i) 
	{	// ѡ��һ����u��Vnew���Ϻ�,�����������
		for (w = FirstAdjVex(u, weight); w>=0; w=NextAdjVex(u, w, weight))  // ���º�u�����ģ�V-Vnew�������ڽӵ��Ȩֵ
			if (!GetTag(w) && D[w].lowCost > weight)
			{//w��(V-Vnew)���ϣ��ұ�(w,D[W].AdjVex)Ȩ>��(u,w)Ȩ
					D[w].lowCost = weight;		// ����lowcost
					D[w].adjVex = u; 			// ����adjVex 
			}
		minCost = InfniteW;
		for (j = 0; j < m_nVexNum; ++j)			// �ں�ѡ���㼯������lowCost��С����u
			if (D[j].lowCost < minCost)
			{
				minCost = D[j].lowCost;  u = j;
			}
		m_pMstEdge[count].vex1 = D[u].adjVex; 		// ������С��������һ����
		m_pMstEdge[count].vex2 = u;
		m_pMstEdge[count++].weight = D[u].lowCost;
		D[u].lowCost = InfniteW;					// ����u�Ѳ���Vnew����
		SetTag(u, true);//����u�Ѿ�����
	}
	delete[] D;
	m_nMstEdgeNum = count;
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
//�������:��Kruskal�㷨������С������
bool GraphBase<ElemType, NodeType, EdgeWType>::Kruskal()
{
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 || m_type != UNDIR_NETWORK)
		return false;//�����ڶ����ߣ��Լ�����������������ʧ��
	if (m_pMstEdge)
	{
		delete[]m_pMstEdge;
	}
	m_pMstEdge = new GraphEdge<EdgeWType>[m_nVexNum];

	int count = 0;
	GraphEdge<EdgeWType> e;
	
	FindUnionSet S(m_nVexNum);    		// ���鼯S
	MinPriorityHeapQueue <GraphEdge<EdgeWType>> Q; 		// ���ȼ�����Q
	int i,w;
	EdgeWType weight;
	for (int i = 0; i < m_nVexNum; ++i) 
	{	// �������ȼ�����
		for (w = FirstAdjVex(i, weight); w >= 0;w= NextAdjVex(i, w, weight))  // ����u�����Ķ����Dֵ
			if (i < w) 
			{			// ��ֹ�ظ����
				e.vex1 = i;
				e.vex2 = w;
				e.weight = weight;
				Q.InQueue(e); 		// ��e���
			}
	}
	while (count < m_nVexNum - 1) 
	{	// ѡ��m_nVexNum-1����
		 if(!Q.OutQueue(e)) break;		// �����ȼ����г���һ����
		if (!S.IsSameTree(e.vex1,e.vex2)) 
		{	// ���ϵ��������㲻����ͬһ��ͨ����
			S.Union(e.vex1, e.vex2); 		// �ϲ�e.vex1, e.vex2�����Ӽ�����ͨ������
			m_pMstEdge[count++] = e;		// ������С�������ϵ�һ����
		}
	}
	m_nMstEdgeNum = count;
	return true;
}
template <class ElemType, class NodeType, class EdgeWType>
//�������:�����С������
GraphEdge <EdgeWType>* GraphBase<ElemType, NodeType, EdgeWType>::GetMstEdge(int& nMsgEdgeNum) const
{
	nMsgEdgeNum = m_nMstEdgeNum;
	return m_pMstEdge;
}
template <class ElemType, class NodeType, class EdgeWType>
void GraphBase<ElemType, NodeType, EdgeWType>::StatInDegree(int inDegree[]) const
//�������:ͳ�����
{
	int i, w;//��ʱ����
	EdgeWType weight;//Ȩֵ��ʱ����
	for (i = 0; i < m_nVexNum; i++)
		inDegree[i] = 0;
	for (i = 0; i < m_nVexNum; i++)
	{	 // �����߱��󶥵����
		for (w = FirstAdjVex(i, weight); w >= 0; w = NextAdjVex(i, w, weight))
			++inDegree[w];
	}
}
template <class ElemType, class NodeType, class EdgeWType>
//�������:��������,����õĶ����±������lstSortedVex���Ա��У������Ƿ�����ɹ�
bool GraphBase<ElemType, NodeType, EdgeWType>::TopSort(SqList<int>& lstSortedVex) const
{
	if (m_type == UNDIR_NETWORK || m_type == UNDIR_GRAPH)
		return false;//����ͼ���ܽ�����������
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 )
		return false;//�����ڶ����ߣ�����ʧ��
	LinkStack <int> topSortS;			// ������������洢���Ϊ0�Ķ���ջ
	
	int i, curNode, count = 0,w;
	EdgeWType weight;					//Ȩֵ��ʱ����
	int *inDegree = new int[m_nVexNum];	//�������
	StatInDegree(inDegree);				//ͳ�ƶ������
	for (i = 0; i < m_nVexNum; i++) 	// ���Ϊ0�Ķ�����ջ
		if (inDegree[i] == 0) topSortS.Push(i);
	while (!topSortS.IsEmpty())
	{
		if (!topSortS.Pop(curNode))		// ��ջһ��Ϊ0�Ķ��㣬ʧ��ʱ�˳�ѭ��
			break;
		lstSortedVex.AddTail(curNode);	//����������뵽���Ա���
		count++;				// ������+1
		for (w = FirstAdjVex(curNode, weight); w >= 0; w = NextAdjVex(curNode, w, weight))
			if (--inDegree[w] == 0) 	// �ڽӵ���ȼ�1
				topSortS.Push(w); 			// ���Ϊ0�Ķ�����ջ
	}
	delete []inDegree;
	if (count == m_nVexNum)
		return true; 			// ���ȫ�����㣬��������ɹ�
	return false; 			// ������ͼ�л�����������ʧ��
}
template <class ElemType, class NodeType, class EdgeWType>
bool GraphBase<ElemType, NodeType, EdgeWType>::CriticalPath(SqList<GraphEdge<EdgeWType>>& lstCritalPathEdge) const
//��������������Ĺؼ�·��
//������lstCritalPathEdge - ����ؼ�·��������
//���أ������Ƿ�ɹ�
{
	if (m_type != DIR_NETWORK )		return false;//������������ؼ�·��
	if (m_nVexNum <= 0 || !m_pNodeTabel || m_nEdgeNum <= 0 )	return false;//�����ڶ����ߣ�����ʧ��
	int* inDegree = new int[m_nVexNum];	// �����������
	StatInDegree(inDegree);				//ͳ�ƶ������
	int* ve = new int[m_nVexNum];		// �¼����緢��ʱ������
	LinkStack<int> topSortS;			// ������������洢���Ϊ0�Ķ���ջ
	LinkStack<int> reverseS;			// ����ʵ�����������е�ջ
	int ee, el, u, v, count = 0;
	EdgeWType weight;					//Ȩֵ
	for (v = 0; v < m_nVexNum; v++)
	{	// ��ʼ���¼����緢��ʱ��
		ve[v] = 0;
	}

	for (v = 0; v < m_nVexNum; v++)
	{	// ��������
		if (inDegree[v] == 0)
		{	// �������Ϊ0�Ķ���ջ
			topSortS.Push(v);					// ��ջ
		}
	}

	while (!topSortS.IsEmpty())
	{	// ջ�ǿ�
		topSortS.Pop(u);						// ȡ��һ�����Ϊ0�Ķ���
		reverseS.Push(u);						// ����u��ջ���Ա����������������
		count++;								// �Զ�����м���
		for (v = FirstAdjVex(u, weight); v != -1; v = NextAdjVex(u, v, weight))
		{	// v2Ϊv1��һ���ڽӵ�,��v1��ÿ���ڽӵ���ȼ�1
			if (--inDegree[v] == 0)
			{	// v���Ϊ0����v��ջ
				topSortS.Push(v);				// ��ջ 
			};
			if (ve[u] + weight > ve[v])
			{	// �޸�ve[v]
				ve[v] = ve[u] + weight;
			}
		}
	}
	delete[]inDegree;							// �ͷ�inDegree��ռ�õĴ洢�ռ�

	if (count < m_nVexNum)
	{
		delete[]ve;							// �ͷ�ve��ռ�õĴ洢�ռ�
		return false;							// ��g�л�·
	}

	int* vl = new int[m_nVexNum];			// �¼���ٷ���ʱ������
	reverseS.Top(u);							// ȡ��ջ����ջ��Ϊ���
	for (v = 0; v < m_nVexNum; v++)
	{	// ��ʼ���¼���ٷ���ʱ��
		vl[v] = ve[u];
	}

	while (!reverseS.IsEmpty())
	{	// reverseS�ǿ�
		reverseS.Pop(u);						// ��ջ
		for (v = FirstAdjVex(u,weight); v != -1; v = NextAdjVex(u, v,weight))
		{	// vΪu��һ���ڽӵ�
			if (vl[v] - weight < vl[u])
			{	// �޸�vl[u]
				vl[u] = vl[v] - weight;
			}
		}
	}
	GraphEdge<EdgeWType> edge;			//����ʱ����
	for (u = 0; u < m_nVexNum; u++)
	{	// ��ee, el�͹ؼ�·��
		for (v = FirstAdjVex(u, weight); v != -1; v =NextAdjVex(u, v, weight))
		{	// vΪu��һ���ڽӵ�
			ee = ve[u]; el = vl[v] - weight;
			if (ee == el)
			{	// <u, v>Ϊ�ؼ��,����ؼ��
				edge.vex1 = u;
				edge.vex2 = v;
				edge.weight = weight;
				lstCritalPathEdge.AddTail(edge);
				//cout << "<" << u << "," << v << "> ";	// ����ؼ��
			}
		}
	}

	delete[]ve;							// �ͷ�ve��ռ�õĴ洢�ռ�
	delete[]vl;							// �ͷ�vl��ռ�õĴ洢�ռ�
	return true;							// �����ɹ�
}

template <class ElemType, class NodeType, class EdgeWType>
//�����������dijkstra���������·��
//����: start-��ʼ�������
//		 D[] - ��¼���·�����ȵ�����
//		 pre[] -��¼ǰ��������
//		 nBufSize - D�����pre����ĳ���
//����:  �����Ƿ�ɹ�
bool GraphBase<ElemType, NodeType, EdgeWType>::Dijkstra(int start, EdgeWType D[],int pre[],int nBufSize) 
{
	if (start < 0 || start > m_nVexNum - 1)	//Դ���±�Խ��
		return false;
	if (!D || !pre || nBufSize < m_nVexNum)
		return false;						//����������
	if (m_type != DIR_NETWORK)
		return false;						//ֻ�����������������·��
	ClearTag();//��ն�����ʱ�־
	EdgeWType min;   
	int i, j, k, w;
	EdgeWType weight;
	EdgeWType InfniteW = InfniteWeight();//��ȡ��Ȩֵ�������ֵ
	for (i = 0; i < m_nVexNum; ++i) 
	{	// ��ʼ��
		D[i] = InfniteW;  pre[i] = -1;
	}
	D[start] = 0;
	pre[start] = start;	// start�������·��������0
	min = D[start];  k = start;

	for (i = 1; i < m_nVexNum; ++i) 
	{
		SetTag(k, true);//���ö���k�ķ��ʱ�־
		// ˢ��start��k���ڽӵ�����·������
		for (w = FirstAdjVex(k, weight); w >= 0; w = NextAdjVex(k, w, weight))
			if (!GetTag(w) && D[w] > min + weight) 
			{//����w��û�з��ʣ���start��w�ı߾����min+weight
				D[w] = min + weight;
				pre[w] = k;
			}
		min = InfniteW; k = start;
		for (j = 0; j < m_nVexNum; ++j) // �����ҵ�ǰ·�����������δ�����ʹ��Ķ���k
			if (!GetTag(j) && D[j] < min) 
			{
				k = j;
				min = D[k];
			}
	}
	return  true;
}
template <class ElemType, class NodeType, class EdgeWType>
//������������ظ�ֵ�����
GraphBase<ElemType, NodeType, EdgeWType>& GraphBase<ElemType, NodeType, EdgeWType>::operator =(const GraphBase<ElemType, NodeType, EdgeWType>& source)
{
	if (m_nVexNum != source.m_nVexNum)
	{
		ReSize(source.m_nVexNum);
	}
	m_nEdgeNum = source.m_nEdgeNum;
	m_nVexNum = source.m_nVexNum;
	m_nMstEdgeNum = source.m_nMstEdgeNum;
	if (source.m_pMstEdge)
	{
		m_pMstEdge = new GraphEdge<EdgeWType>[source.m_nMstEdgeNum];
		memcpy(m_pMstEdge, source.m_pMstEdge, sizeof(GraphEdge<EdgeWType>) * source.m_nMstEdgeNum);//������С������
	}
	int i;
	NodeType* pSrcNodeTabel = source.m_pNodeTabel;
	for (i = 0; i < m_nVexNum; i++)
	{
		m_pNodeTabel[i] = pSrcNodeTabel[i];
	}
	memcpy(m_pbVistedTabel, source.m_pbVistedTabel, sizeof(bool) * this->m_nVexNum);//����������ʱ������
	m_type = source.m_type;//����ͼ������
	return *this;
}
template <class ElemType, class NodeType, class EdgeWType>
//������������������
EdgeWType GraphBase<ElemType, NodeType, EdgeWType>::InfniteWeight()
{
	int typeSize = sizeof(EdgeWType);
	if (typeSize == 1)
		return 127;//������8λ����
	else if (typeSize == 2)
		return 32767;//������16λ����
	else
		return 2147483647;//����32λ����
}
#endif
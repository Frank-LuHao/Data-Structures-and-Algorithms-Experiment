#if  !defined(__SORT_H__)
#define __SORT_H__
#include "sq_stack.h"
#define SWAP(ELEM0,ELEM1,TEMP) \
		{TEMP=ELEM0; ELEM0=ELEM1;ELEM1=TEMP;}

template <class ElemType>
bool StraightInsertSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)
		return false;
	int i, j;//��ʱ����
	for (i = 1; i < nSize; i++)
	{	// ��i��ֱ�Ӳ�������
		ElemType e = elem[i];	// �ݴ�elem[i]
		for (j = i - 1; j >= 0 && e < elem[j]; j--)
		{	// ����e���Ԫ�ض�����
			elem[j + 1] = elem[j];	// ����
		}
		elem[j + 1] = e;		// j+1Ϊ����λ��
	}
	return true;
}
template <class ElemType>
bool ShellSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)
		return false;
	int gap, pos, j;		// gapΪϣ��������posΪ�������¼λ��
	ElemType e;
	for (gap = (nSize >>1); gap > 0; gap >>= 1) 
	{//gap��nSize/2��ʼ��ÿ��gap��Сһ��
		for (pos = gap; pos < nSize; pos++) 
		{//��gap���������ݣ�����ֱ������
			e = elem[pos];
			for (j = pos - gap; j >= 0 && elem[j] > e; j -= gap)
				elem[j + gap] = elem[j];	// ��¼���� 
			elem[j + gap] = e;		// ���������¼�ŵ�����λ��
		}
	}
	return true;
}
template <class ElemType>
bool BubbleSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)	return false;	//��ȫ�Լ��
	int i, j;
	bool flag = true;				// ��¼һ��������Ƿ���������
	ElemType tmp;				// ���彻����ʱ����
	for (i = 1; i < nSize && flag; ++i)
	{//Ԫ��ȫ��������ɻ�û�н���ʱ����ѭ��
		flag = false;				// �ٶ���������û�н���
		for (j = 0; j < nSize - i; ++j)
			if (elem[j + 1] < elem[j])
			{// ����		
				SWAP(elem[j], elem[j + 1], tmp);	// �ú궨��SWAP���н���,�����ú�����Ч�ʸ���
				flag = true;			// ����������
			}
	}
	return true;
}

template <class ElemType>
//�������������low��high�������������ֵ��������
//����ֵ����ֵ��λ��
int QSortPartition(ElemType elem[], int low, int high)
{
	ElemType  pivotValue = elem[low];		// Ĭ���������ֵΪ��һ��Ԫ��
	if (high - low > 1)
	{//�������г���������Ԫ�أ������õ���ֵ
		int mid = (high + low) >> 1;
		ElemType eMax = elem[mid];
		ElemType eMin = elem[high];
		ElemType tmp;
		if (eMin > eMax)
			SWAP(eMax, eMin, tmp);
		if (pivotValue > eMax) pivotValue = eMax;
		else if (pivotValue < eMin) pivotValue = eMin;
	}
	while (low != high) 
	{		// ��ʼ���зָ�
		while (low < high && elem[high] >= pivotValue)high--;	// ��<��ֵ�ļ�¼
		if (low < high)
		{
			elem[low] = elem[high]; low++; // �ü�¼�ƶ���С�±��
		}	
		while (low < high && elem[low] <= pivotValue) low++;// ��>��ֵ�ļ�¼
		if (low < high)
		{
			elem[high] = elem[low]; high--;// �ü�¼�ƶ������±��
		}	
	}
	elem[low] = pivotValue;			// ����ֵ����ֽ�λ����
	return low;			// ��������λ��
}

typedef struct tagPARTITION
{
	int low;
	int high;
}PARTITION;
template <class ElemType>
//���������㷨���ǵݹ�
bool QuickSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)	return false;	//��ȫ�Լ��
	SqStack<PARTITION> stack;//��������ջ
	PARTITION partition,tmpParti;
	partition.low = 0;
	partition.high = nSize-1;//��ʼʱ��������Ϊ������������
	stack.Push(partition);
	while (!stack.IsEmpty())
	{
		stack.Pop(partition);//��ջ
		int pivotPos = QSortPartition(elem, partition.low, partition.high);//����������ֵ��������
		if (pivotPos > partition.low)
		{//���������һ���������
			tmpParti.low = partition.low;
			tmpParti.high = pivotPos - 1;
			stack.Push(tmpParti);//��ջ
		}
		if (pivotPos + 1 < partition.high)
		{//�������ұ�һ���������
			tmpParti.low = pivotPos + 1;
			tmpParti.high = partition.high;
			stack.Push(tmpParti);//��ջ
		}
	}
	return true;
}
template <class ElemType>
bool SelectSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)	return false;	//��ȫ�Լ��
	int  pos, min, j;	// minΪһ����������С��¼�±�
	// posΪ����ŵ�ǰ��С��¼��λ��
	ElemType tmp;		//tmp��SWAP����ʱ����
	for (pos = 0; pos < nSize - 1; pos++)
	{
		min = pos;		//��Сλ�ó�ʼʱ����Ϊpos
		for (j = pos + 1; j < nSize; ++j)
			if (elem[j] < elem[min]) min = j;// ������С��¼ 
		if (pos != min) SWAP(elem[pos], elem[min], tmp);
	}
}

template <class ElemType>
//�������������������ɸѡ(�����)
void siftDownMaxRoot(ElemType elem[], int pos, int nSize)
{
	int child;
	ElemType tmp = elem[pos]; 		// �ݴ桰������¼elem[pos]
	for (; (child = pos * 2 + 1) < nSize; pos = child)
	{
		if (child != nSize - 1 && elem[child + 1] > elem[child])
			child++;				// ѡȡ�������ӵĴ���		
		if (elem[child] > tmp) 		// �ϴ�ĺ��ӱ�˫�״�
			elem[pos] = elem[child];	// �ϴ�ĺ���������˫��
		else  break;
	}
	elem[pos] = tmp; 			// ��������������ȷλ��
}
template <class ElemType>
//�������������������ɸѡ(С����)
void siftDownMinRoot(ElemType elem[], int pos, int nSize)
{
	int child;
	ElemType tmp = elem[pos]; 		// �ݴ桰������¼elem[pos]
	for (; (child = pos * 2 + 1) < nSize; pos = child)
	{
		if (child != nSize - 1 && elem[child + 1] < elem[child])
			child++;				// ѡȡ�������ӵ�С��		
		if (elem[child] < tmp) 		// ��С�ĺ��ӱ�˫��С
			elem[pos] = elem[child];	// ��С�ĺ���������˫��
		else  break;
	}
	elem[pos] = tmp; 			// ��������������ȷλ��
}
template <class ElemType>
//���������������
bool heapSort(ElemType elem[], int nSize,bool bIncSeq)
{
	if (!elem || nSize < 1)	return false;	//��ȫ�Լ��
	int  i;
	ElemType tmp;//������ʱ����
	// ��ʼ���ѣ������һ����Ҷ��㿪ʼ����
	if (bIncSeq)
	{//����
		for (i = nSize / 2 - 1; i >= 0; i--)
			siftDownMaxRoot(elem, i, nSize);
		for (i = nSize - 1; i > 0; i--)
		{// ��n-1������ɾ���Ѷ�Ԫ�غ󷴸������ѣ�			
			SWAP(elem[0], elem[i], tmp);	// �����Ѷ�Ԫ���������������һ��Ԫ��
			siftDownMaxRoot(elem, 0, i);	// ��elem[0..i]���µ���Ϊ�󶥶�
		}
	}
	else
	{
		for (i = nSize / 2 - 1; i >= 0; i--)
			siftDownMinRoot(elem, i, nSize);
		for (i = nSize - 1; i > 0; i--)
		{// ��n-1������ɾ���Ѷ�Ԫ�غ󷴸������ѣ�			
			SWAP(elem[0], elem[i], tmp);	// �����Ѷ�Ԫ���������������һ��Ԫ��
			siftDownMinRoot(elem, 0, i);	// ��elem[0..i]���µ���ΪС����
		}
	}

	return true;
}
template <class ElemType>
//����������鲢����ϲ�����������
void  merge(ElemType elem[], ElemType tmpElem[], int low, int mid, int high)
{
	int i = low, j = mid, k = 0;
	while (i < mid && j <= high)		// elem�м�¼��С����ز���tmpElem
		if (elem[i] < elem[j])
			tmpElem[k++] = elem[i++]; 		// ��elem[i]��elem[j]��С�߿�����tmpElem[k]
		else tmpElem[k++] = elem[j++];
	while (i < mid) 			// ǰ��ʣ��elem[i.. mid-1]���Ƶ�tmpElem
		tmpElem[k++] = elem[i++];
	while (j <= high) 			// ���ʣ��elem[j..high]���Ƶ�tmpElem
		tmpElem[k++] = elem[j++];
	for (i = 0, k = low; k <= high; )
		elem[k++] = tmpElem[i++];		// �ź���ļ�¼��tmp����elem
}
template <class ElemType>
//����������鲢����������,�ݹ����ʵ�ֹ鲢����
void  mergeSortAux(ElemType elem[], ElemType tmpElem[], int low, int high)
{
	if (low == high) return;
	int mid = (low + high) / 2; 		// ���м仮��Ϊ����������
	mergeSortAux(elem, tmpElem, low, mid);	// �ݹ�鲢������elem[low.. mid] 
	mergeSortAux(elem, tmpElem, mid + 1, high);// �ݹ�鲢������elem[mid+1.. high]  
	merge(elem, tmpElem, low, mid + 1, high);	// �鲢����������
}
template <class ElemType>
//����������鲢����
bool MergeSort(ElemType elem[], int nSize) 
{
	if (!elem || nSize < 1)	return false;	//��ȫ�Լ��
	ElemType* tmpElem = new ElemType[nSize];	// ��������
	mergeSortAux(elem, tmpElem, 0, nSize - 1);			//���ù鲢�����������������
	delete[] tmpElem;							//ɾ����������
	return true;
}
template <class ElemType>
struct NodeTreeLoser
{
	ElemType* pElem;
	int OutCnts;
	int nSqSize;
};
template <class ElemType>
bool  operator <(NodeTreeLoser<ElemType>& first, NodeTreeLoser<ElemType>& second)
{
	return first.pElem[0] < second.pElem[0];
}
template <class ElemType>
bool  operator > (NodeTreeLoser<ElemType>& first, NodeTreeLoser<ElemType>& second)
{
	return first.pElem[0] > second.pElem[0];
}
template <class ElemType>
//�����������·�鲢
//���������pElemSeq-��·���ź���Ļ������׵�ַ
//			nSeqSize- ��·���ź�������ݳ���
//			nChannels-�鲢��·��
//���������pOutBuf-������ݻ���
//			nOutBufSize-������ݻ���������
//���أ�	�ɹ��鲢������ݳ���
//˵�������ð������㷨ѡ���·����������С�����
int MultMerge(ElemType* pElemSeq[], int nSeqSize[], int nChannels, ElemType pOutBuf[], int nOutBufSize)
{
	if (!pElemSeq || !nSeqSize || !pOutBuf)	return 0;
	int i;
	//�����ܵ�������ݳ���
	int nTotalSize = 0;
	for (i = 0; i < nChannels; i++)
	{
		if (nSeqSize[i] <= 0)	return 0;				//��i·���ݳ��Ȳ���ȷ������
		nTotalSize += nSeqSize[i];						//�ۼ����ݳ���
	}
	if (nTotalSize > nOutBufSize)	return	0;
	struct NodeTreeLoser<ElemType> *pHeapLoser;
	pHeapLoser = new struct NodeTreeLoser<ElemType>[nChannels];//����������������
	//��ʼ��������������ÿ���ڵ�
	for (i = 0; i < nChannels; i++)
	{
		if (!pElemSeq[i])	return 0;					//��i·���뻺����󣬷���
		pHeapLoser[i].pElem = pElemSeq[i];				//�������ڵ�Ԫ�ش洢��ַ��ʼ��
		pHeapLoser[i].nSqSize = nSeqSize[i];			//�������ڵ��������ݸ�����ʼ��
		pHeapLoser[i].OutCnts = 0;						//�������ڵ�������ݸ�����ʼ��
	}
	// ��ʼ���ѣ������һ����Ҷ��㿪ʼ����
	for (i = nChannels / 2 - 1; i >= 0; i--)
		siftDownMinRoot(pHeapLoser, i, nChannels);
	int n = nChannels;									//��������ʼ�ڵ���Ϊ����·��
	int nOutIdx = 0;									//������ݸ�����ʼ��Ϊ0
	while (n > 0)										//�������ڵ���Ŀ����0ʱѭ��
	{
		pOutBuf[nOutIdx++] = pHeapLoser[0].pElem[0];	//����Ѷ�����
		pHeapLoser[0].pElem++;							//�Ѷ�����ָ���1
		pHeapLoser[0].OutCnts++;						//�Ѷ�������ݸ�����1
		if (pHeapLoser[0].OutCnts == pHeapLoser[0].nSqSize)
		{//����Ѷ��ڵ������Ѿ�ȫ�����
			pHeapLoser[0]=pHeapLoser[n - 1];			//ʤ���߳���
			n--;										//�ڵ�������1
		}
		siftDownMinRoot(pHeapLoser, 0, n);				// ���ھ��нڵ����µ���ΪС����
	}

	delete []pHeapLoser;								//�ͷŰ���������ռ�
	return nOutIdx;										//����������ݸ���
}

#endif
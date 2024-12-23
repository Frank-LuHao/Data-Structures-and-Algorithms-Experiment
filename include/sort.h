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
	int i, j;//临时变量
	for (i = 1; i < nSize; i++)
	{	// 第i趟直接插入排序
		ElemType e = elem[i];	// 暂存elem[i]
		for (j = i - 1; j >= 0 && e < elem[j]; j--)
		{	// 将比e大的元素都后移
			elem[j + 1] = elem[j];	// 后移
		}
		elem[j + 1] = e;		// j+1为插入位置
	}
	return true;
}
template <class ElemType>
bool ShellSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)
		return false;
	int gap, pos, j;		// gap为希尔增量，pos为待插入记录位置
	ElemType e;
	for (gap = (nSize >>1); gap > 0; gap >>= 1) 
	{//gap从nSize/2开始，每次gap缩小一半
		for (pos = gap; pos < nSize; pos++) 
		{//对gap分组内数据，采用直接排序
			e = elem[pos];
			for (j = pos - gap; j >= 0 && elem[j] > e; j -= gap)
				elem[j + gap] = elem[j];	// 记录后移 
			elem[j + gap] = e;		// 将待插入记录放到合适位置
		}
	}
	return true;
}
template <class ElemType>
bool BubbleSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)	return false;	//安全性检查
	int i, j;
	bool flag = true;				// 记录一趟排序后是否发生过交换
	ElemType tmp;				// 定义交换临时变量
	for (i = 1; i < nSize && flag; ++i)
	{//元素全部排序完成或没有交换时结束循环
		flag = false;				// 假定本趟排序没有交换
		for (j = 0; j < nSize - i; ++j)
			if (elem[j + 1] < elem[j])
			{// 逆序		
				SWAP(elem[j], elem[j + 1], tmp);	// 用宏定义SWAP进行交换,不调用函数，效率更高
				flag = true;			// 交换了数据
			}
	}
	return true;
}

template <class ElemType>
//操作结果，对于low、high区间的序列以轴值分治排列
//返回值：轴值的位置
int QSortPartition(ElemType elem[], int low, int high)
{
	ElemType  pivotValue = elem[low];		// 默认情况下轴值为第一个元素
	if (high - low > 1)
	{//区间内有超过两个个元素，采样得到中值
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
	{		// 开始进行分割
		while (low < high && elem[high] >= pivotValue)high--;	// 找<轴值的记录
		if (low < high)
		{
			elem[low] = elem[high]; low++; // 该记录移动到小下标端
		}	
		while (low < high && elem[low] <= pivotValue) low++;// 找>轴值的记录
		if (low < high)
		{
			elem[high] = elem[low]; high--;// 该记录移动到大下标端
		}	
	}
	elem[low] = pivotValue;			// 把轴值回填到分界位置上
	return low;			// 返回枢轴位置
}

typedef struct tagPARTITION
{
	int low;
	int high;
}PARTITION;
template <class ElemType>
//快速排序算法，非递归
bool QuickSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)	return false;	//安全性检查
	SqStack<PARTITION> stack;//定义区间栈
	PARTITION partition,tmpParti;
	partition.low = 0;
	partition.high = nSize-1;//初始时区间设置为整个序列区间
	stack.Push(partition);
	while (!stack.IsEmpty())
	{
		stack.Pop(partition);//出栈
		int pivotPos = QSortPartition(elem, partition.low, partition.high);//对区间以轴值进行排列
		if (pivotPos > partition.low)
		{//对枢轴左边一半快速排序
			tmpParti.low = partition.low;
			tmpParti.high = pivotPos - 1;
			stack.Push(tmpParti);//入栈
		}
		if (pivotPos + 1 < partition.high)
		{//对枢轴右边一半快速排序
			tmpParti.low = pivotPos + 1;
			tmpParti.high = partition.high;
			stack.Push(tmpParti);//入栈
		}
	}
	return true;
}
template <class ElemType>
bool SelectSort(ElemType elem[], int nSize)
{
	if (!elem || nSize < 1)	return false;	//安全性检查
	int  pos, min, j;	// min为一趟排序中最小记录下标
	// pos为待存放当前最小记录的位置
	ElemType tmp;		//tmp是SWAP的临时变量
	for (pos = 0; pos < nSize - 1; pos++)
	{
		min = pos;		//最小位置初始时设置为pos
		for (j = pos + 1; j < nSize; ++j)
			if (elem[j] < elem[min]) min = j;// 查找最小记录 
		if (pos != min) SWAP(elem[pos], elem[min], tmp);
	}
}

template <class ElemType>
//操作结果：堆排序向下筛选(大根堆)
void siftDownMaxRoot(ElemType elem[], int pos, int nSize)
{
	int child;
	ElemType tmp = elem[pos]; 		// 暂存“根”记录elem[pos]
	for (; (child = pos * 2 + 1) < nSize; pos = child)
	{
		if (child != nSize - 1 && elem[child + 1] > elem[child])
			child++;				// 选取两个孩子的大者		
		if (elem[child] > tmp) 		// 较大的孩子比双亲大
			elem[pos] = elem[child];	// 较大的孩子上移至双亲
		else  break;
	}
	elem[pos] = tmp; 			// 被调整结点放入正确位置
}
template <class ElemType>
//操作结果：堆排序向下筛选(小根堆)
void siftDownMinRoot(ElemType elem[], int pos, int nSize)
{
	int child;
	ElemType tmp = elem[pos]; 		// 暂存“根”记录elem[pos]
	for (; (child = pos * 2 + 1) < nSize; pos = child)
	{
		if (child != nSize - 1 && elem[child + 1] < elem[child])
			child++;				// 选取两个孩子的小者		
		if (elem[child] < tmp) 		// 较小的孩子比双亲小
			elem[pos] = elem[child];	// 较小的孩子上移至双亲
		else  break;
	}
	elem[pos] = tmp; 			// 被调整结点放入正确位置
}
template <class ElemType>
//操作结果：堆排序
bool heapSort(ElemType elem[], int nSize,bool bIncSeq)
{
	if (!elem || nSize < 1)	return false;	//安全性检查
	int  i;
	ElemType tmp;//交换临时变量
	// 初始建堆，从最后一个非叶结点开始调堆
	if (bIncSeq)
	{//增序
		for (i = nSize / 2 - 1; i >= 0; i--)
			siftDownMaxRoot(elem, i, nSize);
		for (i = nSize - 1; i > 0; i--)
		{// 共n-1趟排序（删除堆顶元素后反复调整堆）			
			SWAP(elem[0], elem[i], tmp);	// 交换堆顶元素与子序列中最后一个元素
			siftDownMaxRoot(elem, 0, i);	// 将elem[0..i]重新调整为大顶堆
		}
	}
	else
	{
		for (i = nSize / 2 - 1; i >= 0; i--)
			siftDownMinRoot(elem, i, nSize);
		for (i = nSize - 1; i > 0; i--)
		{// 共n-1趟排序（删除堆顶元素后反复调整堆）			
			SWAP(elem[0], elem[i], tmp);	// 交换堆顶元素与子序列中最后一个元素
			siftDownMinRoot(elem, 0, i);	// 将elem[0..i]重新调整为小顶堆
		}
	}

	return true;
}
template <class ElemType>
//操作结果：归并排序合并两个子序列
void  merge(ElemType elem[], ElemType tmpElem[], int low, int mid, int high)
{
	int i = low, j = mid, k = 0;
	while (i < mid && j <= high)		// elem中记录由小到大地并入tmpElem
		if (elem[i] < elem[j])
			tmpElem[k++] = elem[i++]; 		// 将elem[i]和elem[j]的小者拷贝到tmpElem[k]
		else tmpElem[k++] = elem[j++];
	while (i < mid) 			// 前端剩余elem[i.. mid-1]复制到tmpElem
		tmpElem[k++] = elem[i++];
	while (j <= high) 			// 后端剩余elem[j..high]复制到tmpElem
		tmpElem[k++] = elem[j++];
	for (i = 0, k = low; k <= high; )
		elem[k++] = tmpElem[i++];		// 排好序的记录由tmp拷回elem
}
template <class ElemType>
//操作结果：归并排序辅助函数,递归调用实现归并排序
void  mergeSortAux(ElemType elem[], ElemType tmpElem[], int low, int high)
{
	if (low == high) return;
	int mid = (low + high) / 2; 		// 从中间划分为两个子序列
	mergeSortAux(elem, tmpElem, low, mid);	// 递归归并子序列elem[low.. mid] 
	mergeSortAux(elem, tmpElem, mid + 1, high);// 递归归并子序列elem[mid+1.. high]  
	merge(elem, tmpElem, low, mid + 1, high);	// 归并两个子序列
}
template <class ElemType>
//操作结果：归并排序
bool MergeSort(ElemType elem[], int nSize) 
{
	if (!elem || nSize < 1)	return false;	//安全性检查
	ElemType* tmpElem = new ElemType[nSize];	// 辅助数组
	mergeSortAux(elem, tmpElem, 0, nSize - 1);			//调用归并排序辅助函数完成排序
	delete[] tmpElem;							//删除辅助数组
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
//操作结果：多路归并
//输入参数：pElemSeq-多路已排好序的缓冲区首地址
//			nSeqSize- 多路已排好序的数据长度
//			nChannels-归并的路数
//输出参数：pOutBuf-输出数据缓冲
//			nOutBufSize-输出数据缓冲区长度
//返回：	成功归并后的数据长度
//说明：采用败者树算法选择多路有序数据最小者输出
int MultMerge(ElemType* pElemSeq[], int nSeqSize[], int nChannels, ElemType pOutBuf[], int nOutBufSize)
{
	if (!pElemSeq || !nSeqSize || !pOutBuf)	return 0;
	int i;
	//计算总的输出数据长度
	int nTotalSize = 0;
	for (i = 0; i < nChannels; i++)
	{
		if (nSeqSize[i] <= 0)	return 0;				//第i路数据长度不正确，返回
		nTotalSize += nSeqSize[i];						//累加数据长度
	}
	if (nTotalSize > nOutBufSize)	return	0;
	struct NodeTreeLoser<ElemType> *pHeapLoser;
	pHeapLoser = new struct NodeTreeLoser<ElemType>[nChannels];//创建败者树堆数组
	//初始化败者树堆数组每个节点
	for (i = 0; i < nChannels; i++)
	{
		if (!pElemSeq[i])	return 0;					//第i路输入缓冲错误，返回
		pHeapLoser[i].pElem = pElemSeq[i];				//败者树节点元素存储地址初始化
		pHeapLoser[i].nSqSize = nSeqSize[i];			//败者树节点序列数据个数初始化
		pHeapLoser[i].OutCnts = 0;						//败者树节点输出数据个数初始化
	}
	// 初始建堆，从最后一个非叶结点开始调堆
	for (i = nChannels / 2 - 1; i >= 0; i--)
		siftDownMinRoot(pHeapLoser, i, nChannels);
	int n = nChannels;									//败者树初始节点数为输入路数
	int nOutIdx = 0;									//输出数据个数初始化为0
	while (n > 0)										//败者树节点数目大于0时循环
	{
		pOutBuf[nOutIdx++] = pHeapLoser[0].pElem[0];	//输出堆顶数据
		pHeapLoser[0].pElem++;							//堆顶数据指针加1
		pHeapLoser[0].OutCnts++;						//堆顶输出数据个数加1
		if (pHeapLoser[0].OutCnts == pHeapLoser[0].nSqSize)
		{//如果堆顶节点数据已经全部输出
			pHeapLoser[0]=pHeapLoser[n - 1];			//胜利者出局
			n--;										//节点总数减1
		}
		siftDownMinRoot(pHeapLoser, 0, n);				// 尚在局中节点重新调整为小顶堆
	}

	delete []pHeapLoser;								//释放败者树数组空间
	return nOutIdx;										//返回输出数据个数
}

#endif
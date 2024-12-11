#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
#include "../../include/graph_base.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

// 课程结构
struct CourseType
{
	string id; // 课程编号
	string name; // 课程名称
	int credit; // 学时
	int term; // 开课学期(0~8, 0代表没有指定开课学期)
};

// 课程表类
template <int TermsNum>
class SimpleSchedule
{
public:
	SimpleSchedule();//构造函数
	~SimpleSchedule();//析构函数
	void Run();//运行

private:
	// 私有函数
	void ReadCourseInfo();//读取课程信息
	void TopSort();//拓扑排序
	void GenerateSchedule();//存储课程信息

	// 私有数据
	ifstream m_inFile; //输入文件
	ofstream m_outFile; //输出文件

	AdjListGraph<CourseType, int> *m_pGraph; //课程表
	
	CourseType *m_pCourseInfo; //所有课程信息,作为结点
	int m_nCourseNum; //课程数量
	GraphEdge<int>* m_pEdge; //存储边,用于初始化图
	int m_nEdgeNum; //边数量

	SqList<int> m_qTopSortResult; //拓扑排序结果
	int m_nCourseNumPerTerm[TermsNum + 1]; //每学期课程数量
	SqList<int>* m_scheduleResult[TermsNum + 1]; //各学期排课结果,总共8个学期
};

template <int TermsNum>
SimpleSchedule<TermsNum>::SimpleSchedule()
{
	// 指针
	m_pCourseInfo = NULL;
	m_pGraph = NULL;
	for (int i = 0; i < TermsNum + 1; i++)
		m_scheduleResult[i] = NULL;

	// 文件
	m_inFile.open("course_inf.csv");
	if (!m_inFile.is_open()) {
		cout << "课程信息文件打开失败" << endl;
		exit(1);
	}
	m_outFile.open("curriculum_schedule.txt");
	if (!m_outFile.is_open()) {
		cout << "输出课程表文件打开失败" << endl;
		exit(2);
	}
};

template <int TermsNum>
SimpleSchedule<TermsNum>::~SimpleSchedule()
{
	// 指针
	if (m_pCourseInfo != NULL)
		delete[] m_pCourseInfo;
	if (m_pGraph != NULL)
		delete m_pGraph;
	for (int i = 0; i < TermsNum + 1; i++)
		if (m_scheduleResult[i] != NULL)
			delete m_scheduleResult[i];
	
	// 文件
	if (m_inFile.is_open())
		m_inFile.close();
	if (m_outFile.is_open())
		m_outFile.close();
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::ReadCourseInfo()
{ 
	//读取每学期开课数
	m_nCourseNum = 0;
	for (int i = 0; i < 8; i++) {
		m_inFile >> m_nCourseNumPerTerm[i];
		m_nCourseNum += m_nCourseNumPerTerm[i];
	}
	
	// 初始化
	m_pCourseInfo = new CourseType[m_nCourseNum];
	SqList<GraphEdge<int>> qEdge; // 存储边
	GraphEdge<int> tmp_edge;
	tmp_edge.weight = 1; // 初始化权重为1
	// 读取课程信息
	// 读入格式：课程编号 课程名称 学时 开课学期 先修课程
	for (int i = 0; i < m_nCourseNum; i++)
	{
		string preCourseInf;
		m_inFile >> m_pCourseInfo[i].id >> m_pCourseInfo[i].name >> m_pCourseInfo[i].credit >> m_pCourseInfo[i].term >> preCourseInf;
		// 统计先修课程
		int preCourseNum = 0;
		for (int j = 0; j < preCourseInf.size() && preCourseInf != "0"; j++) {
			if (preCourseInf[j] == ',') {
				// 添加边
				tmp_edge.vex1 = preCourseNum - 1;
				tmp_edge.vex2 = i;
				qEdge.AddTail(tmp_edge);
				// 重置
				preCourseNum = 0;
			}
			else if (j == preCourseInf.size() - 1) {
				preCourseNum = preCourseNum * 10 + preCourseInf[j] - '0';
				// 添加边
				tmp_edge.vex1 = preCourseNum - 1;
				tmp_edge.vex2 = i;
				qEdge.AddTail(tmp_edge);
			}
			else {
				preCourseNum = preCourseNum * 10 + preCourseInf[j] - '0';
			}
		}
	}
	// 将边转化为数组用于初始化
	m_nEdgeNum = qEdge.Length();
	m_pEdge = new GraphEdge<int>[m_nEdgeNum];
	for (int i = 0; i < m_nEdgeNum; i++) {
		qEdge.GetElem(i, tmp_edge);
		m_pEdge[i] = tmp_edge;
	}
	// 构建图
	m_pGraph = new AdjListGraph<CourseType, int>(m_pCourseInfo, m_nCourseNum, DIR_GRAPH);  // 类型为有向图
	// 添加边
	m_pGraph->InsertEdges(m_pEdge, m_nEdgeNum);
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::TopSort()
{
	m_pGraph->TopSort(m_qTopSortResult);
	// 显示结果
	cout << "拓扑排序结果: ";
	for (int i = 0; i < m_qTopSortResult.Length(); i++) {
		int tmp;
		m_qTopSortResult.GetElem(i, tmp);
		cout << tmp+1 << " ";
	}
	cout << endl;
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::GenerateSchedule()
{
	// 将课程划分到各学期之中
	// 策略：先保存固定学期的课程，再将剩余课程按每学期修读指标分配到各学期
	
	// 初始化
	for (int i = 0; i < TermsNum; i++) {
		m_scheduleResult[i] = new SqList<int>;
	}
	for (int j = 0; j < TermsNum; j++) {
		int CurNum = 0;
		// 先保存固定学期的课程
		for (int i = 0; i < m_qTopSortResult.Length(); i++) {
			int tmp;
			m_qTopSortResult.GetElem(i, tmp);
			if (m_pCourseInfo[tmp].term == j+1) {
				m_scheduleResult[j]->AddTail(tmp);
				CurNum++;
				m_qTopSortResult.Delete(i, tmp);
			}
		}
		// 将剩余课程按每学期修读指标分配到各学期
		for (int i = 0; i < m_qTopSortResult.Length() && CurNum < m_nCourseNumPerTerm[j]; i++) {
			int tmp;
			m_qTopSortResult.GetElem(i, tmp);
			if (m_pCourseInfo[tmp].term == 0) {
				m_scheduleResult[j]->AddTail(tmp);
				CurNum++;
				m_qTopSortResult.Delete(i, tmp);
				i--;
			}
		}
	}
	
	for (int i = 0; i < TermsNum; i++) {
		cout << "第" << i+1 << "学期课程：" << endl;
		for (int j = 0; j < m_scheduleResult[i]->Length(); j++) {
			int tmp;
			m_scheduleResult[i]->GetElem(j, tmp);
			cout << m_pCourseInfo[tmp].id << " " << m_pCourseInfo[tmp].name << " " << m_pCourseInfo[tmp].credit << " " << m_pCourseInfo[tmp].term << endl;
		}
	}
	// 分别对个学期进行排课
	// 策略：

	// 输出课程表
	m_outFile << "节次\t\t\t星期一\t\t\t星期二\t\t\t星期三\t\t\t星期四\t\t\t星期五\n";
	m_outFile << "第一节\n";
	m_outFile << "第二节\n";
	m_outFile << "\n课间休息\n\n";
	m_outFile << "第三节\n";
	m_outFile << "第四节\n";
	m_outFile << "第五节\n";
	m_outFile << "\n午间休息\n\n";
	m_outFile << "第六节\n";
	m_outFile << "第七节\n";
	m_outFile << "\n课间休息\n\n";
	m_outFile << "第八节\n";
	m_outFile << "第九节\n";
	m_outFile << "第十节\n";
	m_outFile << "\n晚自习\n";
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::Run()
{
	ReadCourseInfo(); // 读取课程信息
	TopSort(); // 拓扑排序
	GenerateSchedule();  // 生成课程表
};

#endif
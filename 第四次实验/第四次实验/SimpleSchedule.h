#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
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
	void ReadCourseInfo();//读取课程信息
	void TopSort();//拓扑排序
	void StoreCourseInfo();//存储课程信息

private:
	AdjListGraph<CourseType, int> *m_pGraph; //课程表
	CourseType *m_pCourseInfo; //所有课程信息
	int* m_pInDegree; //各节点入度数组
	ifstream m_inFile; //输入文件
	ofstream m_outFile; //输出文件
	SqList<int> q[TermsNum + 1]; //存储入度为0的课程
	SqList<int> *m_scheduleResult[TermsNum + 1]; //各学期排课结果,总共8个学期
	int m_nCourseNumPerTerm[TermsNum + 1]; //每学期课程数量
};

template <int TermsNum>
SimpleSchedule<TermsNum>::SimpleSchedule()
{
	// 指针
	m_pCourseInfo = NULL;
	m_pGraph = NULL;
	m_pInDegree = NULL;
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
	if (m_pInDegree != NULL)
		delete[] m_pInDegree;
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
	int CourseNum = 0;
	for (int i = 0; i < 8; i++) {
		m_inFile >> m_nCourseNumPerTerm[i];
		CourseNum += m_nCourseNumPerTerm[i];
	}
	
	// 初始化指针
	m_pCourseInfo = new CourseType[CourseNum];
	m_pInDegree = new int[CourseNum + 1];
	// 初始化入度数组
	for (int i = 0; i < CourseNum; i++)
		m_pInDegree[i] = 0;
	// 读取课程信息
	// 读入格式：课程编号 课程名称 学时 开课学期 先修课程
	for (int i = 0; i < CourseNum; i++)
	{
		string preCourseNum;
		m_inFile >> m_pCourseInfo[i].id >> m_pCourseInfo[i].name >> m_pCourseInfo[i].credit >> m_pCourseInfo[i].term >> preCourseNum;
		if (m_pCourseInfo[i].term == 0)
		{ // 未指定开课学期
			cout << preCourseNum << endl;
			for (int j = 0; j < preCourseNum.size() && preCourseNum[j] != ','; j++) {
				m_pInDegree[preCourseNum[j] - '0']++;
			}
		}
	}
	for (int i = 1; i <= CourseNum; i++)
		cout << m_pInDegree[i] << " ";
	// 构建图
	/*
	m_pGraph = new AdjListGraph<CourseType, int>(m_pCourseInfo, m_nCourseNum, DIR_GRAPH);  // 有向图即可
	int v1, v2;
	for (int i = 0; i < m_nCourseNum; i++)
	{
		for (int j = 0; j < m_nCourseNum; j++)
		{
			m_inFile >> v1 >> v2;
			if (v1 == 0 || v2 == 0)
				continue;
			m_pGraph->InsertEdge(v1 - 1, v2 - 1, 1);
			m_pInDegree[v2 - 1]++;
		}
	}
	*/
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::TopSort()
{
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::StoreCourseInfo()
{
};

#endif
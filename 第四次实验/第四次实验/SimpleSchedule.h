#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
#include<iostream>
#include<fstream>
#include <string>
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
class SimpleSchedule
{
public:
	SimpleSchedule(string in_filename, string out_filename);//构造函数
	~SimpleSchedule();//析构函数
	void ReadCourseInfo();//读取课程信息
	void TopSort();//拓扑排序
	void StoreCourseInfo();//存储课程信息

private:
	AdjListGraph<CourseType, int>* m_graph;//课程表
	CourseType* m_courseInfo;//当前课程信息
	int m_nCourseNum;//课程数量
	int* m_inDegree;//个节点入度数组
	ifstream m_inFile;//输入文件
	ofstream m_outFile;//输出文件
	SqList<int> **m_scheduleResult;//个学期排课结果
};

SimpleSchedule::SimpleSchedule(string in_filename, string out_filename)
{
	m_courseInfo = NULL;
	m_nCourseNum = 0;
	m_inDegree = NULL;
	m_graph = NULL;
	m_scheduleResult = NULL;
	m_inFile.open(in_filename.c_str());
	if (!m_inFile)
	{
		cerr << "打开文件 " << in_filename << " 失败!" << endl;
		exit(1);
	}
	m_outFile.open(out_filename.c_str());
	if (!m_outFile)
	{
		cerr << "打开文件 " << out_filename << " 失败!" << endl;
		exit(1);
	}
};

SimpleSchedule::~SimpleSchedule()
{
	if (m_courseInfo != NULL)
		delete[] m_courseInfo;
	if (m_inDegree != NULL)
		delete[] m_inDegree;
	if (m_graph != NULL)
		delete m_graph;
	if (m_scheduleResult != NULL)
	{
		for (int i = 0; i < 8; i++)
		{
			if (m_scheduleResult[i] != NULL)
				delete m_scheduleResult[i];
		}
		delete[] m_scheduleResult;
	}
	m_inFile.close();
	m_outFile.close();
};

void SimpleSchedule::ReadCourseInfo()
{ //读入格式：课程编号 课程名称 学时 开课学期 先修课程
	m_courseInfo = new CourseType[m_nCourseNum];
	m_inDegree = new int[m_nCourseNum];
	for (int i = 0; i < m_nCourseNum; i++)
	{
		m_inFile >> m_courseInfo[i].id >> m_courseInfo[i].name >> m_courseInfo[i].credit >> m_courseInfo[i].term;
		m_inDegree[i] = 0;
	}
	// 构建图
	m_graph = new AdjListGraph<CourseType, int>(m_courseInfo, m_nCourseNum, DIR_GRAPH);
	int v1, v2;
	for (int i = 0; i < m_nCourseNum; i++)
	{
		for (int j = 0; j < m_nCourseNum; j++)
		{
			m_inFile >> v1 >> v2;
			if (v1 == 0 || v2 == 0)
				continue;
			m_graph->InsertEdge(v1 - 1, v2 - 1, 1);
			m_inDegree[v2 - 1]++;
		}
	}
};

void SimpleSchedule::TopSort()
{
	int* topOrder = new int[m_nCourseNum];
	int top = -1;
	for (int i = 0; i < m_nCourseNum; i++)
	{
		if (m_inDegree[i] == 0)
			topOrder[++top] = i;
	}
	while (top != -1)
	{
		int v = topOrder[top--];
		m_outFile << m_courseInfo[v].id << " " << m_courseInfo[v].name << " " << m_courseInfo[v].credit << " " << m_courseInfo[v].term << endl;
		int w;
		for (w = m_graph->FirstAdjVex(v, w); w != -1; w = m_graph->NextAdjVex(v, w))
		{
			if (--m_inDegree[w] == 0)
				topOrder[++top] = w;
		}
	}
	delete[] topOrder;
};

void SimpleSchedule::StoreCourseInfo()
{
	m_outFile << m_nCourseNum << endl;
	for (int i = 0; i < m_nCourseNum; i++)
	{
		m_outFile << m_courseInfo[i].id << " " << m_courseInfo[i].name << " " << m_courseInfo[i].credit << " " << m_courseInfo[i].term << endl;
	}
	int v1, v2;
	for (int i = 0; i < m_nCourseNum; i++)
	{
		for (int j = 0; j < m_nCourseNum; j++)
		{
			m_graph->FirstAdjVex(i, v1);
			m_graph->NextAdjVex(i, v1, v2);
			m_outFile << v1 << " " << v2 << endl;
		}
	}
};

#endif
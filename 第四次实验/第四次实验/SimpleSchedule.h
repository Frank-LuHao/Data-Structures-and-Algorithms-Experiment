#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
#include<iostream>
#include<fstream>
#include <string>
using namespace std;

// �γ̽ṹ
struct CourseType
{
	string id; // �γ̱��
	string name; // �γ�����
	int credit; // ѧʱ
	int term; // ����ѧ��(0~8, 0����û��ָ������ѧ��)
};

// �γ̱���
class SimpleSchedule
{
public:
	SimpleSchedule(string in_filename, string out_filename);//���캯��
	~SimpleSchedule();//��������
	void ReadCourseInfo();//��ȡ�γ���Ϣ
	void TopSort();//��������
	void StoreCourseInfo();//�洢�γ���Ϣ

private:
	AdjListGraph<CourseType, int>* m_graph;//�γ̱�
	CourseType* m_courseInfo;//��ǰ�γ���Ϣ
	int m_nCourseNum;//�γ�����
	int* m_inDegree;//���ڵ��������
	ifstream m_inFile;//�����ļ�
	ofstream m_outFile;//����ļ�
	SqList<int> **m_scheduleResult;//��ѧ���ſν��
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
		cerr << "���ļ� " << in_filename << " ʧ��!" << endl;
		exit(1);
	}
	m_outFile.open(out_filename.c_str());
	if (!m_outFile)
	{
		cerr << "���ļ� " << out_filename << " ʧ��!" << endl;
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
{ //�����ʽ���γ̱�� �γ����� ѧʱ ����ѧ�� ���޿γ�
	m_courseInfo = new CourseType[m_nCourseNum];
	m_inDegree = new int[m_nCourseNum];
	for (int i = 0; i < m_nCourseNum; i++)
	{
		m_inFile >> m_courseInfo[i].id >> m_courseInfo[i].name >> m_courseInfo[i].credit >> m_courseInfo[i].term;
		m_inDegree[i] = 0;
	}
	// ����ͼ
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
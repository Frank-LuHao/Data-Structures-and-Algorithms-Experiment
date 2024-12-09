#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
#include<iostream>
#include<fstream>
#include<string>
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
template <int TermsNum>
class SimpleSchedule
{
public:
	SimpleSchedule();//���캯��
	~SimpleSchedule();//��������
	void ReadCourseInfo();//��ȡ�γ���Ϣ
	void TopSort();//��������
	void StoreCourseInfo();//�洢�γ���Ϣ

private:
	AdjListGraph<CourseType, int> *m_pGraph; //�γ̱�
	CourseType *m_pCourseInfo; //���пγ���Ϣ
	int* m_pInDegree; //���ڵ��������
	ifstream m_inFile; //�����ļ�
	ofstream m_outFile; //����ļ�
	SqList<int> q[TermsNum + 1]; //�洢���Ϊ0�Ŀγ�
	SqList<int> *m_scheduleResult[TermsNum + 1]; //��ѧ���ſν��,�ܹ�8��ѧ��
	int m_nCourseNumPerTerm[TermsNum + 1]; //ÿѧ�ڿγ�����
};

template <int TermsNum>
SimpleSchedule<TermsNum>::SimpleSchedule()
{
	// ָ��
	m_pCourseInfo = NULL;
	m_pGraph = NULL;
	m_pInDegree = NULL;
	for (int i = 0; i < TermsNum + 1; i++)
		m_scheduleResult[i] = NULL;

	// �ļ�
	m_inFile.open("course_inf.csv");
	if (!m_inFile.is_open()) {
		cout << "�γ���Ϣ�ļ���ʧ��" << endl;
		exit(1);
	}
	m_outFile.open("curriculum_schedule.txt");
	if (!m_outFile.is_open()) {
		cout << "����γ̱��ļ���ʧ��" << endl;
		exit(2);
	}
};

template <int TermsNum>
SimpleSchedule<TermsNum>::~SimpleSchedule()
{
	// ָ��
	if (m_pCourseInfo != NULL)
		delete[] m_pCourseInfo;
	if (m_pGraph != NULL)
		delete m_pGraph;
	if (m_pInDegree != NULL)
		delete[] m_pInDegree;
	for (int i = 0; i < TermsNum + 1; i++)
		if (m_scheduleResult[i] != NULL)
			delete m_scheduleResult[i];
	
	// �ļ�
	if (m_inFile.is_open())
		m_inFile.close();
	if (m_outFile.is_open())
		m_outFile.close();
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::ReadCourseInfo()
{ 
	//��ȡÿѧ�ڿ�����
	int CourseNum = 0;
	for (int i = 0; i < 8; i++) {
		m_inFile >> m_nCourseNumPerTerm[i];
		CourseNum += m_nCourseNumPerTerm[i];
	}
	
	// ��ʼ��ָ��
	m_pCourseInfo = new CourseType[CourseNum];
	m_pInDegree = new int[CourseNum + 1];
	// ��ʼ���������
	for (int i = 0; i < CourseNum; i++)
		m_pInDegree[i] = 0;
	// ��ȡ�γ���Ϣ
	// �����ʽ���γ̱�� �γ����� ѧʱ ����ѧ�� ���޿γ�
	for (int i = 0; i < CourseNum; i++)
	{
		string preCourseNum;
		m_inFile >> m_pCourseInfo[i].id >> m_pCourseInfo[i].name >> m_pCourseInfo[i].credit >> m_pCourseInfo[i].term >> preCourseNum;
		if (m_pCourseInfo[i].term == 0)
		{ // δָ������ѧ��
			cout << preCourseNum << endl;
			for (int j = 0; j < preCourseNum.size() && preCourseNum[j] != ','; j++) {
				m_pInDegree[preCourseNum[j] - '0']++;
			}
		}
	}
	for (int i = 1; i <= CourseNum; i++)
		cout << m_pInDegree[i] << " ";
	// ����ͼ
	/*
	m_pGraph = new AdjListGraph<CourseType, int>(m_pCourseInfo, m_nCourseNum, DIR_GRAPH);  // ����ͼ����
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
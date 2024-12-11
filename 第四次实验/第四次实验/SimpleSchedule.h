#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
#include "../../include/graph_base.h"
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
	void Run();//����

private:
	// ˽�к���
	void ReadCourseInfo();//��ȡ�γ���Ϣ
	void TopSort();//��������
	void GenerateSchedule();//�洢�γ���Ϣ

	// ˽������
	ifstream m_inFile; //�����ļ�
	ofstream m_outFile; //����ļ�

	AdjListGraph<CourseType, int> *m_pGraph; //�γ̱�
	
	CourseType *m_pCourseInfo; //���пγ���Ϣ,��Ϊ���
	int m_nCourseNum; //�γ�����
	GraphEdge<int>* m_pEdge; //�洢��,���ڳ�ʼ��ͼ
	int m_nEdgeNum; //������

	SqList<int> m_qTopSortResult; //����������
	int m_nCourseNumPerTerm[TermsNum + 1]; //ÿѧ�ڿγ�����
	SqList<int>* m_scheduleResult[TermsNum + 1]; //��ѧ���ſν��,�ܹ�8��ѧ��
};

template <int TermsNum>
SimpleSchedule<TermsNum>::SimpleSchedule()
{
	// ָ��
	m_pCourseInfo = NULL;
	m_pGraph = NULL;
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
	m_nCourseNum = 0;
	for (int i = 0; i < 8; i++) {
		m_inFile >> m_nCourseNumPerTerm[i];
		m_nCourseNum += m_nCourseNumPerTerm[i];
	}
	
	// ��ʼ��
	m_pCourseInfo = new CourseType[m_nCourseNum];
	SqList<GraphEdge<int>> qEdge; // �洢��
	GraphEdge<int> tmp_edge;
	tmp_edge.weight = 1; // ��ʼ��Ȩ��Ϊ1
	// ��ȡ�γ���Ϣ
	// �����ʽ���γ̱�� �γ����� ѧʱ ����ѧ�� ���޿γ�
	for (int i = 0; i < m_nCourseNum; i++)
	{
		string preCourseInf;
		m_inFile >> m_pCourseInfo[i].id >> m_pCourseInfo[i].name >> m_pCourseInfo[i].credit >> m_pCourseInfo[i].term >> preCourseInf;
		// ͳ�����޿γ�
		int preCourseNum = 0;
		for (int j = 0; j < preCourseInf.size() && preCourseInf != "0"; j++) {
			if (preCourseInf[j] == ',') {
				// ��ӱ�
				tmp_edge.vex1 = preCourseNum - 1;
				tmp_edge.vex2 = i;
				qEdge.AddTail(tmp_edge);
				// ����
				preCourseNum = 0;
			}
			else if (j == preCourseInf.size() - 1) {
				preCourseNum = preCourseNum * 10 + preCourseInf[j] - '0';
				// ��ӱ�
				tmp_edge.vex1 = preCourseNum - 1;
				tmp_edge.vex2 = i;
				qEdge.AddTail(tmp_edge);
			}
			else {
				preCourseNum = preCourseNum * 10 + preCourseInf[j] - '0';
			}
		}
	}
	// ����ת��Ϊ�������ڳ�ʼ��
	m_nEdgeNum = qEdge.Length();
	m_pEdge = new GraphEdge<int>[m_nEdgeNum];
	for (int i = 0; i < m_nEdgeNum; i++) {
		qEdge.GetElem(i, tmp_edge);
		m_pEdge[i] = tmp_edge;
	}
	// ����ͼ
	m_pGraph = new AdjListGraph<CourseType, int>(m_pCourseInfo, m_nCourseNum, DIR_GRAPH);  // ����Ϊ����ͼ
	// ��ӱ�
	m_pGraph->InsertEdges(m_pEdge, m_nEdgeNum);
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::TopSort()
{
	m_pGraph->TopSort(m_qTopSortResult);
	// ��ʾ���
	cout << "����������: ";
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
	// ���γ̻��ֵ���ѧ��֮��
	// ���ԣ��ȱ���̶�ѧ�ڵĿγ̣��ٽ�ʣ��γ̰�ÿѧ���޶�ָ����䵽��ѧ��
	
	// ��ʼ��
	for (int i = 0; i < TermsNum; i++) {
		m_scheduleResult[i] = new SqList<int>;
	}
	for (int j = 0; j < TermsNum; j++) {
		int CurNum = 0;
		// �ȱ���̶�ѧ�ڵĿγ�
		for (int i = 0; i < m_qTopSortResult.Length(); i++) {
			int tmp;
			m_qTopSortResult.GetElem(i, tmp);
			if (m_pCourseInfo[tmp].term == j+1) {
				m_scheduleResult[j]->AddTail(tmp);
				CurNum++;
				m_qTopSortResult.Delete(i, tmp);
			}
		}
		// ��ʣ��γ̰�ÿѧ���޶�ָ����䵽��ѧ��
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
		cout << "��" << i+1 << "ѧ�ڿγ̣�" << endl;
		for (int j = 0; j < m_scheduleResult[i]->Length(); j++) {
			int tmp;
			m_scheduleResult[i]->GetElem(j, tmp);
			cout << m_pCourseInfo[tmp].id << " " << m_pCourseInfo[tmp].name << " " << m_pCourseInfo[tmp].credit << " " << m_pCourseInfo[tmp].term << endl;
		}
	}
	// �ֱ�Ը�ѧ�ڽ����ſ�
	// ���ԣ�

	// ����γ̱�
	m_outFile << "�ڴ�\t\t\t����һ\t\t\t���ڶ�\t\t\t������\t\t\t������\t\t\t������\n";
	m_outFile << "��һ��\n";
	m_outFile << "�ڶ���\n";
	m_outFile << "\n�μ���Ϣ\n\n";
	m_outFile << "������\n";
	m_outFile << "���Ľ�\n";
	m_outFile << "�����\n";
	m_outFile << "\n�����Ϣ\n\n";
	m_outFile << "������\n";
	m_outFile << "���߽�\n";
	m_outFile << "\n�μ���Ϣ\n\n";
	m_outFile << "�ڰ˽�\n";
	m_outFile << "�ھŽ�\n";
	m_outFile << "��ʮ��\n";
	m_outFile << "\n����ϰ\n";
};

template <int TermsNum>
void SimpleSchedule<TermsNum>::Run()
{
	ReadCourseInfo(); // ��ȡ�γ���Ϣ
	TopSort(); // ��������
	GenerateSchedule();  // ���ɿγ̱�
};

#endif
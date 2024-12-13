#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
#include "../../include/graph_base.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
// �ſν��
struct Time
{
	int weekday; // ���ڼ� 1~5
	int block; // �ڼ��ڿ� 0~3
	// ���ظ�ֵ�����
	Time& operator= (const Time& t)
	{
		weekday = t.weekday;
		block = t.block;
		return *this;
	}
};
// �γ̽ṹ
struct CourseType
{
	int id; // �γ̱��
	string name; // �γ�����
	int credit; // ѧʱ
	int term; // ����ѧ��(0~8, 0����û��ָ������ѧ��)
	Time time; // �Ͽ�ʱ��
	// ���ظ�ֵ�����
	CourseType& operator= (const CourseType& c)
	{
		id = c.id;
		name =  c.name;
		credit = c.credit;
		term = c.term;
		time = c.time;
		return *this;
	}
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
	int check(int j, int k, SqList<CourseType>& List); // ����Ƿ��п�
	

	// ˽������
	ifstream m_inFile; //�����ļ�
	ofstream m_outFile; //����ļ�

	AdjListGraph<CourseType, int> *m_pGraph; //�γ̱�
	
	CourseType *m_pCourseInfo; //���пγ���Ϣ,��Ϊ���
	int m_nCourseNum; //�γ�����
	GraphEdge<int>* m_pEdge; //�洢��,���ڳ�ʼ��ͼ
	int m_nEdgeNum; //������

	SqList<int> m_qTopSortResult; //����������
	int m_nCourseNumPerTerm[TermsNum]; //ÿѧ�ڿγ�����
	SqList<int>* m_scheduleResult[TermsNum]; //��ѧ���ſν��,�ܹ�8��ѧ��
};

template <int TermsNum>
SimpleSchedule<TermsNum>::SimpleSchedule()
{
	// ָ��
	m_pCourseInfo = NULL;
	m_pGraph = NULL;
	for (int i = 0; i < TermsNum; i++)
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
	for (int i = 0; i < TermsNum; i++)
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
	
	// �ֱ�Ը�ѧ�ڽ����ſ�
	// ���ԣ�ÿ���Ͽ�ѧʱ�������ȣ����ǵ�����һ���϶࣬�������ſΣ��γ̿�ʱ����������к������Ҳ�ֺ�Ŀγ̾�����������������, ���пγ������羡���ֲ�����
	// step1: ��ʱ��� 4 = 2 + 2, 5 = 3 + 2, 6 = 3 + 3
	SqList<CourseType> *ScheduleDetails = new SqList<CourseType>[TermsNum]; // ����ſν����ϸ��Ϣ
	for (int i = 0; i < TermsNum; i++)
	{ //��ÿѧ�ڱ���
		int len = m_scheduleResult[i]->Length();
		int tmp;
		CourseType tmpCourse;
		// ���Ƚ���ʼ�γ̼�����ϸ��Ϣ���Դ�ν��е�һ�β��
		//cout << "��һ�β��" << endl;
		for (int j = 0; j < len; j++)
		{ // ��ÿ�ſα���
			m_scheduleResult[i]->GetElem(j, tmp);
			tmpCourse = m_pCourseInfo[tmp];
			if (m_pCourseInfo[tmp].credit == 4 || m_pCourseInfo[tmp].credit == 5 || m_pCourseInfo[tmp].credit == 6)
			{ // ��ʱ���
				tmpCourse.credit = tmpCourse.credit%2 == 0 ? tmpCourse.credit/2 : tmpCourse.credit/2 + 1;
			}
			ScheduleDetails[i].AddTail(tmpCourse);
		}
		// Ȼ�󽫴�εĵڶ����ּ�����ϸ��Ϣ
		for (int j = 0; j < len; j++)
		{ //��ÿ�ſα���
			m_scheduleResult[i]->GetElem(j, tmp);
			tmpCourse = m_pCourseInfo[tmp];
			if (m_pCourseInfo[tmp].credit == 4 || m_pCourseInfo[tmp].credit == 5 || m_pCourseInfo[tmp].credit == 6)
			{ // ��ʱ���
				tmpCourse.credit = tmpCourse.credit / 2;
				ScheduleDetails[i].AddTail(tmpCourse); //�ڶ���ֻ�Դ�ν������
			}
		}
	}
	

	// step2: �γ��ſ�
	// ���ԣ��γ̾����������ڿ�ǰ��ʱ��Σ��Ҿ�����֤ÿ�춼�п�
	SqList<CourseType>* FinalSchedule = new SqList<CourseType>[TermsNum]; // �洢�����ſν��
	for (int i = 0; i < TermsNum; i++)
	{ //��ÿѧ�ڱ���

		// 2.1: ����ÿ��γ���
		int NumPerDay[5]; // ÿ��γ���
		int len = ScheduleDetails[i].Length(), AverageNum = len / 5, Overleft = len - AverageNum * 5;
		for (int j = 0; j < 5; j++)
		{ // ����ÿ������
			NumPerDay[j] = AverageNum;
			if (Overleft > 0)
			{ // ����Ŀ������ڿ�ǰ�ļ�����ȷ���
				NumPerDay[j]++;
				Overleft--;
			}
		}

		// 2.2: ȷ�������Ͽ�ʱ��
		// ���ԣ�135�����ſ������磬 24�����ſ������磬�Ա�֤������������
		
		int curDay = 0, BlockFlag[5][4]; // curDay��ʾ��ǰ�γ�������BlockFlag��ʾĳʱ����Ƿ��п�
		CourseType tmpCourse;
		// ��ʼ�� BlockFlag
		for (int j = 0; j < 5; j++)
			for (int k = 0; k < 4; k++)
				BlockFlag[j][k] = 0;

		// ��ʼ�ſ�
		for (int j = 0; j < 5; j++)
		{ // ��ÿ���������
			int k = 0;
			while (k < NumPerDay[j])
			{ // ÿ�춼Ҫ��NumPerDay[j]�ڿ�
				int Flag = 1; // ���ڼ�¼�Ƿ��ſγɹ�
				if (j % 2 == 0)
				{ // ��135��������
					ScheduleDetails[i].GetElem(curDay, tmpCourse);
					tmpCourse.time.weekday = j + 1;
					if (tmpCourse.credit == 2)
					{ // ��ѧʱ��0��2ʱ��
						if (BlockFlag[j][0] == 0)
						{ // ��������
							BlockFlag[j][0] = 1;
							tmpCourse.time.block = 0;
						}
						else if (BlockFlag[j][2] == 0)
						{ // �ڿ�������
							BlockFlag[j][2] = 1;
							tmpCourse.time.block = 2;
						}
						else
						{ // ����
							Flag = 0; // �ſβ��ɹ�
						}
					}
					else
					{ // ��ѧʱ��1��3ʱ��
						if (BlockFlag[j][1] == 0)
						{ // ��������
							BlockFlag[j][1] = 1;
							tmpCourse.time.block = 1;
						}
						else if (BlockFlag[j][3] == 0)
						{ // �ٿ�������
							BlockFlag[j][3] = 1;
							tmpCourse.time.block = 3;
						}
						else
						{ // ����
							Flag = 0; // �ſβ��ɹ�
						}
					}
				}
				else
				{ // ��24��������
					ScheduleDetails[i].GetElem(curDay, tmpCourse);
					tmpCourse.time.weekday = j + 1;
					if (tmpCourse.credit == 2)
					{ // ��ѧʱ��0��2ʱ��
						if (BlockFlag[j][2] == 0)
						{ // ��������
							BlockFlag[j][2] = 1;
							tmpCourse.time.block = 2;
						}
						else if (BlockFlag[j][0] == 0)
						{ // �ٿ�������
							BlockFlag[j][0] = 1;
							tmpCourse.time.block = 0;
						}
						else
						{ // ����
							Flag = 0; // �ſβ��ɹ�
						}
					}
					else
					{ // ��ѧʱ��1��3ʱ��
						if (BlockFlag[j][3] == 0)
						{ // ��������
							BlockFlag[j][3] = 1;
							tmpCourse.time.block = 3;
						}
						else if (BlockFlag[j][1] == 0)
						{ // �ٿ�������
							BlockFlag[j][1] = 1;
							tmpCourse.time.block = 1;
						}
						else
						{ // ����
							Flag = 0; // �ſβ��ɹ�
						}
					}
				}
				// һ���ſι������
				if (Flag)
				{ // �ſγɹ�
					k++;
					FinalSchedule[i].AddTail(tmpCourse);
					ScheduleDetails[i].Delete(curDay, tmpCourse);
					curDay = 0; // ����
				}
				else
				{ // �ſβ��ɹ�
					curDay++; // ��������һ�ſ�
				}
			}
		}
	}
	
	// ����γ̱�
	for (int i = 0; i < TermsNum; i++)
	{ // ����ÿѧ��
		m_outFile << "��" << i + 1 << "ѧ�ڿγ̱�" << endl;
		m_outFile << "   �ڴ�\t\t\t����һ\t\t\t���ڶ�\t\t\t������\t\t\t������\t\t\t������\n";
		m_outFile << " ��һ��\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(0, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		m_outFile << " �ڶ���\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(0, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		m_outFile << "\n�μ���Ϣ\n\n";

		m_outFile << " ������\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(1, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		m_outFile << " ���Ľ�\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(1, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		m_outFile << " �����\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(1, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";

		m_outFile << "\n�����Ϣ\n\n";

		m_outFile << " ������\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(2, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		m_outFile << " ���߽�\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(2, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";

		m_outFile << "\n�μ���Ϣ\n\n";
		
		m_outFile << " �ڰ˽�\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(3, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		m_outFile << " �ھŽ�\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(3, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		m_outFile << " ��ʮ��\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // ����ͬһblock�µ�ÿ����
			int id;
			if ((id = check(3, j, FinalSchedule[i])) != -1)
			{
				m_outFile << " " << id - 1 << "\t\t\t\t   ";
			}
			else
			{
				m_outFile << "\t\t\t\t";
			}
		}
		m_outFile << "\n";
		
		m_outFile << "\n����ϰ\n";

		m_outFile << "\n\n";
	}

};

template <int TermsNum>
void SimpleSchedule<TermsNum>::Run()
{
	ReadCourseInfo(); // ��ȡ�γ���Ϣ
	TopSort(); // ��������
	GenerateSchedule();  // ���ɿγ̱�
};

template <int TermsNum>
int SimpleSchedule<TermsNum>::check(int j, int k, SqList<CourseType>& List)
{
	for (int m = 0; m < List.Length(); m++)
	{
		CourseType tmpCourse;
		List.GetElem(m, tmpCourse);
		//cout << tmpCourse.id << " " << tmpCourse.time.weekday << " " << tmpCourse.time.block << endl;
		if (tmpCourse.time.weekday == k + 1 && tmpCourse.time.block == j)
		{
			return tmpCourse.id;
		}
	}
	return -1;
};
#endif
#if !defined(SIMPLE_SCHEDULE_H)
#define SIMPLE_SCHEDULE_H

#include "../../include/adj_lst_graph.h"
#include "../../include/graph_base.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
// 排课结果
struct Time
{
	int weekday; // 星期几 1~5
	int block; // 第几节课 0~3
	// 重载赋值运算符
	Time& operator= (const Time& t)
	{
		weekday = t.weekday;
		block = t.block;
		return *this;
	}
};
// 课程结构
struct CourseType
{
	int id; // 课程编号
	string name; // 课程名称
	int credit; // 学时
	int term; // 开课学期(0~8, 0代表没有指定开课学期)
	Time time; // 上课时间
	// 重载赋值运算符
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
	int check(int j, int k, SqList<CourseType>& List); // 检查是否有课
	

	// 私有数据
	ifstream m_inFile; //输入文件
	ofstream m_outFile; //输出文件

	AdjListGraph<CourseType, int> *m_pGraph; //课程表
	
	CourseType *m_pCourseInfo; //所有课程信息,作为结点
	int m_nCourseNum; //课程数量
	GraphEdge<int>* m_pEdge; //存储边,用于初始化图
	int m_nEdgeNum; //边数量

	SqList<int> m_qTopSortResult; //拓扑排序结果
	int m_nCourseNumPerTerm[TermsNum]; //每学期课程数量
	SqList<int>* m_scheduleResult[TermsNum]; //各学期排课结果,总共8个学期
};

template <int TermsNum>
SimpleSchedule<TermsNum>::SimpleSchedule()
{
	// 指针
	m_pCourseInfo = NULL;
	m_pGraph = NULL;
	for (int i = 0; i < TermsNum; i++)
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
	for (int i = 0; i < TermsNum; i++)
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
	
	// 分别对个学期进行排课
	// 策略：每天上课学时尽量均匀，考虑到周五一般活动较多，尽量少排课，课程课时数过多则进行合理拆分且拆分后的课程尽量不在相邻两天上, 所有课程上下午尽量分布均匀
	// step1: 课时拆分 4 = 2 + 2, 5 = 3 + 2, 6 = 3 + 3
	SqList<CourseType> *ScheduleDetails = new SqList<CourseType>[TermsNum]; // 最后排课结果详细信息
	for (int i = 0; i < TermsNum; i++)
	{ //对每学期遍历
		int len = m_scheduleResult[i]->Length();
		int tmp;
		CourseType tmpCourse;
		// 首先将初始课程加入详细信息，对大课进行第一次拆分
		//cout << "第一次拆分" << endl;
		for (int j = 0; j < len; j++)
		{ // 对每门课遍历
			m_scheduleResult[i]->GetElem(j, tmp);
			tmpCourse = m_pCourseInfo[tmp];
			if (m_pCourseInfo[tmp].credit == 4 || m_pCourseInfo[tmp].credit == 5 || m_pCourseInfo[tmp].credit == 6)
			{ // 课时拆分
				tmpCourse.credit = tmpCourse.credit%2 == 0 ? tmpCourse.credit/2 : tmpCourse.credit/2 + 1;
			}
			ScheduleDetails[i].AddTail(tmpCourse);
		}
		// 然后将大课的第二部分加入详细信息
		for (int j = 0; j < len; j++)
		{ //对每门课遍历
			m_scheduleResult[i]->GetElem(j, tmp);
			tmpCourse = m_pCourseInfo[tmp];
			if (m_pCourseInfo[tmp].credit == 4 || m_pCourseInfo[tmp].credit == 5 || m_pCourseInfo[tmp].credit == 6)
			{ // 课时拆分
				tmpCourse.credit = tmpCourse.credit / 2;
				ScheduleDetails[i].AddTail(tmpCourse); //第二次只对大课进行添加
			}
		}
	}
	

	// step2: 课程排课
	// 策略：课程尽量排在星期靠前的时间段，且尽量保证每天都有课
	SqList<CourseType>* FinalSchedule = new SqList<CourseType>[TermsNum]; // 存储最终排课结果
	for (int i = 0; i < TermsNum; i++)
	{ //对每学期遍历

		// 2.1: 计算每天课程数
		int NumPerDay[5]; // 每天课程数
		int len = ScheduleDetails[i].Length(), AverageNum = len / 5, Overleft = len - AverageNum * 5;
		for (int j = 0; j < 5; j++)
		{ // 遍历每周五天
			NumPerDay[j] = AverageNum;
			if (Overleft > 0)
			{ // 多余的课在星期靠前的几天均匀分配
				NumPerDay[j]++;
				Overleft--;
			}
		}

		// 2.2: 确定具体上课时间
		// 策略：135优先排课在上午， 24优先排课在下午，以保证上下午分配均匀
		
		int curDay = 0, BlockFlag[5][4]; // curDay表示当前课程索引，BlockFlag表示某时间段是否有课
		CourseType tmpCourse;
		// 初始化 BlockFlag
		for (int j = 0; j < 5; j++)
			for (int k = 0; k < 4; k++)
				BlockFlag[j][k] = 0;

		// 开始排课
		for (int j = 0; j < 5; j++)
		{ // 对每周五天遍历
			int k = 0;
			while (k < NumPerDay[j])
			{ // 每天都要排NumPerDay[j]节课
				int Flag = 1; // 用于记录是否排课成功
				if (j % 2 == 0)
				{ // 周135优先上午
					ScheduleDetails[i].GetElem(curDay, tmpCourse);
					tmpCourse.time.weekday = j + 1;
					if (tmpCourse.credit == 2)
					{ // 两学时排0、2时段
						if (BlockFlag[j][0] == 0)
						{ // 优先上午
							BlockFlag[j][0] = 1;
							tmpCourse.time.block = 0;
						}
						else if (BlockFlag[j][2] == 0)
						{ // 在考虑下午
							BlockFlag[j][2] = 1;
							tmpCourse.time.block = 2;
						}
						else
						{ // 均满
							Flag = 0; // 排课不成功
						}
					}
					else
					{ // 三学时排1、3时段
						if (BlockFlag[j][1] == 0)
						{ // 优先上午
							BlockFlag[j][1] = 1;
							tmpCourse.time.block = 1;
						}
						else if (BlockFlag[j][3] == 0)
						{ // 再考虑下午
							BlockFlag[j][3] = 1;
							tmpCourse.time.block = 3;
						}
						else
						{ // 均满
							Flag = 0; // 排课不成功
						}
					}
				}
				else
				{ // 周24优先下午
					ScheduleDetails[i].GetElem(curDay, tmpCourse);
					tmpCourse.time.weekday = j + 1;
					if (tmpCourse.credit == 2)
					{ // 两学时排0、2时段
						if (BlockFlag[j][2] == 0)
						{ // 优先下午
							BlockFlag[j][2] = 1;
							tmpCourse.time.block = 2;
						}
						else if (BlockFlag[j][0] == 0)
						{ // 再考虑上午
							BlockFlag[j][0] = 1;
							tmpCourse.time.block = 0;
						}
						else
						{ // 均满
							Flag = 0; // 排课不成功
						}
					}
					else
					{ // 三学时排1、3时段
						if (BlockFlag[j][3] == 0)
						{ // 优先下午
							BlockFlag[j][3] = 1;
							tmpCourse.time.block = 3;
						}
						else if (BlockFlag[j][1] == 0)
						{ // 再考虑上午
							BlockFlag[j][1] = 1;
							tmpCourse.time.block = 1;
						}
						else
						{ // 均满
							Flag = 0; // 排课不成功
						}
					}
				}
				// 一次排课过程完成
				if (Flag)
				{ // 排课成功
					k++;
					FinalSchedule[i].AddTail(tmpCourse);
					ScheduleDetails[i].Delete(curDay, tmpCourse);
					curDay = 0; // 重置
				}
				else
				{ // 排课不成功
					curDay++; // 继续排下一门课
				}
			}
		}
	}
	
	// 输出课程表
	for (int i = 0; i < TermsNum; i++)
	{ // 遍历每学期
		m_outFile << "第" << i + 1 << "学期课程表" << endl;
		m_outFile << "   节次\t\t\t星期一\t\t\t星期二\t\t\t星期三\t\t\t星期四\t\t\t星期五\n";
		m_outFile << " 第一节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		m_outFile << " 第二节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		m_outFile << "\n课间休息\n\n";

		m_outFile << " 第三节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		m_outFile << " 第四节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		m_outFile << " 第五节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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

		m_outFile << "\n午间休息\n\n";

		m_outFile << " 第六节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		m_outFile << " 第七节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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

		m_outFile << "\n课间休息\n\n";
		
		m_outFile << " 第八节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		m_outFile << " 第九节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		m_outFile << " 第十节\t\t\t";
		for (int j = 0; j < 5; j++)
		{ // 遍历同一block下的每五天
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
		
		m_outFile << "\n晚自习\n";

		m_outFile << "\n\n";
	}

};

template <int TermsNum>
void SimpleSchedule<TermsNum>::Run()
{
	ReadCourseInfo(); // 读取课程信息
	TopSort(); // 拓扑排序
	GenerateSchedule();  // 生成课程表
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
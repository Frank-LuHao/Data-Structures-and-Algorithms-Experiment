// 第三次实验.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "../../include/huf_tree(homework).h"
#include <iostream>
using namespace std;

int main()
{
	HufTree<char, int> hufTree;
	char e[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int wt[] = { 5, 4, 3, 2, 1, 1 };
	hufTree.CreateHufTree(e, wt, 6);
	CharString strCode;
	hufTree.Ecode('a', strCode);
	cout << "a的编码为：" << strCode.ToCStr() << endl;
	hufTree.Ecode('b', strCode);
	cout << "b的编码为：" << strCode.ToCStr() << endl;
	hufTree.Ecode('c', strCode);
	cout << "c的编码为：" << strCode.ToCStr() << endl;
	hufTree.Ecode('d', strCode);
	cout << "d的编码为：" << strCode.ToCStr() << endl;
	hufTree.Ecode('e', strCode);
	cout << "e的编码为：" << strCode.ToCStr() << endl;
	hufTree.Ecode('f', strCode);
	cout << "f的编码为：" << strCode.ToCStr() << endl;
	/*SqList<char> eLst;
	CharString strCode1("101");
	hufTree.Dcode(eLst, strCode1);
	cout << "101的译码为：";
	for (int i = 0; i < eLst.Length(); i++)
		cout << eLst.GetElem(i) << " ";
	cout << endl;*/
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

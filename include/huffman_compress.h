#if !defined(_HUFFMAN_COMPRESS_H_)
#define _HUFFMAN_COMPRESS_H_
#include "huf_tree(homework).h"
#include <iostream>
using namespace std;

// 字符缓存器
struct BufferType
{
	char ch;
	unsigned int bits;
};

// 哈夫曼压缩类
class HuffmanCompress
{
public:
	HuffmanCompress();
	~HuffmanCompress();
	void Compress(); // 压缩
	void Decompress(); // 解压缩

private:
	void Write(unsigned int bit); // 写入
	void WriteToOutFile(); // 剩余字符写入文件
	HufTree<unsigned char, unsigned int> m_HufTree; // 哈夫曼树
	BufferType m_Buffer; // 字符缓存器
	FILE* m_pInFile, * m_pOutFile; // 输入输出文件
};

HuffmanCompress::HuffmanCompress() : m_Buffer({ 0, 0 })
{
	m_pInFile = NULL;
	m_pOutFile = NULL;
}

HuffmanCompress::~HuffmanCompress()
{
	if (m_pInFile != NULL)
		fclose(m_pInFile);
	if (m_pOutFile != NULL)
		fclose(m_pOutFile);
}

void HuffmanCompress::Write(unsigned int bit)
{
	m_Buffer.bits++;
	m_Buffer.ch = m_Buffer.ch << 1 | bit;
	if (m_Buffer.bits == 8)
	{
		fputc(m_Buffer.ch, m_pOutFile);
		m_Buffer.ch = 0;
		m_Buffer.bits = 0;
	}
}

void HuffmanCompress::WriteToOutFile()
{
	if (m_Buffer.bits > 0)
	{
		m_Buffer.ch = m_Buffer.ch << (8 - m_Buffer.bits);
		fputc(m_Buffer.ch, m_pOutFile);
	}
}

void HuffmanCompress::Compress()
{
	char infName[256], outfName[256];

	// 源文件读入
	cout << "请输入源文件名称:";
	cin >> infName;
	if ((m_pInFile = fopen(infName, "rb")) == NULL)
	{
		cout << "文件不存在！" << endl;
		exit(1);
	}
	fgetc(m_pInFile);
	if (feof(m_pInFile))
	{
		cout << "文件为空！" << endl;
		exit(2);
	}

	// 目标文件读入
	cout << "请输入目标文件名称:";
	cin >> outfName;
	if ((m_pOutFile = fopen(outfName, "wb")) == NULL)
	{
		cout << "" << endl;
		exit(3);
	}

	cout << "正在压缩..." << endl;

	// 压缩信息写入
	const int N = 256;
	unsigned char ch[N];   // 字符集256个字符
	unsigned int w[N]; // 字符权值
	int size = 0; // 字符数
	unsigned char cha;

	// 初始化
	for (int i = 0; i < N; i++)
	{
		ch[i] = (char)i;
		w[i] = 0;
	}

	// 统计字符权值
	rewind(m_pInFile); // 重置文件指针
	cha = fgetc(m_pInFile);
	while (!feof(m_pInFile))
	{
		w[(unsigned int)cha]++;
		size++;
		cha = fgetc(m_pInFile);
	}

	// 创建哈夫曼树
	m_HufTree.CreateHufTree(ch, w, N);
	rewind(m_pOutFile);

	// 写入压缩信息
	fwrite(&size, sizeof(int), 1, m_pOutFile); // 写入字符数
	for (int i = 0; i < N; i++)
	{
		fwrite(&w[i], sizeof(int), 1, m_pOutFile); // 写入字符权值
	}

	// 压缩
	m_Buffer.bits = 0;
	m_Buffer.ch = 0;
	rewind(m_pInFile);
	cha = fgetc(m_pInFile);
	while (!feof(m_pInFile))
	{
		CharString code;
		m_HufTree.Ecode(cha, code); // 编码
		for (int i = 0; i < code.Length(); i++)
		{
			if (code[i] == '0')
				Write(0);
			else
				Write(1);
		}
		cha = fgetc(m_pInFile);
	}
	WriteToOutFile();

	// 压缩完成
	cout << "压缩成功!" << endl;
	cout << "压缩比：" << (double)ftell(m_pOutFile) / ftell(m_pInFile) << endl;
	fclose(m_pInFile);
	fclose(m_pOutFile);

}

void HuffmanCompress::Decompress() {
	char infName[256], outfName[256];

	// 压缩文件读入
	cout << "请输入压缩文件名称:";
	cin >> infName;
	if ((m_pInFile = fopen(infName, "rb")) == NULL)
	{
		cout << "文件不存在!" << endl;
		exit(1);
	}
	fgetc(m_pInFile);
	if (feof(m_pInFile))
	{
		cout << "文件为空!" << endl;
		exit(2);
	}

	// 目标文件读入
	cout << "请输入目标文件名称:";
	cin >> outfName;
	if ((m_pOutFile = fopen(outfName, "wb")) == NULL)
	{
		cout << "文件不存在!" << endl;
		exit(3);
	}

	cout << "正在解压缩..." << endl;

	// 读取压缩信息
	const int N = 256;
	unsigned char ch[N];   // 字符集256个字符
	unsigned int w[N]; // 字符权值
	int size = 0; // 字符数
	unsigned char cha;

	// 读取压缩信息
	rewind(m_pInFile);
	fread(&size, sizeof(int), 1, m_pInFile); // 读取字符数
	for (int i = 0; i < N; i++) {
		ch[i] = (char)i;
		fread(&w[i], sizeof(int), 1, m_pInFile); // 读取字符权值
	}
	m_HufTree.CreateHufTree(ch, w, N); // 创建哈夫曼树

	// 读取压缩文件
	CharString code = "";
	cha = fgetc(m_pInFile);
	while (!feof(m_pInFile))
	{ //读取压缩文件内容，存储成CharString类型，便于解码
		for (int i = 0; i < 8; i++) {
			if (cha < 128)
				code += "0";
			else
				code += "1";
			cha <<= 1;
		}
		cha = fgetc(m_pInFile);
	}

	// 解码
	SqList<unsigned char> result;
	m_HufTree.Dcode(result, code);

	// 写入解压缩文件
	rewind(m_pOutFile);
	for (int i = 0; i < size; i++) {
		unsigned char tmp;
		result.GetElem(i, tmp);
		fputc(tmp, m_pOutFile);
	}

	// 解压缩完成
	cout << "解压缩成功！" << endl;
	fclose(m_pInFile);
	fclose(m_pOutFile);

}
#endif
#if !defined(_HUFFMAN_COMPRESS_H_)
#define _HUFFMAN_COMPRESS_H_
#include "huf_tree(homework).h"
#include <iostream>
using namespace std;

// �ַ�������
struct BufferType
{
	char ch;
	unsigned int bits;
};

// ������ѹ����
class HuffmanCompress
{
public:
	HuffmanCompress();
	~HuffmanCompress();
	void Compress(); // ѹ��
	void Decompress(); // ��ѹ��

private:
	void Write(unsigned int bit); // д��
	void WriteToOutFile(); // ʣ���ַ�д���ļ�
	HufTree<unsigned char, unsigned int> m_HufTree; // ��������
	BufferType m_Buffer; // �ַ�������
	FILE* m_pInFile, * m_pOutFile; // ��������ļ�
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

	// Դ�ļ�����
	cout << "������Դ�ļ�����:";
	cin >> infName;
	if ((m_pInFile = fopen(infName, "rb")) == NULL)
	{
		cout << "�ļ������ڣ�" << endl;
		exit(1);
	}
	fgetc(m_pInFile);
	if (feof(m_pInFile))
	{
		cout << "�ļ�Ϊ�գ�" << endl;
		exit(2);
	}

	// Ŀ���ļ�����
	cout << "������Ŀ���ļ�����:";
	cin >> outfName;
	if ((m_pOutFile = fopen(outfName, "wb")) == NULL)
	{
		cout << "" << endl;
		exit(3);
	}

	cout << "����ѹ��..." << endl;

	// ѹ����Ϣд��
	const int N = 256;
	unsigned char ch[N];   // �ַ���256���ַ�
	unsigned int w[N]; // �ַ�Ȩֵ
	int size = 0; // �ַ���
	unsigned char cha;

	// ��ʼ��
	for (int i = 0; i < N; i++)
	{
		ch[i] = (char)i;
		w[i] = 0;
	}

	// ͳ���ַ�Ȩֵ
	rewind(m_pInFile); // �����ļ�ָ��
	cha = fgetc(m_pInFile);
	while (!feof(m_pInFile))
	{
		w[(unsigned int)cha]++;
		size++;
		cha = fgetc(m_pInFile);
	}

	// ������������
	m_HufTree.CreateHufTree(ch, w, N);
	rewind(m_pOutFile);

	// д��ѹ����Ϣ
	fwrite(&size, sizeof(int), 1, m_pOutFile); // д���ַ���
	for (int i = 0; i < N; i++)
	{
		fwrite(&w[i], sizeof(int), 1, m_pOutFile); // д���ַ�Ȩֵ
	}

	// ѹ��
	m_Buffer.bits = 0;
	m_Buffer.ch = 0;
	rewind(m_pInFile);
	cha = fgetc(m_pInFile);
	while (!feof(m_pInFile))
	{
		CharString code;
		m_HufTree.Ecode(cha, code); // ����
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

	// ѹ�����
	cout << "ѹ���ɹ�!" << endl;
	cout << "ѹ���ȣ�" << (double)ftell(m_pOutFile) / ftell(m_pInFile) << endl;
	fclose(m_pInFile);
	fclose(m_pOutFile);

}

void HuffmanCompress::Decompress() {
	char infName[256], outfName[256];

	// ѹ���ļ�����
	cout << "������ѹ���ļ�����:";
	cin >> infName;
	if ((m_pInFile = fopen(infName, "rb")) == NULL)
	{
		cout << "�ļ�������!" << endl;
		exit(1);
	}
	fgetc(m_pInFile);
	if (feof(m_pInFile))
	{
		cout << "�ļ�Ϊ��!" << endl;
		exit(2);
	}

	// Ŀ���ļ�����
	cout << "������Ŀ���ļ�����:";
	cin >> outfName;
	if ((m_pOutFile = fopen(outfName, "wb")) == NULL)
	{
		cout << "�ļ�������!" << endl;
		exit(3);
	}

	cout << "���ڽ�ѹ��..." << endl;

	// ��ȡѹ����Ϣ
	const int N = 256;
	unsigned char ch[N];   // �ַ���256���ַ�
	unsigned int w[N]; // �ַ�Ȩֵ
	int size = 0; // �ַ���
	unsigned char cha;

	// ��ȡѹ����Ϣ
	rewind(m_pInFile);
	fread(&size, sizeof(int), 1, m_pInFile); // ��ȡ�ַ���
	for (int i = 0; i < N; i++) {
		ch[i] = (char)i;
		fread(&w[i], sizeof(int), 1, m_pInFile); // ��ȡ�ַ�Ȩֵ
	}
	m_HufTree.CreateHufTree(ch, w, N); // ������������

	// ��ȡѹ���ļ�
	CharString code = "";
	cha = fgetc(m_pInFile);
	while (!feof(m_pInFile))
	{ //��ȡѹ���ļ����ݣ��洢��CharString���ͣ����ڽ���
		for (int i = 0; i < 8; i++) {
			if (cha < 128)
				code += "0";
			else
				code += "1";
			cha <<= 1;
		}
		cha = fgetc(m_pInFile);
	}

	// ����
	SqList<unsigned char> result;
	m_HufTree.Dcode(result, code);

	// д���ѹ���ļ�
	rewind(m_pOutFile);
	for (int i = 0; i < size; i++) {
		unsigned char tmp;
		result.GetElem(i, tmp);
		fputc(tmp, m_pOutFile);
	}

	// ��ѹ�����
	cout << "��ѹ���ɹ���" << endl;
	fclose(m_pInFile);
	fclose(m_pOutFile);

}
#endif
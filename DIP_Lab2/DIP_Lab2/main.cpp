// image1.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>


using namespace std;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;

const int _MAXN = 4000;


typedef struct  tagBITMAPFILEHEADER
{
	DWORD bfSize;//�ļ���С   
	WORD bfReserved1;//������   
	WORD bfReserved2;//������   
	DWORD bfOffBits;//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���   
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;//��Ϣͷ��С   
	LONG biWidth;//ͼ����   
	LONG biHeight;//ͼ��߶�   
	WORD biPlanes;//λƽ����������Ϊ1   
	WORD biBitCount;//ÿ����λ��   
	DWORD  biCompression; //ѹ������   
	DWORD  biSizeImage; //ѹ��ͼ���С�ֽ���   
	LONG  biXPelsPerMeter; //ˮƽ�ֱ���   
	LONG  biYPelsPerMeter; //��ֱ�ֱ���   
	DWORD  biClrUsed; //λͼʵ���õ���ɫ����   
	DWORD  biClrImportant; //��λͼ����Ҫ��ɫ����   
}BITMAPINFOHEADER; //λͼ��Ϣͷ����   

typedef struct tagRGBQUAD
{
	BYTE rgbBlue; //����ɫ����ɫ����   
	BYTE rgbGreen; //����ɫ����ɫ����   
	BYTE rgbRed; //����ɫ�ĺ�ɫ����   
	BYTE rgbReserved; //����ֵ   
}RGBQUAD;//��ɫ�嶨��   

		 //������Ϣ   
typedef struct tagIMAGEDATA
{
	BYTE r;
	BYTE g;
	BYTE b;
}IMAGEDATA;

BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
IMAGEDATA data2 [_MAXN][_MAXN];
IMAGEDATA data1 [_MAXN][_MAXN];//�洢������Ϣ   

void showBmpHead(BITMAPFILEHEADER pBmpHead)//��ʾλͼ�ļ�ͷ��Ϣ  
{
	cout << "λͼ�ļ�ͷ:" << endl;
	cout << "�ļ���С:" << pBmpHead.bfSize << endl;
	cout << "������_1:" << pBmpHead.bfReserved1 << endl;
	cout << "������_2:" << pBmpHead.bfReserved2 << endl;
	cout << "ʵ��λͼ���ݵ�ƫ���ֽ���:" << pBmpHead.bfOffBits << endl << endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead)
{
	cout << "λͼ��Ϣͷ:" << endl;
	cout << "�ṹ��ĳ���:" << pBmpInforHead.biSize << endl;
	cout << "λͼ��:" << pBmpInforHead.biWidth << endl;
	cout << "λͼ��:" << pBmpInforHead.biHeight << endl;
	cout << "biPlanesƽ����:" << pBmpInforHead.biPlanes << endl;
	cout << "biBitCount������ɫλ��:" << pBmpInforHead.biBitCount << endl;
	cout << "ѹ����ʽ:" << pBmpInforHead.biCompression << endl;
	cout << "biSizeImageʵ��λͼ����ռ�õ��ֽ���:" << pBmpInforHead.biSizeImage << endl;
	cout << "X����ֱ���:" << pBmpInforHead.biXPelsPerMeter << endl;
	cout << "Y����ֱ���:" << pBmpInforHead.biYPelsPerMeter << endl;
	cout << "ʹ�õ���ɫ��:" << pBmpInforHead.biClrUsed << endl;
	cout << "��Ҫ��ɫ��:" << pBmpInforHead.biClrImportant << endl;
}

void checkRGB(int &a)//���������ֵ
{
	if (a >255)
	{
		a = 255;
	}
	else
	{
		if (a < 0)
		{
			a = 0;
		}
	}
}

void readFile(char *strFile)//����ͼƬ����������
{
	FILE *fpi;
	fpi = fopen(strFile, "rb");
	if (fpi != NULL)
	{
		//�ȶ�ȡ�ļ�����   
		WORD bfType;
		fread(&bfType, 1, sizeof(WORD), fpi);
		if (0x4d42 != bfType)
		{
			cout << "the file is not a bmp file!" << endl;
			return exit(0);
		}
		//��ȡbmp�ļ����ļ�ͷ����Ϣͷ   
		fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);
		//showBmpHead(strHead);//��ʾ�ļ�ͷ   
		fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);
		//showBmpInforHead(strInfo);//��ʾ�ļ���Ϣͷ   

		//��ʼ��
		for (int i = 0; i < 2000; i++)
		{
			for (int j = 0; j < 2000; j++)
			{
				data1[i][j].r = 0;
				data1[i][j].g = 0;
				data1[i][j].b = 0;
			}
		}
		int i, j;
		for (i = 0; i < strInfo.biHeight; i++)
		{
			for (j = 0; j < strInfo.biWidth; j++)
			{
				fread(&data1[i][j], 1, sizeof(struct tagIMAGEDATA), fpi);
			}
			j = j * 3;//���油0�Ĵ���
			while ((j % 4) != 0)
			{
				char ch;
				ch = fgetc(fpi);
				++j;
			}

		}

		fclose(fpi);
	}
	else
	{
		cout << "file open error!" << endl;
		exit(0);
	}
}

void saveFile(char *strFile) //����bmpͼƬ 
{
	FILE *fpw;

	if ((fpw = fopen(strFile, "wb")) == NULL)
	{
		cout << "create the bmp file error!" << endl;
		exit(0);
	}
	WORD bfType = 0x4d42;
	fwrite(&bfType, 1, sizeof(WORD), fpw);
	fwrite(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpw);
	fwrite(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpw);

	//������������  
	int i, j;
	for (i = 0; i < strInfo.biHeight; ++i)
	{
		for (j = 0; j < strInfo.biWidth; ++j)
		{
			fwrite(&data1[i][j].r, 1, 1, fpw);
			fwrite(&data1[i][j].g, 1, 1, fpw);
			fwrite(&data1[i][j].b, 1, 1, fpw);
		}
		j = j * 3;//���油0
		while ((j % 4) != 0)
		{
			char ch;
			ch = fputc('0', fpw);
			++j;
		}
	}

	fclose(fpw);
}

void changeY(char *inputName, char *outputName)
{
	int r, g, b;
	double Y, U, V;

	readFile(inputName);
	for (int x = 0; x < strInfo.biHeight; x++)
	{
		for (int y = 0; y < strInfo.biWidth; y++)
		{
			r = data1[x][y].r;
			g = data1[x][y].g;
			b = data1[x][y].b;
			Y = (0.299 * r) + (0.587 * g) + (0.114 * b);//ת����YUV
			U = (-0.147 * r) + (-0.289 * g) + (0.435 * b);
			V = (0.615 * r) + (-0.515 * g) + (-0.1 * b);

			Y = 150;//�ı�Yֵ
			r = int(Y + (1.140 * V));//ת��ΪRGB���
			g = int(Y + (-0.395 * U) + (-0.581 * V));
			b = int(Y + (2.032 * U));
			checkRGB(r);//���������ֵ
			checkRGB(g);
			checkRGB(b);
			data1[x][y].r = BYTE(r);
			data1[x][y].g = BYTE(g);
			data1[x][y].b = BYTE(b);
		}
	}
	saveFile(outputName);
}

void onlyY(char *inputName, char *outputName)
{
	int r, g, b;
	double Y, U, V;

	readFile(inputName);
	for (int x = 0; x < strInfo.biHeight; x++)
	{
		for (int y = 0; y < strInfo.biWidth; y++)
		{
			r = data1[x][y].r;
			g = data1[x][y].g;
			b = data1[x][y].b;
			Y = (0.299 * r) + (0.587 * g) + (0.114 * b);//ת����YUV
			U = 0;
			V = 0;
			r = int(Y + (1.140 * V));//ת��ΪRGB���
			g = int(Y + (-0.395 * U) + (-0.581 * V));
			b = int(Y + (2.032 * U));
			checkRGB(r);//���������ֵ
			checkRGB(g);
			checkRGB(b);
			data1[x][y].r = BYTE(r);
			data1[x][y].g = BYTE(g);
			data1[x][y].b = BYTE(b);
		}
	}
	saveFile(outputName);
}

void binarization(char *inputName, char *outputName)//binarization
{
	readFile(inputName);
	int r = 0;
	int g = 0;
	int b = 0;

	int max = -1;
	int min = 3000;
	//�������Сֵ
	for (int x = 0; x<strInfo.biHeight; x++)
	{
		for (int y = 0; y<strInfo.biWidth; y++)
		{
			r = data1[x][y].r;
			if (r < min)
			{
				min = r;
			}
			if (r > max)
			{
				max = r;
			}
		}
	}


	int threshold = min + 1;
	double betweenMax = -1;
	int t = 0;
	//����ֵ
	while (threshold <= max)
	{
		int i, j;
		int Nf = 0;//ǰ��������
		int Nb = 0;//����������
		int N = 0;//��������
		int f = 0;//ǰ����ֵ
		int b = 0;//������ֵ
		double miuF = 0;//ǰ��ƽ��ֵ
		double miuB = 0;//����ƽ��ֵ
		double between = 0;

		for (i = 0; i < strInfo.biHeight; i++)
		{
			for (j = 0; j <strInfo.biWidth; j++)
			{
				r = data1[i][j].r;
				if (r >= threshold)
				{
					Nf++;
					f = f + r;
				}
				else
				{
					Nb++;
					b = b + r;
				}
			}
		}
		N = Nb + Nf;
		miuF = f / Nf;
		miuB = b / Nb;

		between = ((Nf * Nb) * ((miuF - miuB) * (miuF - miuB))) / (N*N);
		if (between > betweenMax)
		{
			betweenMax = between;//�����betweenֵ
			t = threshold;
		}
		threshold++;
	}
	//��ֵ��
	for (int i = 0; i < strInfo.biHeight; i++)
	{
		for (int j = 0; j <strInfo.biWidth; j++)
		{
			r = data1[i][j].r;
			if (r >= t)
			{
				r = 255;
				g = 255;
				b = 255;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;
			}
			data1[i][j].r = r;//����rgb
			data1[i][j].g = g;
			data1[i][j].b = b;
		}
	}

	saveFile(outputName);
}

void delation(char *inputName, char *outputName)
{
	readFile(inputName);

	int structElement[3][3] = { 255,0,255,0,0,0,255,0,255 };//�ṹԪ
	int deviationX[3][3] = { -1,-1,-1,0,0,0,1,1,1 };//��ƫ��
	int deviationY[3][3] = { -1,0,1,-1,0,1,-1,0,1 };//��ƫ��
	int i, j, k, p;
	int value;

	//����ͼ��
	for (i = 0; i < _MAXN; i++)
	{
		for (j = 0; j < _MAXN; j++)
		{
			data2[i][j] = data1[i][j];
		}
	}

	bool tag = false;
	for (i = 1; i < strInfo.biHeight - 1; i++)
	{
		for (j = 1; j < strInfo.biWidth - 1; j++)
		{
			//�����Ƿ��ཻ
			tag = false;
			for (k = 0; k < 3; k++)
			{
				for (p = 0; p < 3; p++)
				{
					value = data2[(i + deviationX[k][p])][(j + deviationY[k][p])].r;
					if ((structElement[k][p] == 0) && (structElement[k][p] == value))//��һ���ط��ཻ������λ������Ϊ0������ѭ������������
					{
						data1[i][j].r = 0;
						data1[i][j].g = 0;
						data1[i][j].b = 0;
						tag = true;
						break;
					}
				}
				if (tag)
					break;
			}
		}
	}

	saveFile(outputName);
}

void erosion(char *inputName, char *outputName)
{
	readFile(inputName);

	int structElement[3][3] = { 255,0,255,0,0,0,255,0,255 };//ʮ�ּ���
															//int structElement[3][3] = {0,0,0,0,0,0,0,0,0};//3*3�ڿ�
	int deviationX[3][3] = { -1,-1,-1,0,0,0,1,1,1 };//��ƫ��
	int deviationY[3][3] = { -1,0,1,-1,0,1,-1,0,1 };//��ƫ��
	int i, j, k, p;
	int value;

	//����ͼ��
	for (i = 0; i < _MAXN; i++)
	{
		for (j = 0; j < _MAXN; j++)
		{
			data2[i][j] = data1[i][j];
		}
	}

	bool tag = false;
	for (i = 1; i < strInfo.biHeight - 1; i++)
	{
		for (j = 1; j < strInfo.biWidth - 1; j++)
		{
			tag = true;
			for (k = 0; k < 3; k++)
			{
				for (p = 0; p < 3; p++)
				{
					value = data2[(i + deviationX[k][p])][(j + deviationY[k][p])].r;
					if ((structElement[k][p] == 0) && (structElement[k][p] != value))//�����һ���ط������ϣ����Ϊ�٣�����
					{
						tag = false;
						break;
					}
				}
				if (!tag)
					break;
			}
			if (tag)//��������������λ�ø�ֵ
			{
				data1[i][j].r = 0;
				data1[i][j].g = 0;
				data1[i][j].b = 0;
			}
			else
			{
				data1[i][j].r = 255;
				data1[i][j].g = 255;
				data1[i][j].b = 255;
			}
		}
	}

	saveFile(outputName);
}

void opening(char *inputName, char *outputName)
{
	erosion(inputName, "Open_tmp(e).bmp");
	delation("Open_tmp(e).bmp", outputName);
}

void closing(char *inputName, char *outputName)
{
	delation(inputName, "Close_tmp(d).bmp");
	erosion("Close_tmp(d).bmp", outputName);
}

int main()
{
	char inputName[30];//bmp�ļ���  

	cout << "Please input a BMP filename:" << endl;
	cin >> inputName;

	onlyY(inputName, "onlyY.bmp");
	binarization("onlyY.bmp", "binarization.bmp");
	delation("binarization.bmp", "delation.bmp");
	erosion("binarization.bmp", "erosion.bmp");

	opening("binarization.bmp", "open.bmp");
	closing("binarization.bmp", "close.bmp");

	cout << "ok" << endl;

	return 0;
}

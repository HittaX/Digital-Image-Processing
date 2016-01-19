// image1.cpp : 定义控制台应用程序的入口点。
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
	DWORD bfSize;//文件大小   
	WORD bfReserved1;//保留字   
	WORD bfReserved2;//保留字   
	DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数   
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;//信息头大小   
	LONG biWidth;//图像宽度   
	LONG biHeight;//图像高度   
	WORD biPlanes;//位平面数，必须为1   
	WORD biBitCount;//每像素位数   
	DWORD  biCompression; //压缩类型   
	DWORD  biSizeImage; //压缩图像大小字节数   
	LONG  biXPelsPerMeter; //水平分辨率   
	LONG  biYPelsPerMeter; //垂直分辨率   
	DWORD  biClrUsed; //位图实际用到的色彩数   
	DWORD  biClrImportant; //本位图中重要的色彩数   
}BITMAPINFOHEADER; //位图信息头定义   

typedef struct tagRGBQUAD
{
	BYTE rgbBlue; //该颜色的蓝色分量   
	BYTE rgbGreen; //该颜色的绿色分量   
	BYTE rgbRed; //该颜色的红色分量   
	BYTE rgbReserved; //保留值   
}RGBQUAD;//调色板定义   

		 //像素信息   
typedef struct tagIMAGEDATA
{
	BYTE r;
	BYTE g;
	BYTE b;
}IMAGEDATA;

BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
IMAGEDATA data2 [_MAXN][_MAXN];
IMAGEDATA data1 [_MAXN][_MAXN];//存储像素信息   

void showBmpHead(BITMAPFILEHEADER pBmpHead)//显示位图文件头信息  
{
	cout << "位图文件头:" << endl;
	cout << "文件大小:" << pBmpHead.bfSize << endl;
	cout << "保留字_1:" << pBmpHead.bfReserved1 << endl;
	cout << "保留字_2:" << pBmpHead.bfReserved2 << endl;
	cout << "实际位图数据的偏移字节数:" << pBmpHead.bfOffBits << endl << endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead)
{
	cout << "位图信息头:" << endl;
	cout << "结构体的长度:" << pBmpInforHead.biSize << endl;
	cout << "位图宽:" << pBmpInforHead.biWidth << endl;
	cout << "位图高:" << pBmpInforHead.biHeight << endl;
	cout << "biPlanes平面数:" << pBmpInforHead.biPlanes << endl;
	cout << "biBitCount采用颜色位数:" << pBmpInforHead.biBitCount << endl;
	cout << "压缩方式:" << pBmpInforHead.biCompression << endl;
	cout << "biSizeImage实际位图数据占用的字节数:" << pBmpInforHead.biSizeImage << endl;
	cout << "X方向分辨率:" << pBmpInforHead.biXPelsPerMeter << endl;
	cout << "Y方向分辨率:" << pBmpInforHead.biYPelsPerMeter << endl;
	cout << "使用的颜色数:" << pBmpInforHead.biClrUsed << endl;
	cout << "重要颜色数:" << pBmpInforHead.biClrImportant << endl;
}

void checkRGB(int &a)//处理溢出的值
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

void readFile(char *strFile)//读出图片的像素数据
{
	FILE *fpi;
	fpi = fopen(strFile, "rb");
	if (fpi != NULL)
	{
		//先读取文件类型   
		WORD bfType;
		fread(&bfType, 1, sizeof(WORD), fpi);
		if (0x4d42 != bfType)
		{
			cout << "the file is not a bmp file!" << endl;
			return exit(0);
		}
		//读取bmp文件的文件头和信息头   
		fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);
		//showBmpHead(strHead);//显示文件头   
		fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);
		//showBmpInforHead(strInfo);//显示文件信息头   

		//初始化
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
			j = j * 3;//后面补0的处理
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

void saveFile(char *strFile) //保存bmp图片 
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

	//保存像素数据  
	int i, j;
	for (i = 0; i < strInfo.biHeight; ++i)
	{
		for (j = 0; j < strInfo.biWidth; ++j)
		{
			fwrite(&data1[i][j].r, 1, 1, fpw);
			fwrite(&data1[i][j].g, 1, 1, fpw);
			fwrite(&data1[i][j].b, 1, 1, fpw);
		}
		j = j * 3;//后面补0
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
			Y = (0.299 * r) + (0.587 * g) + (0.114 * b);//转换成YUV
			U = (-0.147 * r) + (-0.289 * g) + (0.435 * b);
			V = (0.615 * r) + (-0.515 * g) + (-0.1 * b);

			Y = 150;//改变Y值
			r = int(Y + (1.140 * V));//转换为RGB输出
			g = int(Y + (-0.395 * U) + (-0.581 * V));
			b = int(Y + (2.032 * U));
			checkRGB(r);//处理溢出的值
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
			Y = (0.299 * r) + (0.587 * g) + (0.114 * b);//转换成YUV
			U = 0;
			V = 0;
			r = int(Y + (1.140 * V));//转换为RGB输出
			g = int(Y + (-0.395 * U) + (-0.581 * V));
			b = int(Y + (2.032 * U));
			checkRGB(r);//处理溢出的值
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
	//找最大、最小值
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
	//找阈值
	while (threshold <= max)
	{
		int i, j;
		int Nf = 0;//前景像素数
		int Nb = 0;//背景像素数
		int N = 0;//总像素数
		int f = 0;//前景总值
		int b = 0;//背景总值
		double miuF = 0;//前景平均值
		double miuB = 0;//背景平均值
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
			betweenMax = between;//找最大between值
			t = threshold;
		}
		threshold++;
	}
	//二值化
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
			data1[i][j].r = r;//设置rgb
			data1[i][j].g = g;
			data1[i][j].b = b;
		}
	}

	saveFile(outputName);
}

void delation(char *inputName, char *outputName)
{
	readFile(inputName);

	int structElement[3][3] = { 255,0,255,0,0,0,255,0,255 };//结构元
	int deviationX[3][3] = { -1,-1,-1,0,0,0,1,1,1 };//行偏差
	int deviationY[3][3] = { -1,0,1,-1,0,1,-1,0,1 };//列偏差
	int i, j, k, p;
	int value;

	//复制图像
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
			//计算是否相交
			tag = false;
			for (k = 0; k < 3; k++)
			{
				for (p = 0; p < 3; p++)
				{
					value = data2[(i + deviationX[k][p])][(j + deviationY[k][p])].r;
					if ((structElement[k][p] == 0) && (structElement[k][p] == value))//有一个地方相交则中心位置设置为0，跳出循环，继续查找
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

	int structElement[3][3] = { 255,0,255,0,0,0,255,0,255 };//十字架型
															//int structElement[3][3] = {0,0,0,0,0,0,0,0,0};//3*3黑框
	int deviationX[3][3] = { -1,-1,-1,0,0,0,1,1,1 };//行偏差
	int deviationY[3][3] = { -1,0,1,-1,0,1,-1,0,1 };//列偏差
	int i, j, k, p;
	int value;

	//复制图像
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
					if ((structElement[k][p] == 0) && (structElement[k][p] != value))//如果有一个地方不符合，标记为假，跳出
					{
						tag = false;
						break;
					}
				}
				if (!tag)
					break;
			}
			if (tag)//符合条件，中心位置赋值
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
	char inputName[30];//bmp文件名  

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

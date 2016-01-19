#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
using namespace std;

void  RGB_TO_YUV(unsigned char *yuvBuff, unsigned char *rgbBuff, int width, int height);
void  YUV_TO_RGB_gray(unsigned char *yuvBuff, unsigned char *rgbBuff, int width, int height);

void  YUV_TO_RGB(unsigned char *yuvBuff, unsigned char *rgbBuff, int width, int height);

int main()
{
	char fileName[30];                        //定义打开图像名字
	unsigned char *buf,*p,*color, *yuv;                                //定义文件读取缓冲区
	int r, g, b, pix;

	FILE *fp;                                 //定义文件指针
	FILE *fpw,*fpw2;                                //定义保存文件指针
	DWORD w, h;                                //定义读取图像的长和宽
	DWORD bitCorlorUsed;                      //定义
	DWORD bitSize;                            //定义图像的大小
	BITMAPFILEHEADER bf;                      //图像文件头
	BITMAPINFOHEADER bi;                      //图像文件头信息

	cout << "Please enter the file name：";
	cin >> fileName;
	if ((fp = fopen(fileName, "rb")) == NULL)
	{
		cout << "File not found！";
		exit(0);
	}
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);	//读取BMP文件头文件
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);	//读取BMP文件头文件信息
	w = bi.biWidth;									//获取图像的宽
	h = bi.biHeight;								//获取图像的高
	bitSize = bi.biSizeImage;						//获取图像的size
	buf = (unsigned char*)malloc(w*h * 3);					//分配缓冲区大小
	fseek(fp, long(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), 0);//定位到像素起始位置
	fread(buf, 1, w*h * 3, fp);                   //开始读取数据

	yuv = (unsigned char*)malloc(w*h * 3);
	p = (unsigned char*)malloc(w*h * 3);					//分配缓冲区大小
	RGB_TO_YUV(yuv, buf, w, h);

	fpw = fopen("gray.bmp", "wb");
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpw);  //写入文件头
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpw);  //写入文件头信息
	YUV_TO_RGB_gray(yuv, p, w, h);
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i<w * 3; i++)
		{
			fwrite(p++, 1, 1, fpw);
		}
	}


	memset(yuv, 0, w*h * 3);
	color = (unsigned char*)malloc(w*h * 3);					//分配缓冲区大小

	RGB_TO_YUV(yuv, buf, w, h);
	fpw2 = fopen("color.bmp", "wb");
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpw2);  //写入文件头
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpw2);  //写入文件头信息
	YUV_TO_RGB(yuv, color, w, h);
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i<w * 3; i++)
		{
			fwrite(color++, 1, 1, fpw2);
		}
	}


	fclose(fpw);
	fclose(fpw2);
	fclose(fp);
}

void  RGB_TO_YUV(unsigned char *yuvBuff, unsigned char *rgbBuff, int width, int height)
{
	int	i, j, total, width3;

	width3 = 3 * width;
	total = width3 * height;
	for (i = 0; i<total; i += width3)
		for (j = 0; j<width3; j += 3)
		{
			// Y
			yuvBuff[i + j] = (unsigned char)(0.299*rgbBuff[i + j] + 0.587*rgbBuff[i + j + 1] + 0.114 * rgbBuff[i + j + 2]);
			// U = 0.493(B-Y)
			// Cb = B-Y
			yuvBuff[i + j + 1] = 128 + (unsigned char)(-0.16874*rgbBuff[i + j ] - 0.33126*rgbBuff[i + j + 1] + 0.5 * rgbBuff[i + j + 2]);
			// V = 0.877(R-Y)
			// Cr = R-Y
			yuvBuff[i + j + 2] = 128 + (unsigned char)(0.5*rgbBuff[i + j ] - 0.41869 *rgbBuff[i + j + 1] - 0.08131 * rgbBuff[i + j + 2]);
		}
}

void  YUV_TO_RGB(unsigned char *yuvBuff, unsigned char *rgbBuff, int width, int height)
{

	int	i, j, total, width3;

	width3 = 3 * width;
	total = width3 * height;
	for (i = 0; i<total; i += width3)
		for (j = 0; j<width3; j += 3)
		{
			// Red
			rgbBuff[i + j ] = yuvBuff[i + j] + 1.13983*(yuvBuff[i + j + 2] - 128);

			// U = 0.493(B-Y)
			// Cb = B-Y
			rgbBuff[i + j + 1] = yuvBuff[i + j] -0.39465*(yuvBuff[i + j + 1]-128)- 0.58060*(yuvBuff[i + j + 2] - 128);

			// V = 0.877(R-Y)
			// Cr = R-Y
			rgbBuff[i + j + 2] = yuvBuff[i + j] + 2.03211*(yuvBuff[i + j + 1] - 128);
		}
}

void  YUV_TO_RGB_gray(unsigned char *yuvBuff, unsigned char *rgbBuff, int width, int height)
{
	int	i, j, total, width3;

	width3 = 3 * width;
	total = width3 * height;
	for (i = 0; i<total; i += width3)
		for (j = 0; j<width3; j += 3)
		{
			rgbBuff[i + j] = yuvBuff[i + j];

			// U = 0.493(B-Y)
			// Cb = B-Y
			rgbBuff[i + j + 1] = yuvBuff[i + j];

			// V = 0.877(R-Y)
			// Cr = R-Y
			rgbBuff[i + j + 2] = yuvBuff[i + j];
		}
}
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
	char fileName[30];                        //�����ͼ������
	unsigned char *buf,*p,*color, *yuv;                                //�����ļ���ȡ������
	int r, g, b, pix;

	FILE *fp;                                 //�����ļ�ָ��
	FILE *fpw,*fpw2;                                //���屣���ļ�ָ��
	DWORD w, h;                                //�����ȡͼ��ĳ��Ϳ�
	DWORD bitCorlorUsed;                      //����
	DWORD bitSize;                            //����ͼ��Ĵ�С
	BITMAPFILEHEADER bf;                      //ͼ���ļ�ͷ
	BITMAPINFOHEADER bi;                      //ͼ���ļ�ͷ��Ϣ

	cout << "Please enter the file name��";
	cin >> fileName;
	if ((fp = fopen(fileName, "rb")) == NULL)
	{
		cout << "File not found��";
		exit(0);
	}
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);	//��ȡBMP�ļ�ͷ�ļ�
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);	//��ȡBMP�ļ�ͷ�ļ���Ϣ
	w = bi.biWidth;									//��ȡͼ��Ŀ�
	h = bi.biHeight;								//��ȡͼ��ĸ�
	bitSize = bi.biSizeImage;						//��ȡͼ���size
	buf = (unsigned char*)malloc(w*h * 3);					//���仺������С
	fseek(fp, long(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), 0);//��λ��������ʼλ��
	fread(buf, 1, w*h * 3, fp);                   //��ʼ��ȡ����

	yuv = (unsigned char*)malloc(w*h * 3);
	p = (unsigned char*)malloc(w*h * 3);					//���仺������С
	RGB_TO_YUV(yuv, buf, w, h);

	fpw = fopen("gray.bmp", "wb");
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpw);  //д���ļ�ͷ
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpw);  //д���ļ�ͷ��Ϣ
	YUV_TO_RGB_gray(yuv, p, w, h);
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i<w * 3; i++)
		{
			fwrite(p++, 1, 1, fpw);
		}
	}


	memset(yuv, 0, w*h * 3);
	color = (unsigned char*)malloc(w*h * 3);					//���仺������С

	RGB_TO_YUV(yuv, buf, w, h);
	fpw2 = fopen("color.bmp", "wb");
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpw2);  //д���ļ�ͷ
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpw2);  //д���ļ�ͷ��Ϣ
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
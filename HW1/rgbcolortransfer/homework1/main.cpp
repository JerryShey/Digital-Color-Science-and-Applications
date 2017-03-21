#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include "MyBMP.h"

using namespace std;

//void Image_color(BMP Img_In, const char *OutputName);
void Image_color(BMP Img_In);
void calcMean(BMP, float*, float*, float*);
void calcVariance(BMP, float, float, float, float*, float*, float*);
string Dec2Bi(float);

int main(int argc, char** argv)
{
	float R = 0.0, G = 0.0, B = 0.0;
	float varR = 0.0, varG = 0.0, varB = 0.0;
	BMP Img;

	Img.ReadFromFile("Lena64x64.bmp");  //讀取的圖檔名字

	calcMean(Img, &R, &G, &B);
	calcVariance(Img, R, G, B, &varR, &varG, &varB);

	cout << "Mean:\t" << R << "\t" << G << "\t" << B << endl;
	
	cout << "Variance:\t" << varR << "\t" << varG << "\t" << varB << endl;
	getchar();
	return 0;
}

void calcMean(BMP img, float* R, float* G, float* B) {
	int cnt = 0;
	for (int i = 0; i < img.TellHeight(); i++)
	{
		for (int j = 0; j < img.TellWidth(); j++)
		{
			RGBApixel NewPixel = img.GetPixel(i, j);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 
			
			*R += NewPixel.Red;
			*G += NewPixel.Green;
			*B += NewPixel.Blue;

			cnt++;

		}
	}
	*R /= cnt;
	*G /= cnt;
	*B /= cnt;
}

void calcVariance(BMP img, float avgR, float avgG, float avgB, float* R, float* G, float* B) {
	int cnt = 0;
	float tempR, tempG, tempB;
	for (int i = 0; i < img.TellHeight(); i++)
	{
		for (int j = 0; j < img.TellWidth(); j++)
		{
			RGBApixel NewPixel = img.GetPixel(i, j);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 
			tempR = 0.0;	tempG = 0.0;	tempB = 0.0;
													
			tempR = NewPixel.Red - avgR;
			*R += tempR*tempR;
			tempG = NewPixel.Green - avgG;
			*G += tempG*tempG;
			tempB = NewPixel.Blue - avgB;
			*B += tempB*tempB;

			cnt++;

		}
	}
	*R /= cnt;
	*G /= cnt;
	*B /= cnt;
	
}

string Dec2Bi(float num) {
	string result;
	if (num < 0)
		result.append("1");
	else
		result.append("0");

	
}
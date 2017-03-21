#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include "MyBMP.h"

using namespace std;

void calcMean(BMP, float*, float*, float*);
void calcVariance(BMP, float, float, float, float*, float*, float*);
float colorTransfer(float, float, float, float, float);

int main(int argc, char** argv)
{
	float SRavg = 0.0, SGavg = 0.0, SBavg = 0.0;
	float SRsd = 0.0, SGsd = 0.0, SBsd = 0.0;
	BMP Img;

	Img.ReadFromFile("Lena64x64.bmp");  //讀取的圖檔名字

	calcMean(Img, &SRavg, &SGavg, &SBavg);
	calcVariance(Img, SRavg, SGavg, SBavg, &SRsd, &SGsd, &SBsd);

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

float colorTransfer(float Savg, float Ssd, float Tavg, float Tsd, float S) {
	return (Tsd / Ssd)*(S - Savg) + Tavg;
}
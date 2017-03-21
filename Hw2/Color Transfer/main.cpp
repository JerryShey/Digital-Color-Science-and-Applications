#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include "MyBMP.h"

using namespace std;

void calcMean(BMP, float*, float*, float*);
void calcSD(BMP, float, float, float, float*, float*, float*);
float colorTransfer(float, float, float, float, float);

int main(int argc, char** argv)
{
	float SRavg = 0.0, SGavg = 0.0, SBavg = 0.0;
	float TRavg = 0.0, TGavg = 0.0, TBavg = 0.0;
	float SRsd = 0.0, SGsd = 0.0, SBsd = 0.0;
	float TRsd = 0.0, TGsd = 0.0, TBsd = 0.0;
	BMP sourceImg, targetImg;
	BMP Output;
	Output.SetSize(sourceImg.TellWidth() , sourceImg.TellHeight());
	Output.SetBitDepth(24);

	sourceImg.ReadFromFile("Lena64x64.bmp");  //讀取的圖檔名字

	calcMean(sourceImg, &SRavg, &SGavg, &SBavg);
	calcMean(targetImg, &TRavg, &TGavg, &TBavg);
	calcSD(sourceImg, SRavg, SGavg, SBavg, &SRsd, &SGsd, &SBsd);
	calcSD(sourceImg, SRavg, SGavg, SBavg, &SRsd, &SGsd, &SBsd);
	for (int i = 0; i<sourceImg.TellHeight(); i++)
	{
		for (int j = 0; j<sourceImg.TellWidth(); j++)
		{
			RGBApixel NewPixel = sourceImg.GetPixel(i, j);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 

			//----------------------------在這部分做像素的修改--------------------------------//
			NewPixel.Red = colorTransfer(SRavg, SRsd, TRavg, TRsd, NewPixel.Red);
			NewPixel.Green = colorTransfer(SGavg, SGsd, TGavg, TGsd, NewPixel.Green);
			NewPixel.Blue = colorTransfer(SBavg, SBsd, TBavg, TBsd, NewPixel.Blue);
			//----------------------------修改影像結束--------------------------------//

			Output.SetPixel(i, j, NewPixel);  //儲存單一個像素結構
		}
	}
	Output.WriteToFile("");  //儲存的圖檔名字
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

void calcSD(BMP img, float avgR, float avgG, float avgB, float* R, float* G, float* B) {
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
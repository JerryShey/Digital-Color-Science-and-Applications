#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include "MyBMP.h"
#include <math.h>

using namespace std;

void calcMean(BMP, float*, float*, float*);
void calcSD(BMP, float, float, float, float*, float*, float*);
float colorTransfer(float, float, float, float, float);
string imgName[6] = {"01.bmp", "02.bmp", "03.bmp", "04.bmp", "05.bmp", "06.bmp"};
int main(int argc, char** argv)
{
	float SRavg = 0.0, SGavg = 0.0, SBavg = 0.0;
	float TRavg = 0.0, TGavg = 0.0, TBavg = 0.0;
	float SRsd = 0.0, SGsd = 0.0, SBsd = 0.0;
	float TRsd = 0.0, TGsd = 0.0, TBsd = 0.0;
	BMP sourceImg, targetImg;
	BMP Output;

	for (int x = 0; x < 6; x++) {
		cout << x+1 << ".\n";

		string path = "..\\Color Transfer\\Source Img\\" + imgName[x];
		sourceImg.ReadFromFile(path.c_str());  //讀取的圖檔名字
		path = "..\\Color Transfer\\Target Img\\" + imgName[x];
		targetImg.ReadFromFile(path.c_str());
		Output.SetSize(sourceImg.TellWidth(), sourceImg.TellHeight());
		Output.SetBitDepth(24);

		calcMean(sourceImg, &SRavg, &SGavg, &SBavg);
		calcMean(targetImg, &TRavg, &TGavg, &TBavg);
		calcSD(sourceImg, SRavg, SGavg, SBavg, &SRsd, &SGsd, &SBsd);
		calcSD(targetImg, TRavg, TGavg, TBavg, &TRsd, &TGsd, &TBsd);

		for (int i = 0; i < sourceImg.TellHeight(); i++)
		{
			for (int j = 0; j < sourceImg.TellWidth(); j++)
			{
				RGBApixel NewPixel = sourceImg.GetPixel(j, i);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 

				//----------------------------在這部分做像素的修改--------------------------------//
				NewPixel.Red = colorTransfer(SRavg, SRsd, TRavg, TRsd, NewPixel.Red);
				NewPixel.Green = colorTransfer(SGavg, SGsd, TGavg, TGsd, NewPixel.Green);
				NewPixel.Blue = colorTransfer(SBavg, SBsd, TBavg, TBsd, NewPixel.Blue);
				//----------------------------修改影像結束--------------------------------//

				Output.SetPixel(j, i, NewPixel);  //儲存單一個像素結構
			}
		}
		cout << "Source:\n\t" << SRavg << "\t" << SGavg << "\t" << SBavg;
		cout << "\n\t" << SRsd << "\t" << SGsd << "\t" << SBsd;
		cout << "\nTarget:\n\t" << TRavg << "\t" << TGavg << "\t" << TBavg;
		cout << "\n\t" << TRsd << "\t" << TGsd << "\t" << TBsd << endl;
		
		path = "..\\Color Transfer\\Result Img\\" + imgName[x];
		Output.WriteToFile(path.c_str());  //儲存的圖檔名字
	}
	cout << "請輸入任意鍵結束：";
	getchar();
	return 0;
}

void calcMean(BMP img, float* R, float* G, float* B) {
	int cnt = 0;
	for (int i = 0; i < img.TellHeight(); i++)
	{
		for (int j = 0; j < img.TellWidth(); j++)
		{
			RGBApixel NewPixel = img.GetPixel(j, i);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 

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
			RGBApixel NewPixel = img.GetPixel(j, i);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 
			tempR = 0.0;	tempG = 0.0;	tempB = 0.0;

			tempR = NewPixel.Red - avgR;
			*R +=tempR*tempR;
			tempG = NewPixel.Green - avgG;
			*G += tempG*tempG;
			tempB = NewPixel.Blue - avgB;
			*B += tempB*tempB;

			cnt++;

		}
	}
	*R /= cnt;
	*R = sqrt(*R);
	*G /= cnt;
	*G = sqrt(*G);
	*B /= cnt;
	*B = sqrt(*B);

}

float colorTransfer(float Savg, float Ssd, float Tavg, float Tsd, float S) {
	float result = S - Savg;
	float r = Tsd / Ssd;
	result *= r;
	result += Tavg;
	return result;
}
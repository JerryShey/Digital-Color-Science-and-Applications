#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include <math.h>
#include <bitset>
#include <iomanip>
#include "MyBMP.h"


using namespace std;

//void Image_color(BMP Img_In, const char *OutputName);
void Image_color(BMP Img_In);
void calcMean(BMP, float*, float*, float*);
void calcSD(BMP, float, float, float, float*, float*, float*);
string Dec2Bi(float);

int main(int argc, char** argv)
{
	float avgR = 0.0, avgG = 0.0, avgB = 0.0;
	float sdR = 0.0, sdG = 0.0, sdB = 0.0;
	fstream fp;
	BMP Img;

	Img.ReadFromFile("Lena64x64.bmp");  //讀取的圖檔名字
	fp.open("Lena64x64.txt", ios::out);

	calcMean(Img, &avgR, &avgG, &avgB);
	calcSD(Img, avgR, avgG, avgB, &sdR, &sdG, &sdB);
	
	fp << fixed << setprecision(6) << avgR << endl
		<< Dec2Bi(avgR) << endl
		<< avgG << endl
		<< Dec2Bi(avgG) << endl
		<< avgB << endl
		<< Dec2Bi(avgB) << endl;
	fp << sdR << endl
		<< Dec2Bi(sdR) << endl
		<< sdG << endl
		<< Dec2Bi(sdG) << endl
		<< sdB << endl
		<< Dec2Bi(sdB) << endl;

	fp.close();
	
	cout << "請輸入任意鍵結束";
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
	*R = sqrt(*R);
	*G = sqrt(*G);
	*B = sqrt(*B);
}

string Dec2Bi(float num) {
	string result;
	if (num < 0)
		result.append("1");
	else
		result.append("0");
	float x = 1;
	while (x < num)
		x*=2;
	std::bitset<sizeof(float)*CHAR_BIT> foo(*reinterpret_cast<unsigned long*>(&num));
	return foo.to_string();
	
}
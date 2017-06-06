#include <iostream>
#include <cstdlib>
#include <string> 
#include <fstream>
#include <sstream>
#include "MyBMP.h"
#include <math.h>
/*
std::cout << "[" << *G;
for(int j = 0; j < n; j++)
std::cout << ", " <<*(G+j);
std::cout << "]";
*/
void calcMean(BMP, float*, float*, float*);
void calcSD(BMP, float, float, float, float*, float*, float*);
float colorTransfer(float, float, float, float, float);
float calcPSNR(BMP, BMP);
void EMD_conceal(int, int, int*);
void EMD_decode(int n, int* G, int* result);

int main(int argc, char** argv)
{
	int n = 0, secretNum = 0;
	BMP testImg, outputImg;
	std::fstream file;

	testImg.ReadFromFile("test.bmp");
	outputImg.SetSize(testImg.TellWidth(), testImg.TellHeight());
	outputImg.SetBitDepth(24);

	std::cout << "請輸入n(" << "2~" << testImg.TellWidth()*testImg.TellHeight() << "):";
	std::cin >> n;
	std::cout << "\n請輸入數量(" << 1 << "~" << testImg.TellWidth()*testImg.TellHeight() / n << "):";
	std::cin >> secretNum;

	while (secretNum < 0 || secretNum > testImg.TellWidth()*testImg.TellHeight() / n){
		std::cout << "\n請輸入數量(" << 1 << "~" << testImg.TellWidth()*testImg.TellHeight() / n << "):";
		std::cin >> secretNum;
	}


	int *secret = new int[secretNum];
	int *nonsecret = new int[secretNum];
	int cnt = 0;

	int size = testImg.TellWidth()*testImg.TellHeight() * 3 + 1;
	int *test = new int[size];

	srand(100);

	for (int i = 0; i < secretNum; i++)
		secret[i] = (int)((2 * n)*(double)rand() / RAND_MAX);

	for (int i = 0; i < testImg.TellWidth(); i++)
	{
		for (int j = 0; j < testImg.TellHeight(); j++)
		{
			RGBApixel NewPixel = testImg.GetPixel(i, j);

			*(test + (cnt++)) = NewPixel.Red;
			*(test + (cnt++)) = NewPixel.Green;
			*(test + (cnt++)) = NewPixel.Blue;
		}
	}
	file.open("secret.txt");
	for (int i = 0; i < secretNum; i++){

		EMD_conceal(n, secret[i], test + i*n);
		file << secret[i] << std::endl;

	}
	file.close();

	cnt = 0;
	for (int i = 0; i < testImg.TellWidth(); i++)
	{
		for (int j = 0; j < testImg.TellHeight(); j++)
		{
			RGBApixel NewPixel;

			NewPixel.Red = *(test + (cnt++));
			NewPixel.Green = *(test + (cnt++));
			NewPixel.Blue = *(test + (cnt++));

			outputImg.SetPixel(i, j, NewPixel);
		}
	}

	file.open("decode.txt");
	for (int i = 0; i < secretNum; i++){
		EMD_decode(n, test + i*n, nonsecret + i);
		file << nonsecret[i] << std::endl;
	}
	file.close();

	delete test;
	outputImg.WriteToFile("outputImg.bmp");

	std::cout << calcPSNR(outputImg, testImg);

	
	std::cout << "請輸入任意鍵結束" << std::endl;
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
			*R += tempR*tempR;
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
	float result = S - Tavg;
	float r = Ssd / Tsd;
	result *= r;
	result += Savg;
	return result;
}

float calcPSNR(BMP Simg, BMP RSimg){
	float result = 0.0;
	int temp = 0;
	for (int i = 0; i < Simg.TellHeight(); i++){
		for (int j = 0; j < Simg.TellWidth(); j++){
			RGBApixel SPixel = Simg.GetPixel(j, i);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 
			RGBApixel RSPixel = RSimg.GetPixel(j, i);

			//----------------------------在這部分做像素的修改--------------------------------//
			temp = SPixel.Red - RSPixel.Red;
			result += temp * temp;
			temp = SPixel.Green - RSPixel.Green;
			result += temp * temp;
			temp = SPixel.Blue - RSPixel.Blue;
			result += temp * temp;
			//----------------------------修改影像結束--------------------------------//

		}
	}
	result = result / (float)Simg.TellHeight() / (float)Simg.TellWidth() / 3.0;
	result = 10 * log10(255 * 255 / result);
	return result;
}

void EMD_conceal(int n, int d, int* G){

	int f = 0;
	for (int i = 0; i < n; i++){
		f += *(G + i) * (i + 1);
	}
	f = f % (2 * n + 1);
	int s = (d - f) % (2 * n + 1);
	if (s < 0)
		s += (2 * n + 1);

	if (s == 0)
		return;
	else if (s <= n){
		*(G + (s - 1)) += 1;
		if (*(G + (s - 1))>255){
			*(G + (s - 1)) = 254;
			EMD_conceal(n, d, G);
		}

		else{
			return;
		}
	}
	else{
		int index = 2 * n + 1 - s;
		*(G + (index - 1)) -= 1;
		if (*(G + (index - 1)) < 0){
			*(G + (index - 1)) = 1;
			EMD_conceal(n, d, G);
		}
		else{
			return;
		}
	}
}

void EMD_decode(int n, int* G, int* result){
	int f = 0;
	for (int i = 0; i < n; i++){
		f += *(G + i) * (i + 1);
	}
	*(result) = f % (2 * n + 1);
}
#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include "MyBMP.h"

using namespace std;

//void Image_color(BMP Img_In, const char *OutputName);
void Image_color(BMP Img_In);
void writefile(fstream, int, RGBApixel);

int main(int argc, char** argv)
{

	BMP Img;
	Img.ReadFromFile("Lena64x64.bmp");  //Ū�������ɦW�r

	Image_color(Img);

	getchar();
	return 0;
}

//void Image_color(BMP Img_In, const char *OutputName)
void Image_color(BMP Img_In)
{
	printf("<Image_color>\n");
	//BMP Output;
	//Output.SetSize(Img_In.TellWidth() ,Img_In.TellHeight());
	//Output.SetBitDepth(24);

	int cnt = 0;
	fstream file;
	file.open("Lena64x64.csv");
	file << "pixel" << "," << "Red" << "," << "Green" << "," << "Blue" << endl;
	for (int i = 0; i<Img_In.TellHeight(); i++)
	{
		for (int j = 0; j<Img_In.TellWidth(); j++)
		{
			RGBApixel NewPixel = Img_In.GetPixel(i, j);  //Ū����@�ӹ������c�CP.S.�]�i�H�ۦ�令��Ū����(R,G,B)�}�C��A�A�����ΡC 

														 //----------------------------�b�o�������������ק�--------------------------------//
			cout << cnt << "\t" << (int)NewPixel.Red << "\t" << (int)NewPixel.Green << "\t" << (int)NewPixel.Blue << endl;
			file << cnt << "," << (int)NewPixel.Red << "," << (int)NewPixel.Green << "," << (int)NewPixel.Blue << endl;
			cnt++;
			//----------------------------�ק�v������--------------------------------//

			//Output.SetPixel(i, j, NewPixel);  //�x�s��@�ӹ������c
		}
	}
	file.close();
	//Output.WriteToFile(OutputName);  //�x�s�����ɦW�r
}

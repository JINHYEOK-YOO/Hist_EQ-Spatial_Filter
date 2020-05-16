//
// HW #2-1
// 2-1.cpp
//
// Created by 2014707040 유진혁 Yoo Jinhyeok
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int HEIGHT = 256;	// Input image height
const int WIDTH = 256;		// Input image width 
const unsigned int PIXEL = 256;		// Pixel value range (8bit)

double histogram[PIXEL] = { 0 };

// Transfer function
unsigned char Transfer(unsigned char r)
{
	// Integrate the histogram from 0 to r.
	double sum = 0.0;
	for (int i = 0; i <= r; i++)
		sum += histogram[i];
	return (unsigned char)((PIXEL - 1)*sum);	// s = T(r)
}

// Histogram Equalization
string HistogramEqualize(string inputFilename, string outputFilename = "output.raw")
{
	ifstream fin;
	ofstream fout;
	unsigned char input[HEIGHT][WIDTH] = { 0 };
	unsigned char output[HEIGHT][WIDTH] = { 0 };

	// Read a image and produce a histogram.
	fin.open(inputFilename, ios::binary);
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
		{
			input[i][j] = fin.get();
			histogram[input[i][j]]++;
		}
	fin.close();

	// Histogram normalize
	for (int i = 0; i < PIXEL; i++)
		histogram[i] /= (WIDTH * HEIGHT);

	// Write a image.
	fout.open(outputFilename, ios::binary);
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
		{
			// Assign new pixel values.
			output[i][j] = Transfer(input[i][j]);
			fout << output[i][j];
		}
	fout.close();

	return outputFilename;
}

int main()
{
	cout << "input.raw에 Histogram Equalization을 적용합니다." << endl;
	cout << "..." << endl;
	cout << "결과 영상이 " << HistogramEqualize("input.raw", "histogram_equalization.raw") << "로 저장되었습니다." << endl;
	return 0;
}
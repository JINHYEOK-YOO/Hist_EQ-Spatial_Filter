//
// HW #2-2
// 2-2.cpp
//
// Created by 2014707040 유진혁 Yoo Jinhyeok
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const int HEIGHT = 256;	// Input image height
const int WIDTH = 256;		// Input image width 

unsigned char input[HEIGHT][WIDTH] = { 0 };
unsigned char output[HEIGHT][WIDTH] = { 0 };

// Read a image.
void ImageLoad(string inputFilename)
{
	ifstream fin;
	fin.open(inputFilename, ios::binary);
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
		input[i][j] = fin.get();
	fin.close();
}

// Write a image.
void ImageSave(string outputFilename)
{
	ofstream fout;
	fout.open(outputFilename, ios::binary);
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			fout << output[i][j];
	fout.close();
}

// Apply filter1 to the image.
void AverageFilter1()
{
	memset(output, 0, WIDTH * HEIGHT);	// for initializing the output array
	double filter1[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			filter1[i][j] = 1.0 / 9.0;

	// Spatial filtering
	for (int i = 1; i < HEIGHT - 1; i++)
		for (int j = 1; j < WIDTH - 1; j++)
		{
			double sum = 0.0;
			for (int s = -1; s <= 1; s++)
				for (int t = -1; t <= 1; t++)
					sum += (filter1[s + 1][t + 1] * input[i + s][j + t]);
			output[i][j] = (unsigned char)sum;
		}
}

// Apply filter2 to the image.
void AverageFilter2()
{
	memset(output, 0, WIDTH * HEIGHT);	// for initializing the output array
	double filter2[7][7];
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			filter2[i][j] = 1.0 / 49.0;

	// Spatial filtering
	for (int i = 3; i < HEIGHT - 3; i++)
		for (int j = 3; j < WIDTH - 3; j++)
		{
			double sum = 0.0;
			for (int s = -3; s <= 3; s++)
				for (int t = -3; t <= 3; t++)
					sum += (filter2[s + 3][t + 3] * input[i + s][j + t]);
			output[i][j] = (unsigned char)sum;
		}
}

// Apply filter3 to the image.
void SmoothFilter()
{
	memset(output, 0, WIDTH * HEIGHT);	// for initializing the output array
	double filter3[3][3] = { {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
							 {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
							 {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0} };

	// Spatial filtering
	for (int i = 1; i < HEIGHT - 1; i++)
		for (int j = 1; j < WIDTH - 1; j++)
		{
			double sum = 0.0;
			for (int s = -1; s <= 1; s++)
				for (int t = -1; t <= 1; t++)
					sum += (filter3[s + 1][t + 1] * input[i + s][j + t]);
			output[i][j] = (unsigned char)sum;
		}
}

// Apply filter4 to the image result from filter1.
void SharpeningFilter()
{
	memset(output, 0, WIDTH * HEIGHT);	// for initializing the output array
	int filter4[3][3] = { {0, -1, 0},
						  {-1, 5, -1},
						  {0, -1, 0} };

	// Spatial filtering
	for (int i = 2; i < HEIGHT - 2; i++)
		for (int j = 2; j < WIDTH - 2; j++)
		{
			double sum = 0.0;
			for (int s = -1; s <= 1; s++)
				for (int t = -1; t <= 1; t++)
					sum += (filter4[s + 1][t + 1] * input[i + s][j + t]);
			output[i][j] = (unsigned char)sum;
		}
}

// Apply median filter to the images.
void MedianFilter(size_t maskSize)
{
	memset(output, 0, WIDTH * HEIGHT);		// for initializing the output array
	size_t size = maskSize * maskSize;		// total size of filter
	int startPoint = maskSize / 2;			// image filtering start point for input images
	vector<unsigned char> filterMask(size);	// median filter

	// Spatial filtering
	for (int i = startPoint; i < HEIGHT - startPoint; i++)
		for (int j = startPoint; j < WIDTH - startPoint; j++)
		{
			for (int s = -startPoint; s <= startPoint; s++)
				for (int t = -startPoint; t <= startPoint; t++)
					filterMask[(s + startPoint)*maskSize + (t + startPoint)] = input[i + s][j + t];
			sort(filterMask.begin(), filterMask.end());	// sorting
			output[i][j] = filterMask[size / 2];
		}
}

int main()
{
	int num;
	cout << "영상에 Spatial Filter를 적용합니다." << endl;
	cout << "(1) Average Filter" << endl;
	cout << "(2) Smooth Filter" << endl;
	cout << "(3) Sharpening Filter" << endl;
	cout << "(4) Median Filter" << endl;
	cout << ">> ";

	cin >> num;
	cout << endl;

	switch (num)
	{
	case 1:
		cout << "lena256.raw에 Filter1, Filter2를 적용합니다." << endl;
		cout << "..." << endl;

		ImageLoad("lena256.raw");

		AverageFilter1();
		ImageSave("average_filter_1.raw");

		AverageFilter2();
		ImageSave("average_filter_2.raw");

		cout << "결과 영상이 average_filter_1.raw, average_filter_2.raw로 저장되었습니다." << endl;
		break;
	case 2:
		cout << "lena256.raw에 Filter3을 적용합니다." << endl;
		cout << "..." << endl;

		ImageLoad("lena256.raw");
		SmoothFilter();
		ImageSave("smooth_filter.raw");

		cout << "결과 영상이 smooth_filter.raw로 저장되었습니다." << endl;
		break;
	case 3:
		cout << "Filter1의 결과에 Filter3을 적용합니다." << endl;
		cout << "..." << endl;

		ImageLoad("lena256.raw");
		AverageFilter1();
		// The filter1 result image is replaced with the input image.
		for (int i = 0; i < HEIGHT; i++)
			for (int j = 0; j < WIDTH; j++)
				input[i][j] = output[i][j];

		SharpeningFilter();
		ImageSave("sharpening_filter.raw");

		cout << "결과 영상이 sharpening_filter.raw로 저장되었습니다." << endl;
		break;
	case 4:
		size_t size;

		cout << "Filter Mask 크기 (n X n, n은 홀수)" << endl;
		cout << "n = ";

		cin >> size;
		cout << endl;

		// only odd number
		if (size % 2 == 0)
		{
			cout << "입력 오류" << endl;
			break;
		}
		else
		{
			cout << "노이즈가 포함된 lena256_n5.raw, lena256_n10.raw, lena256_n25.raw에 Median Filter를 적용합니다." << endl;
			cout << "..." << endl;

			ImageLoad("lena256_n5.raw");
			MedianFilter(size);
			ImageSave("median_filter_n5.raw");

			ImageLoad("lena256_n10.raw");
			MedianFilter(size);
			ImageSave("median_filter_n10.raw");

			ImageLoad("lena256_n25.raw");
			MedianFilter(size);
			ImageSave("median_filter_n25.raw");

			cout << "결과 영상이 median_filter_n5.raw, median_filter_n10.raw, median_filter_n25.raw로 저장되었습니다." << endl;
			break;
		}
	default:
		break;
	}

	return 0;
}
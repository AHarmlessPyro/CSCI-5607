#include "pch.h"
#include <iostream>
#include <math.h>
#include <tuple>
#include <fstream>
#include "ConsoleApplication1.h"
#include <string>
#include <stdexcept>

using namespace std;

// pi defined here as getting import errors otherwise from math.h
#define M_PI       3.14159265358979323846

// Define colors
int const COLORS = 255;

// Code for HSV to RGB adapted from https://stackoverflow.com/a/6930407
// and https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB

std::tuple<int, int, int> HSVtoRGB(std::tuple<float, float, float> HSV) {
	long        i;
	// Standard C,X,m values as defined on wiki algorithm
	double C, X, m;

	// C++ 17 feature, just trying out different versions of C++
	// is more akin to script like languages that I'm more used to
	// (say python or javascript)
#if __cplusplus == 201703L
	auto[hue, saturation, value] = HSV;
#else
	// If anything above c++ 11 defined, use std::tie
	double hue;
	float saturation;
	float value;

	std::tie(hue, saturation, value) = HSV;
#endif
	// If by chance have hue above 360, shift it back to within 0 to 360.
	hue = fmod(hue, 360.0);

	C = value * saturation;
	X = C * (1 - abs(fmod(hue / 60.0, 2) - 1));
	m = value - C;

	// convert to int as we need an int for switch ... case
	i = ((int)hue) / 60;

	switch (i) {
	case 0:
		return std::make_tuple((C + m) * 255, (X + m) * 255, m * 255);
	case 1:
		return std::make_tuple((X + m) * 255, (C + m) * 255, m * 255);
	case 2:
		return std::make_tuple(m * 255, (C + m) * 255, (X + m) * 255);
	case 3:
		return std::make_tuple(m * 255, (X + m) * 255, (C + m) * 255);
	case 4:
		return std::make_tuple((X + m) * 255, m * 255, (C + m) * 255);
	case 5:
	default:
		return std::make_tuple((C + m) * 255, m * 255, (X + m) * 255);
	}
}



// Function to set color for inside and outside the circle expected.
// Due to laziness, circle centre is at X/2 and Y/2 as taken from input.txt.
// So everything needs to be shifted by X/2 and Y/2
void setColor(int posX, int posY, int centX, int centY, int rad, std::ofstream& file) {

	long distFromCent = std::sqrt(std::pow((posX - centX), 2) + std::pow((posY - centY), 2));

	if (distFromCent > rad) {
		// Set Black color for area outside the circle
		file << 0;
		file << " ";
		file << 0;
		file << " ";
		file << 0;
		file << std::endl;
	}
	else {
		// On items on the inside, create an RGB pattern

		// Actual angle made by point at centre to determine hue
		double hue = (180 / M_PI) * (std::atan2(posY - centY, posX - centX) + M_PI);

		// Actual distance of point from centre to determine saturation
		double saturation = (std::sqrt(std::pow((posX - centX), 2) + std::pow((posY - centY), 2))) / rad;

		// distActual set to not exceed 1. Value is fixed to 1. 
		auto HSVColor = std::make_tuple(hue, saturation > 1 ? 1 : saturation, 1.0);

		auto rgb = HSVtoRGB(HSVColor);

		file << (int)std::get<0>(rgb);
		file << " ";
		file << (int)std::get<1>(rgb);
		file << " ";
		file << (int)std::get<2>(rgb);
		file << std::endl;
	}
}


int main(int argc, char **argv)
{
	// temporary variables to add option for default values in case 
	// input file not available
	int assignmentX, assignmentY;
	// if input available, take second input value. Should have named
	// inputs, but I'm lazy
	if (argc > 1)
	{
		// try reading values from input file.
		try {
			ifstream fin(argv[1]);
			std::string imgSize;
			fin >> imgSize >> assignmentX >> assignmentY;
			// if file doesn't start with imsize, it isn't correct. Kill process.
			if (!imgSize.compare("imsize") == 0) {
				// throw error that will be caught and will exit program gracefully.
				throw std::invalid_argument("Line doesn't start with required parameter \'imsize\'");
			}
		}
		catch (const std::exception& e) {
			std::cout << "Format error while reading file. Correct format to \'imsize width height\'" << std::endl;
			return 0;
		}
	}
	// Set default values. Run at 999x999 pixels.
	else {
		std::cout << "Running program with default values" << std::endl;
		assignmentX = 999;
		assignmentY = 999;
	}
	// assign values for X and Y to constants that will be used henceforth
	int const X = assignmentX;
	int const Y = assignmentY;

	// Assuming image to be made would be a circle, so assign the centre.
	int const centre = (X + 1) / 2;

	//create output stream for writing netpbm file.
	ofstream fout;

	fout.open("Output.ppm");

	// Writing Headers
	fout << "P3" << std::endl;
	fout << X << std::endl;
	fout << Y << std::endl;
	fout << COLORS << std::endl;

	// Call function for assigning colors
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			setColor(i, j, centre, centre, centre, fout);
		}
	}

	// Close file as done writing
	fout.close();
	return 0;
}

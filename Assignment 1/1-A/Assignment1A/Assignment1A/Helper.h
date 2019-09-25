#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Vector3.h"

std::vector<std::string> tokenize(std::string input, char delemiter) {
	std::vector<std::string> accumulator = std::vector<std::string>();
	std::string tempAcc = std::string();
	for (char s : input) {
		if (s == delemiter) {
			// push to accumulator and then clear string
			accumulator.push_back(tempAcc);
			tempAcc = std::string();
		}
		else {
			tempAcc += s;
		}
	}
	accumulator.push_back(tempAcc);
	return accumulator;
}

void printPPMToFile(std::vector<std::vector<Vector3>> input, std::string fileName, int width, int height) {
	std::ofstream FILE;
	FILE.open(fileName);
	FILE << "P3" << std::endl;
	FILE << width << " " << height << std::endl;
	FILE << 255 << std::endl; // colors
	for (auto vec : input) {
		for (auto elem : vec) {
			FILE << elem << std::endl;
		}
		//FILE << std::get<0>(elem) << std::get<1>(elem) << std::get<2>(elem) << std::endl;
	}
	FILE.close();
}
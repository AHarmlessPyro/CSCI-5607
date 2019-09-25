// Assignment1A.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"

#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>
#include "Vector3.h"
#include "Helper.h"
#include "Ray.h"
#include "Object3D.h"
#include "Sphere.h"
#include <limits>

using namespace std;

// define if we need debug statements or not
#define DEBUG 0
/* define maginification factor for fading away background depending on angle between
 view and ray*/
#define MAGFACTOR 100
 // Exceptions require strings of constant size. Define string size for that
const size_t EXCEPTIONSTRINGSIZE = 200;

// Maintain record of last used material here. 
int lastMat = INT_MIN;

// Misc required variables made global for ease.

Vector3 eye;
Vector3 up;
Vector3 viewDir;
float vfov;
std::tuple<int, int> imSize;
Vector3 bgColor;
vector<Vector3> mtlList;
// not currently assigning values to array. Will do that soon.
vector<vector<Vector3>> colArray;
vector<Object3D*> objList;

/*
Used to make sure that the required values above have been filled. Fails if the same
value is found in the input file again. That is not a usecase we have any directions on
how to resolve either, i.e. keep latest vs keep earliest issue on conflict.
*/
int CriticalValues = 0;

// Assign variables from tokenized string to global vars.
void fillVars(vector<std::string> tokenizedString) {
	// If first token var is eye
	if (tokenizedString[0] == "eye") {
		// if all 4 vars required aren't there, exit.
		if (tokenizedString.size() < 4) {
			std::cout << "Wrong number of arguments supplied for EYE" << std::endl;
			exit(EXIT_FAILURE);
		}
		eye = Vector3(
			stof(tokenizedString[1]),
			stof(tokenizedString[2]),
			stof(tokenizedString[3])
		);
		// is an important value, so increment criticalvalues
		CriticalValues++;
	}
	// If first token var is updir
	else if (tokenizedString[0] == "updir") {
		// if all 4 vars required aren't there, exit.
		if (tokenizedString.size() < 4) {
			std::cout << "Wrong number of arguments supplied for UPDIR" << std::endl;
			exit(EXIT_FAILURE);
		}
		up = Vector3(
			stof(tokenizedString[1]),
			stof(tokenizedString[2]),
			stof(tokenizedString[3])
		);
		// is an important value, so increment criticalvalues
		CriticalValues++;
	}
	// If first token var is viewdir
	else if (tokenizedString[0] == "viewdir") {
		// Need 4 values, exit otherwise
		if (tokenizedString.size() < 4) {
			std::cout << "Wrong number of arguments supplied for VIEWDIR" << std::endl;
			exit(EXIT_FAILURE);
		}
		viewDir = Vector3(
			stof(tokenizedString[1]),
			stof(tokenizedString[2]),
			stof(tokenizedString[3])
		);
		// is an important value, so increment criticalvalues
		CriticalValues++;
	}
	// If first token var is vfov
	else if (tokenizedString[0] == "vfov") {
		// Need 2 values, exit otherwise
		if (tokenizedString.size() < 2) {
			std::cout << "Wrong number of arguments supplied for VFOV" << std::endl;
			exit(EXIT_FAILURE);
		}
		float fov = stof(tokenizedString[1]);
		// Make sure that fov is a valid value between 0 and 90
		if (fov < 0.0 && fov < 90.0) {
			const char* string = "Invalid FOV. %f is less than 0\0";
			char* temp = (char*)calloc(EXCEPTIONSTRINGSIZE, sizeof(char));
			int val = sprintf_s(temp, EXCEPTIONSTRINGSIZE, string, fov);
			//throw error vs exit? error might be better for future, but exit is easier rn
			//throw std::out_of_range(temp);
			exit(EXIT_FAILURE);
		}
		vfov = fov;
		// It is a critical value, so increment
		CriticalValues++;
	}
	else if (tokenizedString[0] == "imsize") {
		// imsize requires 3 values
		if (tokenizedString.size() < 3) {
			std::cout << "Wrong number of arguments supplied for IMSIZE" << std::endl;
			exit(EXIT_FAILURE);
		}

		int width = stoi(tokenizedString[1]);
		int height = stoi(tokenizedString[2]);
		// make sure that scanned values aren't negative
		if (width < 0 || height < 0) {
			const char* string = "Image size incorrect. %d or %d is less than 0\0";
			char* temp = (char*)calloc(EXCEPTIONSTRINGSIZE, sizeof(char));
			int val = sprintf_s(temp, EXCEPTIONSTRINGSIZE, string, width, height);
			// throw vs exit issue.
			//throw std::out_of_range(temp);
			exit(EXIT_FAILURE);
		}
		imSize = std::make_tuple(width, height);
		// Create array/vector of colors
		colArray = vector<vector<Vector3>>(height);
		for (unsigned int i = 0; i < colArray.size(); i++) {
			colArray[i] = vector<Vector3>(width);
		}
		// image size is a critical value
		CriticalValues++;
	}
	else if (tokenizedString[0] == "bkgcolor") {
		// background color needs 4 values
		if (tokenizedString.size() < 4) {
			std::cout << "Wrong number of arguments supplied for BKGCOLOR" << std::endl;
			exit(EXIT_FAILURE);
		}

		float red = stof(tokenizedString[1]);
		float green = stof(tokenizedString[2]);
		float blue = stof(tokenizedString[3]);
		// RGB values need to be between 0.0 and 1.0
		if ((red < 0 || red >1.0) || (green < 0 || green >1.0) || (blue < 0 || blue >1.0)) {
			const char* string = "RGB values out of range for background. Value must be between 0.0 and 1.0 . %f or %f or %f is less than 0\0";
			char* temp = (char*)calloc(EXCEPTIONSTRINGSIZE, sizeof(char));
			int val = sprintf_s(temp, EXCEPTIONSTRINGSIZE, string, red, green, blue);
			// throw vs exit issue
			// throw std::out_of_range(temp);
			exit(EXIT_FAILURE);
		}
		bgColor = Vector3(red, green, blue);
		// background color is critical
		CriticalValues++;
	}
	else if (tokenizedString[0] == "mtlcolor") {
		// material color has 4 values.
		if (tokenizedString.size() < 4) {
			std::cout << "Wrong number of arguments supplied for MTLCOLOR" << std::endl;
			exit(EXIT_FAILURE);
		}

		float red = stof(tokenizedString[1]);
		float green = stof(tokenizedString[2]);
		float blue = stof(tokenizedString[3]);

		if ((red < 0 || red >1.0) || (green < 0 || green >1.0) || (blue < 0 || blue >1.0)) {
			const char* string = "RGB values out of range for material. Value must be between 0.0 and 1.0 . %f or %f or %f is less than 0\0";
			char* temp = (char*)calloc(EXCEPTIONSTRINGSIZE, sizeof(char));
			int val = sprintf_s(temp, EXCEPTIONSTRINGSIZE, string, red, green, blue);
			// throw vs exit issue
			// throw std::out_of_range(temp);
			exit(EXIT_FAILURE);
		}

		mtlList.push_back(Vector3(red, green, blue));
		lastMat = mtlList.size() - 1;

		// not a critical value.
	}
	else if (tokenizedString[0] == "sphere") {
		// make sure that there is a material
		if (lastMat == INT_MIN) {
			std::cout << "No material defined for sphere.Material should exist before sphere" << std::endl;
			exit(EXIT_FAILURE);
		}
		// spheres need 5 variables (name, x,y,z, rad)
		if (tokenizedString.size() < 5) {
			std::cout << "Wrong number of arguments supplied for SPHERE" << std::endl;
			exit(EXIT_FAILURE);
		}
		// sphere's aren't critical values.
		objList.push_back(
			new Sphere(
				Vector3(stof(tokenizedString[1]),
					stof(tokenizedString[2]),
					stof(tokenizedString[3])
				), stof(tokenizedString[4]),
				lastMat
			));
	}
	else {
		std::cout << "Ignoring unknown input with values ";
		for (auto elem : tokenizedString) {
			std::cout << "\t" << elem;
		}
		std::cout << std::endl;
	}
}
// colorise shot ray
Vector3 returnColor(Ray r, int i, int j) {
	// create object for storing info.
	Object3D::Hit* last = (Object3D::Hit*)calloc(1, sizeof(Object3D::Hit));
	// define farthest object as maxfloat
	last->tVal = std::numeric_limits<float>::max();
	// last material hit is set to -1, so that by chance we don't select a random
	// material
	int lastMtlHit = -1;
	// run ray intersection for all objects
	for (unsigned int i = 0; i < objList.size(); i++) {
		auto obj = objList[i];
		// perform collision
		Object3D::Hit* val = (Object3D::Hit*)calloc(1, sizeof(Object3D::Hit));
		// run collision if it is between t value 0 and last->tval
		// (or 0 and the last collision t-value)
		val = &(obj->hit(r, 0, last->tVal));
		// if collision happens, store info
		if (val->hit) {
			last->hit = true;
			last->tVal = val->tVal;
			lastMtlHit = objList[i]->Mat();
		}
	}
	// In observance, the angle deviation between viewdir and casted ray is 
	// between 0 and 2 degrees. This isn't visible as a color difference. So
	// that deviation is magnified by a pre-defined factor. This creates a fade
	// away effect on the background depending on angle between view and 
	// current ray
	float returnRayColor = MAGFACTOR * (angleBetween(viewDir, r.Direction())) / 180.0f;
	// inverse scaling according to the previously calculated value. This makes 
	// a fade outwards instead of fade inwards
	Vector3 col = ((1 - returnRayColor)*bgColor*255.99f).toInt();
#if DEBUG == 1
	std::cout << "Angle is " << extrenous << "\n and color is " << col << std::endl;
#endif
	// if there was a collision, return color of material else calculated background
	return last->hit ? (mtlList[lastMtlHit] * 255.99f).toInt() : col;
}

int main(char argc, char**argv)
{
	// if have 1 or more arguments, attempts 
	if (argc > 1) {
		// open file
		ifstream file;
		try {
			file.open(argv[1]);
		}
		// if it doesn't exist, exit with error
		catch (exception e) {
			std::cout << "WARNING : FILE NOT FOUND`" << std::endl;
			exit(EXIT_FAILURE);
		}
		// if File was opened, attempt to tokenize each line
		std::string read;
		std::vector<std::string> tokenized = std::vector<std::string>();
		while (std::getline(file, read)) {
			// tokenize the read in line
			tokenized = tokenize(read, ' ');
			try {
				// fill in the globally defined values.
				fillVars(tokenized);
			}
			catch (exception e) {
				std::cout << "Had an Error. Describing it now : \n" << e.what() << std::endl;
				break;
			}
		}
	}
	// if zero arguments supllied, exit
	else {
		std::cout << "Wrong number of arguments supplied, please add argument for input file" << std::endl;
		exit(EXIT_FAILURE);
	}

	// if critical values (see above) are not filled, exit
	if (CriticalValues < 6) {
		std::cout << "Critical values (constituted of eye,up view, vfov, imsize and bkgcolor) are not filled properly" << std::endl;
		exit(EXIT_FAILURE);
	}

	int width = std::get<0>(imSize);
	int height = std::get<1>(imSize);

	float aspect = (float)width / (float)height;
	float dist = (float)height / (float)(2.0f * tan((M_PI / 180.0f)*(vfov / 2)));

	// calculate u and v from view and up
	Vector3 u = cross(viewDir, up).normalize();
	Vector3 v = cross(u, viewDir).normalize();
	Vector3 windowCenter = eye + dist * viewDir.normalize();

	// calculate 4 corners of viewing window
	Vector3 UpL = windowCenter + height / 2 * v - width / 2 * u;
	Vector3 DnL = windowCenter - height / 2 * v - width / 2 * u;

	Vector3 UpR = windowCenter + height / 2 * v + width / 2 * u;
	Vector3 DnR = windowCenter - height / 2 * v + width / 2 * u;

	// calculate vertical and horizontal offset values	
	Vector3 offsetHorizontal_major = (UpR - UpL) / (width);
	Vector3 offsetVertical_major = (UpR - DnR) / (height);
	Vector3 offsetHorizontal_minor = offsetHorizontal_major / 2;
	Vector3 offsetVertical_minor = offsetVertical_major / 2;

#if DEBUG == 1
	// misc debug statments
	std::cout << windowCenter << std::endl;

	std::cout << "Dimensions are " << width << ":" << height << std::endl;

	std::cout << "U and V are " << u << "," << v << std::endl;
	std::cout << "Window center is at " << windowCenter << std::endl;

	std::cout << "View Direction is " << viewDir.normalize() << std::endl;

	std::cout << "Major Offset X " << offsetHorizontal_major << "\n";
	std::cout << "Major Offset Y " << offsetVertical_major << "\n";
	std::cout << "Minor Offset X " << offsetHorizontal_minor << "\n";
	std::cout << "Minor Offset Y " << offsetVertical_minor << std::endl;

	std::cout << "Upper Left is " << UpL << std::endl;
	std::cout << "Lower Left is " << DnL << std::endl;
	std::cout << "Upper Right is " << UpR << std::endl;
	std::cout << "Lower Right is " << DnR << std::endl;

	// primitive pause break for analysis purposes.
	int input;
	std::cin >> input;
#endif
	// find file name. Assume first supplied argument is the file name.
	// remove the file extension as required.
	std::string output = tokenize(std::string(argv[1]), '.')[0];
	output += ".ppm";

	// Create output
	//std::ofstream Output(output);

	//Output << "P3\n" << width << " " << height << " \n255" << std::endl;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Ray r = Ray(eye, (DnL + i * offsetHorizontal_major + j * offsetVertical_major + offsetHorizontal_minor + offsetVertical_minor));
#if DEBUG == 1
			std::cout << "For " << i << "," << j << ":" << r.Origin() << "->" << r.Direction().normalize() << std::endl;
#endif			
			//Output << returnColor(r, i, j) << std::endl;
			colArray[j][i] = returnColor(r, i, j);
		}
#if DEBUG == 1
		std::cout << "Done with " << j << std::endl;
#endif
		}
	printPPMToFile(colArray,output,width,height);
	//Output.close();
	}


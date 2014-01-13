#include<iostream>
#include<fstream>
using namespace std;
#include "Image.h"

Image::Image (const char * f) {
	ifstream g(f);
	if (!g.good()) {
		cerr << "Error: Couldn't read from " << f << endl;
	}
	else {
		string a;
		unsigned int i, j, k;
		g >> a >> i >> j >> k;
		g.get();
		width = i;
		height = j;
		range = k;
		data = new pixel *[height];
		for (unsigned int i = 0; i < height; ++i){
			data[i] = new pixel[width];
			g.read( (char *) data[i], width*sizeof(pixel) );
		}
	}
}

Image::Image() {
	width = 0;
	height = 0;
	range = 0;
}

Image::~Image () {
	for (unsigned int i = 0; i < height; ++i)
		delete [] data[i];
	delete [] data;
}

void Image::write (const char * f) {
	ofstream of(f);
	if (!of.good()) {
		cerr << "Error: Couldn't write to " << f << endl;
	}
	else {
		of << "P6" << endl;
		of << width << " " << height << endl;
		of << range << endl;
		for (unsigned int y = 0; y < height; ++y){
			of.write((char *)data[y], width*sizeof(pixel));
		}
	}
}

Image & Image::operator-(const Image & I) {
	if (width == I.width && height == I.height && range == I.range) {
		for (unsigned int i = 0; i < height; ++i) {
			for (unsigned int j = 0; j < width; ++j) {
				data[i][j].red -= I.data[i][j].red;
				data[i][j].green -= I.data[i][j].green;
				data[i][j].blue -= I.data[i][j].blue;
			}
		}
		return *this;
	}
	else {
		cerr << "Error: Image dimensions don't match." << endl;
		return *this;
	}
}
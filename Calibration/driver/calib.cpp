#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Image.h"

using namespace std;

bool captureImages = false;
bool cameraDetected = false;

int WIN_WIDTH = 700;
int WIN_HEIGHT = 700;
char programName[] = "Calibration Test";

int numSections = 2;
int hCount = 1;
int vCount = 1;

int segmentWidth;
int segmentHeight;

enum testState {Vertical, Horizontal};
testState orientation = Vertical;

bool whiteOnBlack = true;
bool firstTime = true;

vector<string> files;
string imageDirectory = "images/";

void init();

void exitAll() {
	int win = glutGetWindow();
	glutDestroyWindow(win);
	exit(0);
}

void processImages() {
	// for (int i = 0; i < (int)files.size(); ++i) {
		// cout << files[i] << endl;
	// }
	Image i(files[0].c_str());
	Image i2(files[1].c_str());
	Image i3 = i - i2;
	i3.write("newImage.ppm");
	cout << "DONE" << endl;
	exitAll();
}

void toggleWOB() {
	if (whiteOnBlack) 
	  whiteOnBlack = false;
	else
	  whiteOnBlack = true;
}

void updateCount() {
	if (!whiteOnBlack) {
		if (orientation == Vertical) {
			++vCount;
		}
		else { // orientation == Horizontal
			++hCount;
		}
		numSections *= 2;
	}
}

string getFileName() {
	stringstream ss;
	string s = imageDirectory;
	if (orientation == Vertical) {
		s += "v";
		ss << vCount;
		s += ss.str();
	}
	else {
		s += "h";
		ss << hCount;
		s += ss.str();
	}
	if (whiteOnBlack)
		s += "a";
	else
		s += "b";
	string temp = s;
	temp += ".ppm";
	files.push_back(temp);
	return s;
}

void syscall(string filename) {
	string command1, command2, command3;
	command1 = "gphoto2 --capture-image-and-download --force-overwrite --filename " + filename + ".jpg";
	command2 = "convert " + filename + ".jpg " + filename + ".ppm";
	command3 = "rm " + filename + ".jpg";
	system(command1.c_str());
	system(command2.c_str());
	system(command3.c_str());
}

void drawRectangle(int x, int y, int width, int height, int r, int g, int b) {
	glColor3f(r/255.0, g/255.0, b/255.0);
	glBegin(GL_QUADS);
	glVertex2d(x, y);
	glVertex2d(x, y + height);
	glVertex2d(x + width, y + height);
	glVertex2d(x + width, y);
	glEnd();
}

int nextPowOfTwo(int size) {
	int total;
	for (total = 1; total < size; total *= 2);
	return total;
}

void setSizes() {
	WIN_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
	WIN_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
}

void checkOrientationSwitch() {
	if (orientation == Vertical) {
		if (segmentWidth == 1 && whiteOnBlack) {
			orientation = Horizontal;
			numSections = 2;
		}
	}
	else { // orientation == Horizontal
		if (segmentHeight == 1 && whiteOnBlack) {
			processImages();
		}
	}
}

void drawWindow() {
 	glClear(GL_COLOR_BUFFER_BIT);
	init();
	segmentWidth = WIN_WIDTH;
	segmentHeight = WIN_HEIGHT;
	int x_pos, y_pos;
	if (orientation == Vertical) {
		segmentWidth = nextPowOfTwo(WIN_WIDTH);
		segmentWidth /= numSections;
		x_pos = 0;
		y_pos = 0;
		if (!whiteOnBlack)
			x_pos += segmentWidth;
		for (int i = 0; i < numSections/2; ++i) {
			drawRectangle(x_pos, y_pos, segmentWidth, segmentHeight, 255, 255, 255);
			x_pos += 2*segmentWidth;
		}
	}
	else { // orientation == Horizontal
		segmentHeight = nextPowOfTwo(WIN_HEIGHT);
		segmentHeight /= numSections;
		x_pos = 0;
		y_pos = 0;
		if (!whiteOnBlack)
			y_pos += segmentHeight;
		for (int i = 0; i < numSections/2; ++i) {
			drawRectangle(x_pos, y_pos, segmentWidth, segmentHeight, 255, 255, 255);
			y_pos += 2*segmentHeight;
		}
	}
	glutSwapBuffers();
}

void cameraCheck() {
	system("gphoto2 --summary > .cameraDetails");
	int fd = open(".cameraDetails", O_RDONLY);
	struct stat st;
	fstat(fd, &st);
	if (st.st_size != 417 && st.st_size != 0) { // size of error message if no camera or no gphoto2
		cameraDetected = true;
	}
	close(fd);
	system("rm .cameraDetails");
}

void calibrate() {  
	
	drawWindow();

	if (!firstTime) {
		string s = getFileName();
		if (captureImages) {
			syscall(s);
		}
		updateCount();
		toggleWOB();
		checkOrientationSwitch();
	}
	else {
		cameraCheck();
		firstTime = false;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char c, int x, int y) {
	switch(c) {
		case 'q':
		case 'Q':
			exitAll();
			break;
		case 'h':
			orientation = Horizontal;
			numSections = 2;
			break;
		case 'v':
			orientation = Vertical;
			numSections = 2;
			break;
		case 32: // space
			toggleWOB();
		default:
			break;
	}
	glutPostRedisplay();
}

void specialKeyboard(int c, int x, int y) {
	switch (c) {
		case GLUT_KEY_LEFT:
			if (numSections > 2) {
				numSections /= 2;
				// cout << "Number of sections: " << numSections << endl;
			}
			else {
				// cout << "Minimum number of divisions reached." << endl;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (orientation == Horizontal) {
				if (glutGet(GLUT_WINDOW_HEIGHT)/numSections >= 1) {
					numSections *= 2;
					// cout << "Number of sections: " << numSections << endl;
				}
				else {
					// cout << "Maximum number of divisions reached." << endl;
				}
			}
			else {
				if (glutGet(GLUT_WINDOW_WIDTH)/numSections >= 1) {
					numSections *= 2;
					// cout << "Number of sections: " << numSections << endl;
				}
				else {
					// cout << "Maximum number of divisions reached." << endl;
				}
			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void init(void) {
	setSizes();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., WIN_WIDTH, WIN_HEIGHT, 0., -1.0, 1.0);
}

void init_gl_window() {
	char * argv[] = { programName };
	int argc = sizeof(argv) / sizeof(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );

	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(programName);
	glutFullScreen();

	glutDisplayFunc(calibrate);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutPostRedisplay();

	glutMainLoop();
}

void argCheck(int argc, char ** argv) {
	if (argc == 2) {
		if (!strcmp(argv[1], "--capture")) {
			captureImages = true;
		}
		else if (!strcmp(argv[1], "--no-capture")) {

		}
		else {
			cerr << "Invalid argument: " << argv[1] << endl;
		}
	}
	else {
		cout << "Would you like to capture images? (Y/N) " << endl;
		string s;
		getline(cin, s);
		if (s[0] == 'Y' || s[0] == 'y') {
			captureImages = true;
		}
	}
	if (captureImages) {
		cameraCheck();
		if (cameraDetected) {
			cout << "Where would you like to store the images? Enter desired directory" << endl;
			getline(cin, imageDirectory);
			if (imageDirectory == "") {
				imageDirectory = "images/";
			}
			else if (imageDirectory[imageDirectory.size()-1] != '/') {
				imageDirectory += "/";
			}
		}
		else {
			cerr << "Error: No camera detected." << endl;
			exit(0);
		}
	}
	else {
		cout << "Where are the calibration images stored? Enter the directory" << endl;
		getline(cin, imageDirectory);
		if (imageDirectory == "") {
			imageDirectory = "images/";
		}
		else if (imageDirectory[imageDirectory.size()-1] != '/') {
			imageDirectory += "/";
		}
	}
}

int main(int argc, char ** argv) {

	argCheck(argc, argv);
	init_gl_window();

	return 0;
}

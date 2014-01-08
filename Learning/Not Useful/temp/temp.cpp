#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>

//g++ -I/usr/include -L/usr/lib -lglut -lGL -lGLU -lGLEW test.cpp -o example

/*
	int main(int argc, char **argv) {
		glutInit(&argc, argv);
		
		//...
		
		glewInit();
		if (glewIsSupported("GL_VERSION_2_0"))
			printf("Ready for OpenGL 2.0\n");
		else {
			printf("OpenGL 2.0 not supported\n");
			exit(1);
		}
	
		//setShaders();
	
		glutMainLoop();
		
	}*/



		int main(int argc, char **argv){
			glutInit(&argc, argv);
			glutCreateWindow("GLEW Test");
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
			  /* Problem: glewInit failed, something is seriously wrong. */
			  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			  
			}
			fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		}
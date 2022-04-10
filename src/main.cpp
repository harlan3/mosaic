/*
 * Mosaic (OpenGL Cube Surface Viewer)
 * Copyright (c) 2022 Harlan Murphy - Orbisoftware
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "BMP_File_Loader.h"

void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

GLuint texture[6];
GLfloat xRot, yRot, zRot;
GLfloat xDist, yDist, zDist;
bool mouseLeftDown;
bool mouseRightDown;
GLfloat mouseX, mouseY;
bool fullScreen;

void loadGLTextures() {

	const char *bmpFile[6] = { "ref_images/face1.bmp",
			"ref_images/face2.bmp", "ref_images/face3.bmp", "ref_images/face4.bmp",
			"ref_images/face5.bmp", "ref_images/face6.bmp" };

	for (int i = 0; i < 6; ++i) {

		BMP_File_Loader bmpFileLoader;
		unsigned char *bytes;
		unsigned int imageWidth, imageHeight;
		bmpFileLoader.LoadImage(bmpFile[i], bytes, imageWidth, imageHeight);

		// texture format based on # of bits per pixel
		GLint format = GL_RGB;
		GLint internalFormat = GL_RGB8;

		// copy the texture to OpenGL
		glGenTextures(1, &texture[i]);

		// set active texture and configure it
		glBindTexture(GL_TEXTURE_2D, texture[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// copy bitmap data to texture object
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageWidth, imageHeight,
				0, format, GL_UNSIGNED_BYTE, bytes);

		free(bytes);

		// unbind
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}

double calcPanVelocity(double z) {

	return (0.00005 * z) + 0.0002;
}

int init() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	loadGLTextures();

	return 1;
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(xDist, 0.0f, 0.0f);
	glTranslatef(0.0f, yDist, -0.0f);
	glTranslatef(0.0f, 0.0f, -zDist);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);

	// Front Face (face 1)
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);

	// Back Face (face 2)
	glTexCoord2d(1.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);

	// Top Face (face 3)
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);

	// Bottom Face (face 4)
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);

	// Right face (face 5)
	glTexCoord2d(1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);

	// Left Face (face 6)
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glutSwapBuffers();
}

void reshape(int w, int h) {

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, (GLfloat) w / (GLfloat) h, .01, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		xRot = 0;
		yRot = 0;
		zRot = 0;
		xDist = 0;
		yDist = 0;
		zDist = 0;
		glutPostRedisplay();
		break;
	case '2':
		xRot = 0;
		yRot = 180;
		zRot = 0;
		xDist = 0;
		yDist = 0;
		zDist = 0;
		glutPostRedisplay();
		break;
	case '3':
		xRot = 90;
		yRot = 0;
		zRot = 0;
		xDist = 0;
		yDist = 0;
		zDist = 0;
		glutPostRedisplay();
		break;
	case '4':
		xRot = -90;
		yRot = 0;
		zRot = 0;
		xDist = 0;
		yDist = 0;
		zDist = 0;
		glutPostRedisplay();
		break;
	case '5':
		xRot = 0;
		yRot = -90;
		zRot = 0;
		xDist = 0;
		yDist = 0;
		zDist = 0;
		glutPostRedisplay();
		break;
	case '6':
		xRot = 0;
		yRot = 90;
		zRot = 0;
		xDist = 0;
		yDist = 0;
		zDist = 0;
		glutPostRedisplay();
		break;
	case 'f':
		if (!fullScreen) {
			glutFullScreen();
			fullScreen = true;
		} else if (fullScreen) {
			glutReshapeWindow(800, 600);
			glutPositionWindow(0, 0);
			fullScreen = false;
		}
		break;
	case 'X':
		xRot -= 1.0f;
		glutPostRedisplay();
		break;
	case 'Y':
		yRot -= 1.0f;
		glutPostRedisplay();
		break;
	case 'Z':
		zRot -= 1.0f;
		glutPostRedisplay();
		break;
	case 'x':
		xRot += 1.0f;
		glutPostRedisplay();
		break;
	case 'y':
		yRot += 1.0f;
		glutPostRedisplay();
		break;
	case 'z':
		zRot += 1.0f;
		glutPostRedisplay();
		break;
	case 'p':
		cout << endl;
		cout << "xRot = " << xRot << endl;
		cout << "yRot = " << yRot << endl;
		cout << "zRot = " << zRot << endl;
		cout << "xDist = " << xDist << endl;
		cout << "yDist = " << yDist << endl;
		cout << "zDist = " << zDist << endl;
		break;
	case 'q':
		exit(0);
	default:
		break;
	}
}

void mouseCB(int button, int state, int x, int y) {

	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseLeftDown = true;
		} else if (state == GLUT_UP)
			mouseLeftDown = false;
	} else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseRightDown = true;
		} else if (state == GLUT_UP)
			mouseRightDown = false;
	} else if (button == 3) { // Scroll wheel up
		if (state == GLUT_UP) {
			mouseRightDown = false;
			return;
		}
		mouseRightDown = true;
		mouseMotionCB(x, y - 200);
	} else if (button == 4) { // Scroll wheel down
		if (state == GLUT_UP) {
			mouseRightDown = false;
			return;
		}
		mouseRightDown = true;
		mouseMotionCB(x, y + 200);
	}

	glutPostRedisplay();
}

void mouseMotionCB(int x, int y) {

	if (mouseLeftDown) {
		// face 1
		if ((abs(xRot - 0) < 5.0) && (abs(yRot - 0) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			xDist += (x - mouseX) * calcPanVelocity(zDist);
			yDist -= (y - mouseY) * calcPanVelocity(zDist);
		} // face 2
		else if ((abs(xRot - 0) < 5.0) && (abs(yRot - 180) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			xDist -= (x - mouseX) * calcPanVelocity(-zDist);
			yDist -= (y - mouseY) * calcPanVelocity(-zDist);
		} // face 3
		else if ((abs(xRot - 90) < 5.0) && (abs(yRot - 0) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			xDist += (x - mouseX) * calcPanVelocity(-yDist);
			zDist -= (y - mouseY) * calcPanVelocity(-yDist);
		} // face 4
		else if ((abs(xRot + 90) < 5.0) && (abs(yRot - 0) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			xDist += (x - mouseX) * calcPanVelocity(yDist);
			zDist += (y - mouseY) * calcPanVelocity(yDist);
		} // face 5
		else if ((abs(xRot - 0) < 5.0) && (abs(yRot + 90) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			zDist += (x - mouseX) * calcPanVelocity(-xDist);
			yDist -= (y - mouseY) * calcPanVelocity(-xDist);
		} // face 6
		else if ((abs(xRot - 0) < 5.0) && (abs(yRot - 90) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			zDist -= (x - mouseX) * calcPanVelocity(xDist);
			yDist -= (y - mouseY) * calcPanVelocity(xDist);
		}
	}

	if (mouseRightDown) {
		// face 1
		if ((abs(xRot - 0) < 5.0) && (abs(yRot - 0) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			zDist += (y - mouseY) * 0.001;
			mouseY = y;
		} // face 2
		else if ((abs(xRot - 0) < 5.0) && (abs(yRot - 180) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			zDist -= (y - mouseY) * 0.001;
			mouseY = y;
		} // face 3
		else if ((abs(xRot - 90) < 5.0) && (abs(yRot - 0) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			yDist -= (y - mouseY) * 0.001;
			mouseY = y;
		} // face 4
		else if ((abs(xRot + 90) < 5.0) && (abs(yRot - 0) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			yDist += (y - mouseY) * 0.001;
			mouseY = y;
		} // face 5
		else if ((abs(xRot - 0) < 5.0) && (abs(yRot + 90) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			xDist -= (y - mouseY) * 0.001;
			mouseY = y;
		} // face 6
		else if ((abs(xRot - 0) < 5.0) && (abs(yRot - 90) < 5.0)
				&& (abs(zRot - 0) < 5.0)) {
			xDist += (y - mouseY) * 0.001;
			mouseY = y;
		}
	}

	glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Mosaic Surface Viewer");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutMainLoop();
	return 0;
}

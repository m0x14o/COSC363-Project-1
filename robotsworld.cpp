//  ========================================================================
//  COSC363: Computer Graphics (2017);  University of Canterbury.
//
//  FILE NAME: Humanoid.cpp
//  See Lab02.pdf for details
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadTGA.h"
#include "loadBMP.h"
using namespace std;

GLUquadric *q;
GLuint texId[9];

//--Globals ---------------------------------------------------------------
int cam_hgt = 0; //Camera height
float angle = 10.0;  //Rotation angle for viewing
int t = 0;
float g = 9.8;
float speed = 0;

const float r = 3.14159265359/180;
float lgt_amb[4] = {0.2, 0.2, 0.2, 1.0};
float lgt_dif[4] = {1.0, 1.0, 1.0, 1.0};
float lgt_spe[4] = {1.0, 1.0, 1.0, 1.0};
float mat_amb[4] = {0.0, 0.0, 1.0, 1.0};
float mat_dif[4] = {0.0, 0.0, 1.0, 1.0};
float mat_spe[4] = {1.0, 1.0, 1.0, 1.0};

float eyex = 0;
float eyey = 15;
float eyez = 24;
float eyet = 0;
float lookx = 0;
float looky = 15;
float lookz = 0;


float rpx = -15;  //robot position x
float rpy = 0;  //robot position y
float rpz = -10;  //robot position z
float rt = 0;   //robot turning
float rs = 0;   //robot steps
float rl = 0;   //robot legs
bool change = false;

float bpx = -15;  //box position x
float bpy = 0;  //box y
float bpz = -10;  //box z
float bt = 0;   //box turning

float cpx = 30;  //car postion x
float cpy = 0;  //car y
float cpz = 0;  //car z
float ct = 0;   //car turning

bool stop = false;

bool view = true;

void loadTexture()
{
	glGenTextures(9, texId);
	
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	loadTGA("front.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	loadTGA("back.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	loadTGA("left.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	loadTGA("right.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	loadTGA("up.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, texId[5]);
	loadTGA("down.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, texId[6]);
	loadTGA("recycle.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glBindTexture(GL_TEXTURE_2D, texId[7]);
	loadTGA("wall.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glBindTexture(GL_TEXTURE_2D, texId[8]);
	loadTGA("glass_texture.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void loadSkybox()
{
	glEnable(GL_TEXTURE_2D);
	
	//front
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glBegin(GL_QUADS);
	glTexCoord2f( 0.,0. );glVertex3f(-50,  0, 50);
	glTexCoord2f( 1.,0. );glVertex3f(50, 0., 50);
	glTexCoord2f( 1.,1. );glVertex3f(50, 50, 50);
	glTexCoord2f( 0.,1. );glVertex3f(-50, 50, 50);
	glEnd();

	//back
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glBegin(GL_QUADS);
	glTexCoord2f( 0.,0. );glVertex3f( 50, 0, -50);
	glTexCoord2f( 1.,0. );glVertex3f(-50, 0, -50);
	glTexCoord2f( 1.,1. );glVertex3f(-50, 50, -50);
	glTexCoord2f( 0.,1. );glVertex3f( 50, 50, -50);
	glEnd();
	
	//left
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	glBegin(GL_QUADS);
	glTexCoord2f( 0.,0. );glVertex3f(-50, 0, -50);
	glTexCoord2f( 1.,0. );glVertex3f(-50, 0., 50);
	glTexCoord2f( 1.,1. );glVertex3f(-50, 50, 50);
	glTexCoord2f( 0.,1. );glVertex3f(-50, 50, -50);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	glBegin(GL_QUADS);
	glTexCoord2f( 0.,0. );glVertex3f(50, 0, 50);
	glTexCoord2f( 1.,0. );glVertex3f(50, 0, -50);
	glTexCoord2f( 1.,1. );glVertex3f(50, 50, -50);
	glTexCoord2f( 0.,1. );glVertex3f(50, 50, 50);
	glEnd();
	
	//top
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	glBegin(GL_QUADS);
	glTexCoord2f( 0.,0. );glVertex3f(50, 50,  50);
	glTexCoord2f( 1.,0. );glVertex3f(50, 50,  -50);
	glTexCoord2f( 1.,1. );glVertex3f(-50, 50, -50);
	glTexCoord2f( 0.,1. );glVertex3f(-50, 50, 50);
	glEnd();
	
	//bottom
	glBindTexture(GL_TEXTURE_2D, texId[5]);
	glBegin(GL_QUADS);
	glTexCoord2f( 0.,0. );glVertex3f(-50, 0, -50);
	glTexCoord2f( 1.,0. );glVertex3f(-50, 0, 50);
	glTexCoord2f( 1.,1. );glVertex3f(50, 0, 50);
	glTexCoord2f( 0.,1. );glVertex3f(50, 0, -50);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(0., 0.5, 0.);			//Floor colour
	for(float i = -50; i <= 50; i ++)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);
			glVertex3f(i, 0 , -50);
			glVertex3f(i, 0, 50);
		glEnd();
	}
}


void drawCube()
{
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);   //front
		glTexCoord2f(0., 0.);  glVertex3f(-1, -1, 1);
		glTexCoord2f(1., 0.);  glVertex3f(1, -1, 1);
		glTexCoord2f(1., 1.);  glVertex3f(1, 1, 1);
		glTexCoord2f(0., 1.);  glVertex3f(-1, 1, 1);

		glNormal3f(0.0, 0.0, -1.0);   //back
		glTexCoord2f(0., 0.);  glVertex3f(1, -1, -1);
		glTexCoord2f(1., 0.);  glVertex3f(-1, -1,-1);
		glTexCoord2f(1., 1.);  glVertex3f(-1, 1,-1);
		glTexCoord2f(0., 1.);  glVertex3f(1, 1, -1);

		glNormal3f(0.0, 1.0, 0.0);   //top
		glTexCoord2f(0., 0.);  glVertex3f(-1, 1, 1);
		glTexCoord2f(1., 0.);  glVertex3f(1, 1,  1);
		glTexCoord2f(1., 1.);  glVertex3f(1, 1, -1);
		glTexCoord2f(0., 1.);  glVertex3f(-1, 1, -1);
		
		glNormal3f(0.0, -1.0, 0.0);   //bottom
		glTexCoord2f(0., 0.);  glVertex3f(1, -1, 1);
		glTexCoord2f(1., 0.);  glVertex3f(-1, -1, 1);
		glTexCoord2f(1., 1.);  glVertex3f(-1, -1, -1);
		glTexCoord2f(0., 1.);  glVertex3f(1, -1, -1);
		
		glNormal3f(1.0, 0.0, 0.0);   //right
		glTexCoord2f(0., 0.);  glVertex3f(1, -1, 1);
		glTexCoord2f(1., 0.);  glVertex3f(1, -1, -1);
		glTexCoord2f(1., 1.);  glVertex3f(1, 1, -1);
		glTexCoord2f(0., 1.);  glVertex3f(1, 1, 1);
		
		glNormal3f(-1.0, 0.0, 0.0);   //left
		glTexCoord2f(0., 0.);  glVertex3f(-1, -1, -1);
		glTexCoord2f(1., 0.);  glVertex3f(-1, -1, 1);
		glTexCoord2f(1., 1.);  glVertex3f(-1, 1, 1);
		glTexCoord2f(0., 1.);  glVertex3f(-1, 1, -1);
	glEnd();
}
//--Draws a character model constructed using GLUT objects ------------------
void drawRobot()
{
	//Head
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-360, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-345, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-330, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-315, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-300, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-285, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-270, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-255, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-240, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-225, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-210, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-195, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-180, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-165, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-150, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-135, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-120, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-105, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-90, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-75, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-60, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-45, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-30, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //screw
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glRotatef(-15, 0, 1, 1);
	  glTranslatef(-1.4, 0, 0);
	  glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
	glPushMatrix();  //wire
	  glColor3f(1., 0.78, 0.06);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.6, -0.1);
	  glRotatef(-45, 1, 0, 0);
	  glutSolidTorus(0.03, 1.39, 99, 99);
	glPopMatrix();
	glPushMatrix();  //left eye
	  glColor3f(1., 0.98, 0.98);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-0.5, 4.8, 1.4);
	  glutSolidTorus(0.1, 0.5, 99, 99);
	glPopMatrix();
	glPushMatrix();  //right eye
	  glColor3f(1., 0.98, 0.98);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0.5, 4.8, 1.4);
	  glutSolidTorus(0.1, 0.5, 99, 99);
	glPopMatrix();
	glPushMatrix();  //head
	  glColor3f(1., 0.77, 0.05);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 4.7, 0);
	  glutSolidSphere(1.4, 99, 99);
	glPopMatrix();

	//Torso
	glPushMatrix();  //disk
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glColor3f(0, 0.75, 1);
	  glTranslatef(0, 2, 0.66);
	  glScalef(1, 1, 0.01);
	  glutSolidSphere(0.7, 99, 99);
	glPopMatrix();
	glPushMatrix();  //lamp
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glColor3f(1., 0., 0.);
	  glTranslatef(.7, 3, 0.65);
	  glutSolidSphere(0.12, 99, 99);
	glPopMatrix();
	glPushMatrix();  //lamp
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glColor3f(0., 1., 0.);
	  glTranslatef(-.7, 3, 0.65);
	  glutSolidSphere(0.12, 99, 99);
	glPopMatrix();
	glPushMatrix();  //lamp
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glColor3f(0., 0., 1.);
	  glTranslatef(0, 3, 0.65);
	  glutSolidSphere(0.12, 99, 99);
	glPopMatrix();
	glPushMatrix();  //body
	  glColor3f(1., 0.27, 0.);
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0, 2.2, 0);
	  glScalef(2, 2.3, 1.3);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right leg
	glPushMatrix();  //foot
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-0.8, 0.1, 0.2);
	  glRotatef(-rl, 1, 0, 0);
	  glScalef(1, 0.45, 1.5);
	  glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();  //let
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-0.8, 0.2, 0);
	  glRotatef(-rl, 1, 0, 0);
	  glRotatef(-90, 1, 0, 0);
	  glutSolidCone(0.2, 2, 99, 99);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left leg
	glPushMatrix();  //foot
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0.8, 0.1, 0.2);
	  glRotatef(rl, 1, 0, 0);
	  glScalef(1, 0.45, 1.5);
	  glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();  //let
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(0.8, 0.2, 0);
	  glRotatef(rl, 1, 0, 0);
	  glRotatef(-90, 1, 0, 0);
	  glutSolidCone(0.2, 2, 99, 99);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right arm
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-1.9, .7, 0);
	  glRotatef(120, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-1.9, .9, 0);
	  glRotatef(-135, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-1.55, .7, 0);
	  glRotatef(60, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-1.55, .9, 0);
	  glRotatef(-45, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //arm
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-1.55, 1.7, 0);
	  glRotatef(-10, 0, 0, 1);
	  glScalef(0.8, 1.4, 0.8);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //big arm
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(-1.2, 2.8, 0);
	  glRotatef(-30, 0, 0, 1);
	  glScalef(0.9, 3, 0.9);
	  glutSolidSphere(0.2, 99, 99);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left arm
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(1.9, .7, 0);
	  glRotatef(-120, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(1.9, .9, 0);
	  glRotatef(135, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(1.55, .7, 0);
	  glRotatef(-60, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //finger
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(1.55, .9, 0);
	  glRotatef(45, 0, 0, 1);
	  glScalef(0.4, 0.1, 0.4);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //arm
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(1.55, 1.7, 0);
	  glRotatef(10, 0, 0, 1);
	  glScalef(0.8, 1.4, 0.8);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //big arm
	  glTranslatef(rpx, rpy, rpz);
	  glRotatef(rt, 0, 1, 0);
	  //glTranslatef(-rpx, -rpy, -rpz);
	  glTranslatef(1.2, 2.8, 0);
	  glRotatef(30, 0, 0, 1);
	  glScalef(0.9, 3, 0.9);
	  glutSolidSphere(0.2, 99, 99);
	glPopMatrix();
}

void drawBox()
{
	glColor3f(0.93, 0.77, 0.57);
	
	glPushMatrix();  //back flip
	  glTranslatef(bpx, bpy, bpz);
	  glRotatef(bt, 0, 1, 0);
	  glTranslatef(0, 3, -3.63);
	  glRotatef(-45, 1, 0, 0);
	  glScalef(2.5, 0.05, 1.25);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //left flip
	  glTranslatef(bpx, bpy, bpz);
	  glRotatef(bt, 0, 1, 0);
	  glTranslatef(-0.625, 3.58, -2);
	  glRotatef(-165, 0, 0, 1);
	  glScalef(1.25, 0.05, 2.5);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //right flip
	  glTranslatef(bpx, bpy, bpz);
	  glRotatef(bt, 0, 1, 0);
	  glTranslatef(0.625, 3.58, -2);
	  glRotatef(-15, 0, 0, 1);
	  glScalef(1.25, 0.05, 2.5);
	  glutSolidCube(1);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[6]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();  //body
	  glTranslatef(bpx, bpy, bpz);
	  glRotatef(bt, 0, 1, 0);
	  glTranslatef(0, 2.2, -2);
	  glScalef(1.25, 1.25, 1.25);
	  drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}


void drawCar()
{
	glPushMatrix();  //tyre
	  glColor3f(0, 0, 0);
	  glTranslatef(-1.2, 0.5, -2);
	  glRotatef(90, 0, 1, 0);
	  glutSolidTorus(0.2, 0.4, 99, 99);
	glPopMatrix();
	glPushMatrix();  //tyre
	  glColor3f(0, 0, 0);
	  glTranslatef(1.2, 0.5, -2);
	  glRotatef(90, 0, 1, 0);
	  glutSolidTorus(0.2, 0.4, 99, 99);
	glPopMatrix();
	glPushMatrix();  //tyre
	  glColor3f(0, 0, 0);
	  glTranslatef(-1.2, 0.5, 2);
	  glRotatef(90, 0, 1, 0);
	  glutSolidTorus(0.2, 0.4, 99, 99);
	glPopMatrix();
	glPushMatrix();  //tyre
	  glColor3f(0, 0, 0);
	  glTranslatef(1.2, 0.5, 2);
	  glRotatef(90, 0, 1, 0);
	  glutSolidTorus(0.2, 0.4, 99, 99);
	glPopMatrix();
	glPushMatrix();  //lamp
	  glColor3f(1, 1, 0);
	  glTranslatef(-0.8, 1, 2.75);
	  glScalef(1, 1, 0.01);
	  glutSolidSphere(0.3, 99, 99);
	glPopMatrix();
	glPushMatrix();  //lamp
	  glColor3f(1, 1, 0);
	  glTranslatef(0.8, 1, 2.75);
	  glScalef(1, 1, 0.01);
	  glutSolidSphere(0.3, 99, 99);
	glPopMatrix();
	glPushMatrix();  //top
	  glColor3f(1, 0, 0);
	  glTranslatef(0, 2, -0.7);
	  glScalef(3, 1, 3);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();  //body
	  glColor3f(1, 0, 0);
	  glTranslatef(0, 1, 0);
	  glScalef(3, 1, 5.5);
	  glutSolidCube(1);
	glPopMatrix();
}

void drawBuilding()
{
	glColor3f(0.8, 0.67, 0.49);
	glPushMatrix();
	  glTranslatef(-7, 20, -7);
	  glRotatef(90, 0, 0, 1);
	  glScalef(8, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(7, 20, -7);
	  glRotatef(90, 0, 0, 1);
	  glScalef(8, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(-7, 20, 7);
	  glRotatef(90, 0, 0, 1);
	  glScalef(8, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(7, 20, 7);
	  glRotatef(90, 0, 0, 1);
	  glScalef(8, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(-7, 20, 0);
	  glRotatef(90, 0, 1, 0);
	  glScalef(17, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(7, 20, 0);
	  glRotatef(90, 0, 1, 0);
	  glScalef(17, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(0, 20, -7);
	  glScalef(17, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(0, 20, 7);
	  glScalef(17, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	glColor3f(0.55, 0.45, 0.33);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[7]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPushMatrix();  //upper base
	  
	  glTranslatef(0, 12, 0);
	  glScalef(8, 4, 8);
	  drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glColor4f(.9, .9, .9, .9);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texId[8]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();  //base
	  glTranslatef(0, 4, 0);
	  glScalef(8, 4, 8);
	  drawCube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void patrolTimer(int value)
{
	if(value < 30 && !stop)
	{
		rpx += sin(rt*r)/2;
		rpz += cos(rt*r)/2;
		bpx += sin(bt*r)/2;
		bpz += cos(bt*r)/2; 
		change = !change;
		if(change) rl += 15;
		else if(!change) rl -= 15;
		glutPostRedisplay();
		value++;
		glutTimerFunc(100, patrolTimer, value);
	}
	else if(value >= 30 && !stop)
	{
		rt += 45; 
		bt += 45;
		change = !change;
		if(change) rl += 15;
		else if(!change) rl -= 15;
		glutPostRedisplay();
		value = 0;
		glutTimerFunc(100, patrolTimer, value);
	}
	else if(stop)
	{
		while(bpy >= -1.1)
		{
			speed += 0.001 * g;
			bpy -= speed;
			glutPostRedisplay();
		}
	}
	
}

void carTimer(int carvalue)
{
	if(carvalue < 25)
	{
		cpx += sin(ct*r)/2;
		cpz += cos(ct*r)/2;
		glutPostRedisplay();
		carvalue++;
		
		glutTimerFunc(100, carTimer, carvalue);
	}
	else if(carvalue >= 25)
	{
		ct += 5; 
		cpx += sin(ct*r)/2;
		cpz += cos(ct*r)/2;
		t = int(ct) % 180;
		glutPostRedisplay();
		carvalue++;
		
		if(t == 0) carvalue = 0;
		glutTimerFunc(100, carTimer, carvalue);
	}
	
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()  
{
	float lpos1[4] = {10., 10., 10., 1.0};  //light's position
	float lpos2[4] = {-10., 10., 10., 1.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if(view){
		gluLookAt(eyex, eyey+cam_hgt, eyez, lookx, looky, lookz, 0, 1, 0);
	}
	else if(!view){
		gluLookAt(rpx, rpy+5, rpz, rpx+sin(rt*r)/2 , rpy+5, rpz+cos(rt*r)/2 , 0, 1, 0);
	}
	glLightfv(GL_LIGHT0,GL_POSITION, lpos1);   //Set light position
	glLightfv(GL_LIGHT1,GL_POSITION, lpos2);

    //glRotatef(angle, 0.0, 1.0, 0.0);  //Rotate the scene about the y-axis

	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    //drawFloor();
    loadSkybox();

	glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model
	drawRobot();
	glPushMatrix();
	  drawBox();
	glPopMatrix();
	glPushMatrix();
	  glTranslatef(cpx, cpy, cpz);
	  glRotatef(ct, 0, 1, 0);
	  drawCar();
	glPopMatrix();
	glPushMatrix();
	  drawBuilding();
	glPopMatrix();

	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	q = gluNewQuadric();
	loadTexture();
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lgt_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lgt_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lgt_spe);
	
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lgt_amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lgt_dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lgt_spe);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spe);
	glMaterialf(GL_FRONT, GL_SHININESS, 50); 
	
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
	//lookx += sin(eyet*r);lookz += sin(eyet*r);
	
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-10.0, 10.0, -10.0, 10.0, 5.0, 120.0);   //Camera Frustum
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) 
    {
		eyet -= 5;
		lookx = eyex + (sin(eyet*r));
		lookz = eyez - (cos(eyet*r));
	}
    else if(key == GLUT_KEY_RIGHT) 
    {
		eyet += 5;
		lookx = eyex + (sin(eyet*r));
		lookz = eyez - (cos(eyet*r));
	}
    else if(key == GLUT_KEY_UP) 
    {
		eyex += sin(eyet*r);
		eyez -= cos(eyet*r);
		lookx = eyex + (sin(eyet*r));
		lookz = eyez - (cos(eyet*r));
	}
    else if(key == GLUT_KEY_DOWN) 
    {
		eyex -= sin(eyet*r);
		eyez += cos(eyet*r);
		lookx = eyex + (sin(eyet*r));
		lookz = eyez - (cos(eyet*r));
	}
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'a': rt += 5; if(!stop) bt += 5;change = !change; break;
		case 'd': rt -= 5; if(!stop) bt -= 5;change = !change; break;
		case 'w': rpx += sin(rt*r)/2;rpz += cos(rt*r)/2;if(!stop) bpx += sin(bt*r)/2;if(!stop) bpz += cos(bt*r)/2; change = !change; break;
		case 's': rpx -= sin(rt*r)/2;rpz -= cos(rt*r)/2;if(!stop) bpx -= sin(bt*r)/2;if(!stop) bpz -= cos(bt*r)/2; change = !change; break;
		case 'v': view = !view; break;
		case 't': stop = true;
	}
	if(change) rl += 15;
	else if(!change) rl -= 15;
	t = 0;
	glutPostRedisplay();
}
//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Humanoid");
   initialize();

   glutDisplayFunc(display);
   glutTimerFunc(100, patrolTimer, 0);
   glutTimerFunc(100, carTimer, 0);
   glutSpecialFunc(special); 
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}

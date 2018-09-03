// sphere.cpp

#include "car.h"
double l = 2;//longth of the car 车长
double w = 0.8;//half width of the 车宽一半
double wheelW = 0.2*w;
double wheelH = 0.15*l;

Car::Car()
{
	quadric1 = gluNewQuadric();
	quadric2 = gluNewQuadric();
	gluQuadricDrawStyle(quadric1, GLU_LINE);
	gluQuadricDrawStyle(quadric2, GLU_FILL);//wireframe mode
}

Car::~Car()
{
	gluDeleteQuadric(quadric1);
	gluDeleteQuadric(quadric2);
}

void Car::drawCylinder(int slices, int stacks, int l)
{
	glColor3f(0, 0, 0);
	gluCylinder(quadric1, 0.05*l, 0.05*l, l, slices, stacks);
}

int Car::render()
{
	glPushMatrix();
	drawCar();

	glPopMatrix();
	return true;
}

void Car::drawCar()
{
	glLineWidth(1);
	drawCylinder(20, 6, l);
}

void Car::drawBaseWheel()
{
	glPushMatrix();
	glLineWidth(3);//线宽加粗
	glRectf(0, -wheelH, wheelW, wheelH);//drawing wheel support spoke 画辐条
	glRotatef(90, 0, 1, 0);
	//glutWireTorus(.1, 0.5, 20, 50);
	glLineWidth(1);//线宽恢复
	gluCylinder(quadric2, wheelH, wheelH, wheelW, 20, 5);
	glRotatef(90, 1, 0, 0);
	glLineWidth(3);
	glRectf(-wheelH, 0, wheelH, wheelW);
	glPopMatrix();
}

void Car::drawWheel()
{
	glPushMatrix();
	glTranslatef(w, 0, 0);
	drawBaseWheel();		//right front

	glTranslatef(wheelW, 0, 0);
	glRotatef(45, 1, 0, 0); // different angle from wheel spoke
	glScalef(0.2, .45, .45);
	drawBaseWheel();		//drawing wheel hub

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-w - wheelW, 0, 0);
	drawBaseWheel();		//left front

	glTranslatef(-wheelW * 0.2, 0, 0);
	glRotatef(45, 1, 0, 0);
	glScalef(0.2, .45, .45);//drawing wheel hub
	drawBaseWheel();

	glPopMatrix();
}

void Car::drawTransmission()
{
	glPushMatrix();
	glLineWidth(1);

	glRotatef(90, 0, 1, 0);
	gluCylinder(quadric1, 0.06*l, 0.06*l, w + wheelW, 16, 4);//right front
	glRotatef(180, 0, 1, 0);
	gluCylinder(quadric1, 0.06*l, 0.06*l, w + wheelW, 16, 4);//left front
	glPopMatrix();
	//glTranslatef(l, 0, 0);
	//gluCylinder(quadric1, 0.06*l, 0.06*l, w, 20, 10);//left back
	//glRotatef(180, 0, 1, 0);
	//gluCylinder(quadric1, 0.06*l, 0.06*l, w + wheelW, 20, 10);//rignt back
}

void Car::drawbackAxel()
{
	glTranslatef(0, 0, l);
	glScalef(1.1, 1.3, 1.3);
}
void Car::DrawAxle()
{
	glTranslatef(0, 0, -l * 0.5);
	//glPushMatrix();

	//drawWheel();
	//glPopMatrix();
	//glPushMatrix();

	//drawWheel();
	//glPopMatrix();
	//glPushMatrix();

	////drawCylinder( );
	//glPopMatrix();
}
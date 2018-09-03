#ifndef __CarA__
#define __CarA__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "utility.h"
#include "glUtils.h"

class Car
{
protected:

	GLUquadricObj *quadric1;
	GLUquadricObj *quadric2;
	static GLfloat	spinAngle;
	void drawCylinder(int slices, int stacks, int l);

	void drawCar();

	void drawBaseWheel();

public:

	virtual int render();
	void drawbackAxel();
	void drawWheel();
	void drawTransmission();

	void  DrawAxle();
	Car();
	~Car();
};

#endif

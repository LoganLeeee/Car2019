/****	car tute 	****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

//#include "utility.h"
//#include "glUtils.h"

#include "Axes.h"

#include "Car.h"

#define framesPerSecond 25
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame = false;

//#define Near	 0.5
//#define Far	20.0

#define ESC	  27

#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99
#define  GLUT_WHEEL_UP 3           //定义滚轮操作
#define  GLUT_WHEEL_DOWN 4

#include <math.h>
static float c = M_PI / 180.0f; //弧度和角度转换参数
static int du = -33, oldmy = 1, oldmx = 1; //du是视点绕y轴的角度,opengl里默认y轴是上方向
static float r = 5.5f, h = 5.0f; //r是视点绕y轴的半径,h是视点高度即在y轴上的坐标

static int   fb = -1, turn = 1; // Car movment:  fb:forward or backward  ; turn: left/right turn

static int	WinWidth, WinHeight;
static int	AppMenu;
//static GLfloat	Spin;
static GLfloat	SpinAngle;

static GLfloat	eyePoint[3];
static GLfloat	lookAtPoint[3];
static GLfloat	upVec[3];//
static GLfloat  fov; // field of view
static GLfloat	near;
static GLfloat	far;

static GLfloat	color3[3];

static GLfloat	red[] = { 1.0, 0.0, 0.0 };
static GLfloat	blue[] = { 0.0, 1.0, 0.0 };
static GLfloat	green[] = { 0.0, 0.0, 1.0 };
static GLfloat	cyan[] = { 0.0, 1.0, 1.0 };
static GLfloat	magenta[] = { 1.0, 0.0, 1.0 };
static GLfloat	yellow[] = { 1.0, 1.0, 0.0 };
static GLfloat	black[] = { 0.0, 0.0, 0.0 };
static GLfloat	white[] = { 1.0, 1.0, 1.0 };

Axes *axes; //
Car *car;

static void setProjection()
{
	GLfloat aspect;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspect = (float)WinWidth / (float)WinHeight;
	gluPerspective(fov, aspect, near, far);
	/* Back to normal */
	glMatrixMode(GL_MODELVIEW);
}

static void seteyePoint()
{
	glLoadIdentity();
	gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2],
		lookAtPoint[0], lookAtPoint[1], lookAtPoint[2],
		upVec[0], upVec[1], upVec[2]);
}

static void SetUp3D()
{
	setProjection();
	//seteyePoint();
	  /* Wireframe mode */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(1, 1, 1, 1);
}

static void drawWorld()
{
	car->DrawAxle();
	glPushMatrix();
	//axes->render();
	//glRotatef(SpinAngle, 0.0, 1.0, 0.0);
	//axes->drawAxesP(1);
	//glColor3fv(color3);
	//car->DrawAxle();
	glColor3f(0, 0, 0);
	car->render();

	glPopMatrix();

	// draw Front parts
	glPushMatrix();
	glRotatef(turn, 0.0, 1.0, 0.0);					//Orientation of wheels and Transmission depends on key pressed.
	glColor3f(0.09*abs(turn), 0.08*abs(turn), 0);	// Transmission's color depends on orientation.
	car->drawTransmission();						// drawing Front Transmission.
	glRotatef(fb*SpinAngle, 1.0, 0.0, 0.0);			// Spining speed same as back wheels.
	glColor3f(0, 0, 0.8);
	car->drawWheel();								// drawing Front Wheels
	glPopMatrix();

	// draw Steering parts
	glPushMatrix();
	glRotatef(turn * 4, 0.0, 1.0, 1.0);				//Orientation of Steering depends on key pressed.
	glColor3f(0.09*abs(turn) + 0.2, 0.08*abs(turn) + 0.2, 0);	// Steering's color depends on orientation.
	car->drawSteering();
	glPopMatrix();

	// draw Back parts
	glPushMatrix();
	car->drawbackAxel();
	glRotatef(fb*SpinAngle, 1.0, 0.0, 0.0);	// Spining speed depends on key pressed.
	glColor3f(-0.3*fb, 0.3*fb, 0);			// Transmission's color depends on speed.
	car->drawTransmission();				// drawing Back Transmission
	glColor3f(0, 0, 0.8);
	car->drawWheel();						// drawing Back Wheels
	glPopMatrix();
}

void CheckGL()
{
	int  err;
	char msg[256];

	err = glGetError();
	if (err != GL_NO_ERROR) {
		sprintf_s(msg, "OpenGL error: %s", (String)gluErrorString(err));
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///鼠标转视角
	  //printf("At:%.2f %.2f %.2f\n",r*cos(c*du),h,r*sin(c*du)); //这就是视点的坐标

	glLoadIdentity();
	gluLookAt(r*cos(c*du), h, r*sin(c*du), 0, 0, 0, 0, 1, 0);	//从视点看远点,y轴方向(0,1,0)是上方向
	///
	SetUp3D();
	drawWorld();
	///
	glFlush();
	///

	  /* Check everything OK and update screen */
	CheckGL();
	glutSwapBuffers();
}

/****			     鼠标视角控制	 			    ****/

/****	Start  change viewing angle by mouse  	 ****/

void Mouse(int button, int state, int x, int y) //处理鼠标点击
{
	if (state == GLUT_DOWN)						//第一次鼠标按下时,记录鼠标在窗口中的初始坐标
	{
		oldmx = x, oldmy = y;
	}
	if (state == GLUT_UP && button == GLUT_WHEEL_UP)
	{
		fov += 10;
		printf("滚轮向上");
	}
	if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
	{
		fov -= 10;
		printf("滚轮向上");
	}
}
void onMouseMove(int x, int y)					//处理鼠标拖动
{
	printf("%d\n",du);
	du += x - oldmx;							//鼠标在窗口x轴方向上的增量加到视点绕y轴的角度上，这样就左右转了
	h += 0.03f*(y - oldmy);						//鼠标在窗口y轴方向上的改变加到视点的y坐标上，就上下转了
	if (h > 8.0f) h = 8.0f;						//视点y坐标作一些限制，不会使视点太奇怪
	else if (h < -8.0f) h = -8.0f;
	oldmx = x, oldmy = y;						//把此时的鼠标坐标作为旧值，为下一次计算增量做准备
}
//void init()
//{
//	glEnable(GL_DEPTH_TEST);
//}
void reshape(int w, int h)
{
	WinWidth = w;
	WinHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0f, (float)w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}
///
/****	END	change viewing angle by mouse  	 ****/

/****	        鼠标视角控制结束	            ****/

static void resize(int width, int height)
{
	/* Save for event handlers */
	WinWidth = width;
	WinHeight = height;

	/* Reset view in window. */
	glViewport(0, 0, WinWidth, WinHeight);
}

/****		User events		****/

static void menuChoice(int item)
{
	switch (item) {
	case cmdRed:
		color3[0] = 1; // R
		color3[1] = 0; // G
		color3[2] = 0; // B
		break;
	case cmdGreen:
		color3[0] = 0; // R
		color3[1] = 1; // G
		color3[2] = 0; // B
		break;
	case cmdExit:
		exit(0);
		break;
	default:
		break;
	}
}

/* In most GUI systems we would write just one event handler
   for all kinds of keystrokes. In GLUT, we need one for the
   standard ASCII keys and one for special cursor or function
   style keys that vary from system to system. Because the
   GLUT special key code range overlaps with ASCII lowercase,
   it isn't safe to use the same function for both.        */

static void asciiKey(unsigned char key, int x, int y)
{
	if (key == ESC)
		menuChoice(cmdExit);
}

static void specialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_F2)
	{
		eyePoint[0] = 1.0;
		eyePoint[1] = 1.0; // changed
		eyePoint[2] = 1.0;

		lookAtPoint[0] = 0;
		lookAtPoint[1] = 0;
		lookAtPoint[2] = 0;

		upVec[0] = 0;
		upVec[1] = 1;
		upVec[2] = 0;

		fov = 120; // field of view
		near = 0.5;
		far = 20;
	}

	if (key == GLUT_KEY_PAGE_UP)
	{
		fov -= 10;
	}
	if (key == GLUT_KEY_PAGE_DOWN)
	{
		fov += 10;
	}
	// Use up/down key control speed and orientation.
	if (key == GLUT_KEY_UP)
	{
		if (fb != -3) { fb -= 1; }
		else	 fb = -3;
	}

	if (key == GLUT_KEY_DOWN)
	{
		if (fb != 3) { fb += 1; }
		else	  fb = 3;
	}
	// Use left/right key control turning orientation.
	if (key == GLUT_KEY_LEFT)
	{
		if (turn < 6) { turn += 3; }
		else turn = 9;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (turn > -6) { turn -= 3; }
		else	  turn = -9;
	}
}

/****		Startup			****/

static void initGraphics(void)
{
	/* Black background */
	glClearColor(0, 0, 0, 0);

	/* colour green */
	color3[0] = 0;
	color3[0] = 1;
	color3[0] = 0;

	/* Needed for vertex arrays */
	glEnableClientState(GL_VERTEX_ARRAY);

	/* Popup menu attached to right mouse button */
	AppMenu = glutCreateMenu(menuChoice);
	glutSetMenu(AppMenu);
	glutAddMenuEntry("Red", cmdRed);
	glutAddMenuEntry("Green", cmdGreen);
	glutAddMenuEntry("----", 0);
	glutAddMenuEntry("Exit", cmdExit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Start values */
	SpinAngle = 0.0;

	//eyePoint[0] = 5.0;
	//eyePoint[1] = 5.0;
	//eyePoint[2] = 5.0;

	//lookAtPoint[0] = 0;
	//lookAtPoint[1] = 0;
	//lookAtPoint[2] = 0;

	//upVec[0] = 0;
	//upVec[1] = 1;
	//upVec[2] = 0;

	fov = 60; // field of view  /视野距离
	near = 0;
	far = 120;

	menuChoice(cmdGreen);

	car = new Car();
	axes = new Axes();
}

void timer(int value)
{
	timeForNextFrame = true;
	glutTimerFunc(timerMSecs, timer, 0);
}

/****		Main control		****/

static void spinDisplay(void)
{
	if (timeForNextFrame)
	{
		SpinAngle += 2.0;
		if (SpinAngle > 360.0)
			SpinAngle -= 360.0;
		glutPostRedisplay();
		timeForNextFrame = false;
	}
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cube");

	initGraphics();

	//glutDisplayFunc(display);
	//glutReshapeFunc(resize);

	glutKeyboardFunc(asciiKey);
	glutSpecialFunc(specialKey);

	///
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);
	//
	glutIdleFunc(spinDisplay);
	glutTimerFunc(timerMSecs, timer, 0);

	glutMainLoop();
	/* Should never get here, but keeps compiler happy */
	return 0;
}
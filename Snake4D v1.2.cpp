#include <GL/freeglut.h>
#include "GlutFunc.h"
#include <ctime>
#include <vector>
#include <cmath>
#include "Quaternion.h"

//#include <iostream>
// Global parameters
int mainWindow;
int subWindow;

double animx = 0;
double animy = 0;
double animz = 0;
int lastSnakex = 0;
int lastSnakey = 0;
int lastSnakez = 0;
int lastSnaket = 0;

int CubeCameraNumber;

bool subWindowFlag = true;
bool cameraAlign = false;

double xMouse, yMouse;

double lgbt = 0;
int flipOver = 1;
int w, h;

int frame;
long timeFPS, timebaseFPS;
char s[50];

unsigned current_time = clock();

const unsigned N = 16;
double const SnakeSize = 0.9;
double const animTime = 500;
double const animCameraTime = 400;
const unsigned MSEC = CLOCKS_PER_SEC / 1000;
const double PI = 3.1415926535;
const double halfPI = 1.57079632679;

const unsigned delay_msec = 500;

double xAngle = 0, yAngle = 0;
unsigned char key = 0;
unsigned char tempKey = key;

bool gameOver = false;

using Vec4 = Quaternion<int>;

Vec4 current(0, 1, 0, 0);
Vec4 tempCurrent = current;
Vec4 movementCurrent = current;
Vec4 lastCurrent = current;

Vec4 currentNormal(0, 0, -1, 0);
Vec4 tempCurrentNormal = currentNormal;
Vec4 movementCurrentNormal = currentNormal;
Vec4 lastCurrentNormal = currentNormal;

Vec4 randTile()
{
	srand(time(NULL));
	unsigned x1, x2, x3, x4;
	x1 = 1 + rand() % N;
	x2 = 1 + rand() % N;
	x3 = 1 + rand() % N;
	x4 = 1 + rand() % N;
	return Vec4(x1, x2, x3, x4);
}

Vec4 apple(randTile());

std::vector<Vec4 > snake(2);

void update()
{
	lastCurrent = current;
	lastCurrentNormal = currentNormal;
	if (tempCurrent.re == 0)
	{
		current = tempCurrent;
		currentNormal = tempCurrentNormal;
	}
	key = tempKey;
	tempKey = 0;
	snake.insert(snake.begin(), snake[0] + tempCurrent);
	if (abs(snake[0].re - (N + 1) / 2.0) > (N - 1) / 2.0  || abs(snake[0].i - (N + 1) / 2.0) > (N - 1) / 2.0 || abs(snake[0].j - (N + 1) / 2.0) > (N - 1) / 2.0 || abs(snake[0].k - (N + 1) / 2.0) > (N - 1) / 2.0)
		gameOver = true;
	for (unsigned i = 1; i < snake.size(); ++i)
	{
		if (snake[i].re == snake[0].re && snake[i].i == snake[0].i && snake[i].j == snake[0].j && snake[i].k == snake[0].k)
			gameOver = true;
	}
	if (snake[0].re != apple.re || snake[0].i != apple.i || snake[0].j != apple.j || snake[0].k != apple.k)
		snake.pop_back();
	else
		apple = randTile();
}

void reset()
{
	snake.erase(snake.begin() + 2, snake.end());
	snake[0] = randTile();
	CubeCameraNumber = snake[0].re - 1;
	gameOver = false;
}


void gameLogic()
{
	if (gameOver)
	{
		reset();
	}
	if (clock() >= current_time + delay_msec * MSEC)
	{
		lastSnakex = snake[snake.size() - 1].i;
		lastSnakey = snake[snake.size() - 1].j;
		lastSnakez = snake[snake.size() - 1].k;
		lastSnaket = snake[snake.size() - 1].re;
		update();

		current_time = clock();
	}
}

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
//               Window   			   //
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
void setProjection(int w1, int h1)
{
	float ratio;
	ratio = 1.0f * w1 / h1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w1, h1);
	gluPerspective(45, ratio, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void changeSize(int ww, int hh) {
	w = ww;
	h = hh;

	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

	if (subWindowFlag)
	{
		glutSetWindow(subWindow);
		glutPositionWindow(w * 3 / 4, h - w / 4);
		glutReshapeWindow(w / 4, w / 4);
		setProjection(w / 4, w / 4);
	}
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//
//               Window   			   //
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
//               Camera      	       //
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
void CameraAligning()
{
	static double deltaAngleY = 0, currentDeltaAngleY = 0, deltaAngleX = 0, currentDeltaAngleX = 0;
	static long alignTimeStart = clock();
	if (cameraAlign)
	{

		deltaAngleX = -xAngle;
		deltaAngleY = -yAngle + PI / 6;
		alignTimeStart = clock();
	}

	if (clock() - alignTimeStart < animCameraTime / 2)
	{

		yAngle += sqrt((clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleY - currentDeltaAngleY;
		xAngle += sqrt((clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleX - currentDeltaAngleX;
		currentDeltaAngleY = sqrt((clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleY;
		currentDeltaAngleX = sqrt((clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleX;
		cameraAlign = false;
		if (yAngle > halfPI + abs(sqrt(1 - (clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleY))
		{
			yAngle = halfPI + abs(sqrt(1 - (clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleY);
		}
		if (yAngle < -halfPI - abs(sqrt(1 - (clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleY))
		{
			yAngle = -halfPI - abs(sqrt(1 - (clock() - alignTimeStart) / (animCameraTime / 2)) * deltaAngleY);
		}
	}
	else
	{
		deltaAngleX = 0;
		deltaAngleY = 0;
		currentDeltaAngleY = 0;
		currentDeltaAngleX = 0;
		/*if (yAngle > halfPI)
		{
			yAngle = halfPI;
		}
		if (yAngle < -halfPI)
		{
			yAngle = -halfPI;
		}*/
	}


}
void CameraMovement(unsigned char keyLast)
{
	static double deltaAngleY = 0, currentDeltaAngleY = 0, deltaAngleX = 0, currentDeltaAngleX = 0;
	if (clock() - current_time < animCameraTime)
	{
		switch (keyLast)
		{
		case 'w':
			yAngle += halfPI;
			deltaAngleX = 0;
			deltaAngleY = -halfPI;
			break;
		case 's':
			yAngle -= halfPI;
			deltaAngleX = 0;
			deltaAngleY = halfPI;
			break;
		case 'a':
			xAngle -= halfPI;
			deltaAngleX = halfPI;
			deltaAngleY = 0;
			break;
		case 'd':
			xAngle += halfPI;
			deltaAngleX = -halfPI;
			deltaAngleY = 0;
			break;

		case 'q':
		case 'e':
		case 0:
			deltaAngleX = 0;
			deltaAngleY = 0;
			break;
		default:
			break;
		}
		yAngle += (sin((clock() - current_time) / (animCameraTime)*PI - halfPI) + 1) / 2 * deltaAngleY - currentDeltaAngleY;
		xAngle += (sin((clock() - current_time) / (animCameraTime)*PI - halfPI) + 1) / 2 * deltaAngleX - currentDeltaAngleX;
		currentDeltaAngleY = (sin((clock() - current_time) / (animCameraTime)*PI - halfPI) + 1) / 2 * deltaAngleY;
		currentDeltaAngleX = (sin((clock() - current_time) / (animCameraTime)*PI - halfPI) + 1) / 2 * deltaAngleX;
		key = 1;
		if (yAngle > halfPI + abs(sqrt(1 - (clock() - current_time) / (animCameraTime)) * deltaAngleY))
		{
			yAngle = halfPI + abs(sqrt(1 - (clock() - current_time) / (animCameraTime)) * deltaAngleY);
		}
		if (yAngle < -halfPI - abs(sqrt(1 - (clock() - current_time) / (animCameraTime)) * deltaAngleY))
		{
			yAngle = -halfPI - abs(sqrt(1 - (clock() - current_time) / (animCameraTime)) * deltaAngleY);
		}
	}
	else
	{
		deltaAngleX = 0;
		deltaAngleY = 0;
		currentDeltaAngleY = 0;
		currentDeltaAngleX = 0;
		if (yAngle > halfPI)
		{
			yAngle = halfPI;
		}
		if (yAngle < -halfPI)
		{
			yAngle = -halfPI;
		}
	}


}
double movementCurrentCoord(double time, int coordLast, int coordNew)
{
	switch (coordLast) {
	case 0:
		switch (coordNew) {
		case 0:
			return 0;
			break;
		case 1:
			return sin(time / halfPI);
			break;
		case -1:
			return -sin(time / halfPI);
		}
		break;
	case 1:
		return cos(time / halfPI);
		break;
	case -1:
		return -cos(time / halfPI);
	}
}
void CurrentMovement()
{
	movementCurrent = current;
	movementCurrentNormal = currentNormal;
	if (current.re == 0 && clock() - current_time < animCameraTime && lastCurrent.k == current.k && lastCurrent.i == current.i && lastCurrent.j == current.j)
	{
		movementCurrent.i = movementCurrentCoord((clock() - current_time) / (animCameraTime), lastCurrent.i, current.i);
		movementCurrent.j = movementCurrentCoord((clock() - current_time) / (animCameraTime), lastCurrent.j, current.j);
		movementCurrent.k = movementCurrentCoord((clock() - current_time) / (animCameraTime), lastCurrent.k, current.k);
	}
	if (current.re == 0 && clock() - current_time < animCameraTime && lastCurrentNormal.k == currentNormal.k && lastCurrentNormal.i == currentNormal.i && lastCurrentNormal.j == currentNormal.j)
	{
		movementCurrent.i = movementCurrentCoord((clock() - current_time) / (animCameraTime), lastCurrentNormal.i, currentNormal.i);
		movementCurrent.j = movementCurrentCoord((clock() - current_time) / (animCameraTime), lastCurrentNormal.j, currentNormal.j);
		movementCurrent.k = movementCurrentCoord((clock() - current_time) / (animCameraTime), lastCurrentNormal.k, currentNormal.k);
	}
}
void cameraUpdate()
{
	//CameraAligning();
	CameraMovement(key);
	//CurrentMovement();
	double objectx = snake[0].i + (snake[0].re - 1) * 1000 + animx;
	double objecty = snake[0].j + (snake[0].re - 1) * 1000 + animy;
	double objectz = snake[0].k + (snake[0].re - 1) * 1000 + animz;
	double deltaEyex = 0, deltaEyey = 0, deltaEyez = 0;
	if (xAngle > 2 * PI)
	{
		xAngle -= 2 * PI;
	}
	if (xAngle < -2 * PI)
	{
		xAngle += 2 * PI;
	}
	if (yAngle > 2 * PI)
	{
		yAngle -= 2 * PI;
	}
	if (yAngle < -2 * PI)
	{
		yAngle += 2 * PI;
	}

	if (currentNormal.i != 0)
	{
		if (current.j != 0)
		{
			deltaEyex = 10 * sin(yAngle) * currentNormal.i;
			deltaEyey = -10 * cos(xAngle) * cos(yAngle) * current.j;
			deltaEyez = -10 * sin(xAngle) * cos(yAngle) * current.j * currentNormal.i;
		}
		if (current.k != 0)
		{
			deltaEyex = 10 * sin(yAngle) * currentNormal.i;
			deltaEyey = 10 * sin(xAngle) * cos(yAngle) * current.k * currentNormal.i;
			deltaEyez = -10 * cos(xAngle) * cos(yAngle) * current.k;
		}
	}

	if (currentNormal.j != 0)
	{
		if (current.k != 0)
		{
			deltaEyex = -10 * sin(xAngle) * cos(yAngle) * current.k * currentNormal.j;
			deltaEyey = 10 * sin(yAngle) * currentNormal.j;
			deltaEyez = -10 * cos(xAngle) * cos(yAngle) * current.k;
		}
		if (current.i != 0)
		{
			deltaEyex = -10 * cos(xAngle) * cos(yAngle) * current.i;
			deltaEyey = 10 * sin(yAngle) * currentNormal.j;
			deltaEyez = 10 * sin(xAngle) * cos(yAngle) * current.i * currentNormal.j;
		}
	}

	if (currentNormal.k != 0)
	{
		if (current.i != 0)
		{
			deltaEyex = -10 * cos(xAngle) * cos(yAngle) * current.i;
			deltaEyey = -10 * sin(xAngle) * cos(yAngle) * current.i * currentNormal.k;
			deltaEyez = 10 * sin(yAngle) * currentNormal.k;

		}
		if (current.j != 0)
		{
			deltaEyex = 10 * sin(xAngle) * cos(yAngle) * current.j * currentNormal.k;
			deltaEyey = -10 * cos(xAngle) * cos(yAngle) * current.j;
			deltaEyez = 10 * sin(yAngle) * currentNormal.k;
		}
	}
	Vec4 deltaEye = (currentNormal - current) * Vec4(10, 0, 0, 0);
	double eyex = objectx + deltaEyex;
	double eyey = objecty + deltaEyey;
	double eyez = objectz + deltaEyez;
	double remainder = yAngle - floor(yAngle / (2 * PI)) * 2 * PI;
	if (remainder > halfPI && remainder < 3 * halfPI - 0.001)
	{
		flipOver = -1;
	}
	else
	{
		flipOver = 1;
	}
	gluLookAt(eyex, eyey, eyez,
		objectx, objecty, objectz,
		flipOver * currentNormal.i, flipOver * currentNormal.j, flipOver * currentNormal.k);

}
void subWindowCamera()
{
	gluLookAt(CubeCameraNumber * 1000 - 20, CubeCameraNumber * 1000 + 10 + N, CubeCameraNumber * 1000 - 20,
		CubeCameraNumber * 1000 + (N / 2 + 0.5), CubeCameraNumber * 1000 + (N / 2 + 0.5), CubeCameraNumber * 1000 + (N / 2 + 0.5),
		0, 1, 0);
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//
//               Camera     	       //
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
//               Animation   	       //
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
void animSnakeTile(int i, int type)
{
	if (clock() < current_time + animTime * MSEC && type == 1)
	{
		if (i < snake.size() - 1)
		{
			animx = (((clock() - current_time) / (animTime * MSEC)) - 1) * (snake[i].i - snake[i + 1].i);
			animy = (((clock() - current_time) / (animTime * MSEC)) - 1) * (snake[i].j - snake[i + 1].j);
			animz = (((clock() - current_time) / (animTime * MSEC)) - 1) * (snake[i].k - snake[i + 1].k);
		}
		else
		{
			animx = (((clock() - current_time) / (animTime * MSEC)) - 1) * (snake[i].i - lastSnakex);
			animy = (((clock() - current_time) / (animTime * MSEC)) - 1) * (snake[i].j - lastSnakey);
			animz = (((clock() - current_time) / (animTime * MSEC)) - 1) * (snake[i].k - lastSnakez);

		}
	}
}


double snakeTileAnimation4thDimension(int type, int i)
{
	double k4thDimension = 1;
	if (type == 1)
	{
		if (i == snake.size() - 1)
		{
			if (snake[i].re != lastSnaket)
			{
				k4thDimension = (clock() - current_time) / (animTime * MSEC);
			}
		}
		if (i < snake.size() - 1)
		{
			if (snake[i].re != snake[i + 1].re)
			{
				k4thDimension = (clock() - current_time) / (animTime * MSEC);
			}
		}

		if (i > 0)
		{
			if (snake[i].re != snake[i - 1].re)
			{
				k4thDimension = 1 - (clock() - current_time) / (animTime * MSEC);
			}
		}

		if (i > 0 && i < snake.size() - 1 && snake[i].re != snake[i - 1].re && snake[i].re != snake[i + 1].re)
		{
			k4thDimension = 1 - 2 * abs((clock() - current_time) / (animTime * MSEC) - 1 / 2);
		}
		if (i > 0 && i == snake.size() - 1 && snake[i].re != snake[i - 1].re && snake[i].re != lastSnaket)
		{
			k4thDimension = 1 - 2 * abs((clock() - current_time) / (animTime * MSEC) - 1 / 2);
		}
	}
	return k4thDimension;
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//
//               Animation   	       //
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
//               Rendering  		   //
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
void renderSnakeTile(int x, int y, int z, int t, int type, int i) // type == 0-head // 1-body // 2-apple // 3-teapot
{
	switch (type)
	{
	case 1:
		glColor3f(0, 0, 1);
		break;
	case 2:
		glColor3f(1, 0, 0);
		break;
	}
	double k4thDimension = snakeTileAnimation4thDimension(type, i);
	animSnakeTile(i, type);
	if (clock() >= current_time + animTime * MSEC || type == 2)
	{
		animx = 0;
		animy = 0;
		animz = 0;
	}


	glCubeSolid(x + (t - 1) * 1000 + animx, y + (t - 1) * 1000 + animy, z + (t - 1) * 1000 + animz, k4thDimension * SnakeSize);
}


void FPS() //rendring frames per second as a test
{
	frame++;

	timeFPS = clock();
	if (timeFPS - timebaseFPS > 1000) {
		sprintf_s(s, "FPS:%4.2f",
			frame * 1000.0 / (timeFPS - timebaseFPS));
		timebaseFPS = timeFPS;
		frame = 0;
	}
	setOrthographicProjection(w, h);
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString2f(12, 75, GLUT_BITMAP_HELVETICA_18, s);
	glPopMatrix();
	restorePerspectiveProjection();
}

void cubeNumberText(int cubeNumber, int x, int y, int ww, int hh) //rendering number of the cube as a text
{
	char s[5]; // char for rendering
	sprintf_s(s, "%d", cubeNumber); // int to char
	setOrthographicProjection(ww, hh); // to render text with coordinates of the creen
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString2f(x, y, GLUT_BITMAP_HELVETICA_18, s); //rendering the text
	glPopMatrix();
	restorePerspectiveProjection(); // restore ccordinates
}

void renderCube(double CubeNumber) // rendering the chosen cube
{

	if (CubeNumber < 0) // not allowing to render the cube if it is out of bounds
	{
		CubeNumber = 0;
	}
	if (CubeNumber > N - 1) // not allowing to render the cube if it is out of bounds
	{
		CubeNumber = N - 1;
	}
	glColor3f(abs(sin(lgbt)), abs(cos(lgbt)), abs(-sin(lgbt))); // 3 random functions which are from Real Numbers to [0,1]

	glCubeCell(CubeNumber * 1000 + (N / 2 + 0.5), CubeNumber * 1000 + (N / 2 + 0.5), CubeNumber * 1000 + (N / 2 + 0.5), N, (N / 2) - 1);//(x,y,z,size,amount of rods) //CubeNumber * 1000 - 4th dimension is expressed by offset of cubes // (N / 2 + 0.5) - offset, so the corner is at 1,1,1


	for (int i = 0; i < snake.size(); i++) //rendering all snake
	{
		if (snake[i].re == CubeNumber + 1) //rendering the snake which we can see
		{
			renderSnakeTile(snake[i].i, snake[i].j, snake[i].k, snake[i].re, 1, i);//rendering a part of the snake (x,y,z,t,type(1-is a snake),number of the body)
		}
	}
	if (apple.re == CubeNumber + 1)//rendering the apple which we can see
	{
		renderSnakeTile(apple.i, apple.j, apple.k, apple.re, 2, 0);//rendering an apple (x,y,z,t,type(2-is an apple),number of the body(it is an apple, so it is 0))
	}
}

void subWindowEdging()
{
	setOrthographicProjection(w / 4, w / 4); // to render text with coordinates of the screen
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(w / 400, 0);
	glVertex2f(w / 400, w / 4);
	glVertex2f(0, w / 4);

	glVertex2f(0, 0);
	glVertex2f(0, w / 400);
	glVertex2f(w / 4, w / 400);
	glVertex2f(w / 4, 0);

	glVertex2f(w / 4, w / 4);
	glVertex2f(w / 4, 0);
	glVertex2f(w / 4 - w / 400, 0);
	glVertex2f(w / 4 - w / 400, w / 4);

	glVertex2f(w / 4, w / 4);
	glVertex2f(0, w / 4);
	glVertex2f(0, w / 4 - w / 400);
	glVertex2f(w / 4, w / 4 - w / 400);
	glEnd();
	glPopMatrix();
	restorePerspectiveProjection(); // restore ccordinates
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//
//               Rendering   	       //
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
//               Scenes   			   //
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
void renderSceneMainWindow()//rendering the main window (with a game)
{
	glutSetWindow(mainWindow);//the window we are going to work with

	gameLogic();//logic of the game

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear buffers to preset values
	glLoadIdentity();
	animSnakeTile(0, 1);//find the offset because of the animation
	cameraUpdate();//set the camera
	//FPS();//write Frames per second on the screen
	cubeNumberText(snake[0].re, 12, 30, w, h);
	lgbt += 0.0003f;
	renderCube(snake[0].re - 1);// rendering the chosen cube 
	glutSwapBuffers();//swapping the bufers(for animation)
}

void renderSceneSubWindow()//rendering the sub window (with a mini camera of the cubes)
{
	glutSetWindow(subWindow);//the window we are going to work with

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear buffers to preset values
	glLoadIdentity();//replace the current matrix with the identity matrix
	subWindowCamera();//set the camera
	cubeNumberText(CubeCameraNumber + 1, w / 100, w / 100, w / 4, h / 4);
	subWindowEdging();
	renderCube(CubeCameraNumber);// rendering the chosen cube
	glutSwapBuffers();//swapping the buufers(for animation)
}

void renderAllScenes()//rendering all scenes simultaneously
{
	renderSceneMainWindow();//rendering the main window (with a game)
	if (subWindowFlag)
	{
		renderSceneSubWindow();//rendering the sub window (with a mini camera of the cubes)
	}
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//
//               Scenes   			   //
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
//          Computer functions         //
//          Keybord and mouse          //
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
void changeDirection(unsigned char keyboardKey)
{

	if (tempCurrent.re != 0)
	{
		switch (keyboardKey) {
		case 'q':
			tempCurrent = Vec4(1, 0, 0, 0);
			return;
			break;
			tempKey = keyboardKey;
		case 'e':
			tempCurrent = Vec4(-1, 0, 0, 0);
			return;
			break;
			tempKey = keyboardKey;
		default:
			tempCurrent = current;
		}
		return;

	}
	else
	{
		switch (keyboardKey)
		{
		case 'w':
			tempCurrent = currentNormal;
			tempCurrentNormal = -current;
			tempKey = keyboardKey;
			break;
		case 's':
			tempCurrent = -currentNormal;
			tempCurrentNormal = current;
			tempKey = keyboardKey;
			break;
		case 'a':
			tempCurrent = currentNormal * current;
			tempCurrentNormal = currentNormal;
			tempKey = keyboardKey;
			break;
		case 'd':
			tempCurrent = current * currentNormal;
			tempCurrentNormal = currentNormal;
			tempKey = keyboardKey;
			break;
		case 'q':
			tempCurrent = Vec4(-1, 0, 0, 0);
			tempKey = keyboardKey;
			break;
		case 'e':
			tempCurrent = Vec4(1, 0, 0, 0);
			tempKey = keyboardKey;
			break;
		default:

			break;
		}
	}

}


void processSpecialKeys(int keyboard, int xx, int yy)
{
	switch (keyboard) {
	case GLUT_KEY_LEFT:
		if (CubeCameraNumber > 0)
		{
			CubeCameraNumber--;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (CubeCameraNumber < N - 1)
		{
			CubeCameraNumber++;
		}
		break;
	}
}

void mouseMove(int x, int y) {

	if (abs(w / 2 - x) > w / 4 || abs(h / 2 - y) > h / 4)
	{
		glutWarpPointer(w / 2, h / 2);
		xMouse = w / 2;
		yMouse = h / 2;
	}
	else
	{
		double xdeltaAngle = (x - xMouse) * -0.005f;
		double ydeltaAngle = (y - yMouse) * 0.005f;
		xAngle += xdeltaAngle;
		yAngle += ydeltaAngle;

		xMouse = x;
		yMouse = y;
	}
}
void mouseWheel(int wheel, int direction, int x, int y) {
	if (direction == 1)
	{
		
			CubeCameraNumber++;
			CubeCameraNumber = (CubeCameraNumber + N) % N;
			
	}
	if (direction == -1)
	{
		
			CubeCameraNumber--;
			CubeCameraNumber = (CubeCameraNumber + N) % N;
	}

}
void processNormalKeys(unsigned char keyboard, int xx, int yy)
{
	switch (keyboard)
	{
	case 9:
		if (subWindowFlag)
		{
			subWindowFlag = false;
			glutDestroyWindow(subWindow);
		}
		else
		{
			subWindowFlag = true;
			subWindow = glutCreateSubWindow(mainWindow, 100, 100, 190, 190);
			glutDisplayFunc(renderSceneSubWindow);
			glutSpecialFunc(processSpecialKeys);
			glEnable(GL_LINE_SMOOTH);
			glLineWidth(0.0000000001);
			glEnable(GL_DEPTH_TEST);
			glutSetWindow(subWindow);
			glutPositionWindow(w * 3 / 4, h - w / 4);
			glutReshapeWindow(w / 4, w / 4);
			setProjection(w / 4, w / 4);
		}
		break;
	case 27:
		exit(0);
		break;
	case 'z':
		if (CubeCameraNumber > 0)
		{
			CubeCameraNumber--;
		}
		break;
	case 'x':
		if (CubeCameraNumber < N - 1)
		{
			CubeCameraNumber++;
		}
		break;
	case 'c':
		cameraAlign = true;

		break;
	default:
		changeDirection(keyboard);
		break;
	}
}
void computerFunc()
{
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutPassiveMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseWheel);

	glEnable(GL_DEPTH_TEST);
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//
//          Computer functions         //
//          Keybord and mouse          //
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑//
int main(int argc, char** argv) {

	srand(time(NULL));

	unsigned x1, x2, x3, x4;
	x1 = 2 + rand() % (N - 2);
	x2 = 2 + rand() % (N - 2);
	x3 = 2 + rand() % (N - 2);
	x4 = 2 + rand() % (N - 2);
	snake[0] = Vec4(x1, x2, x3, x4);
	snake[1] = snake[0] - current;
	lastSnakex = snake[snake.size() - 1].i;
	lastSnakey = snake[snake.size() - 1].j;
	lastSnakez = snake[snake.size() - 1].k;
	lastSnaket = snake[snake.size() - 1].re;
	CubeCameraNumber = snake[0].re - 1;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 400);
	mainWindow = glutCreateWindow("Snake4D");
	glEnable(GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glHint(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutSetCursor(GLUT_CURSOR_NONE);
	glEnable(GL_LINE_SMOOTH);
	//lLineWidth(0.0000000001);


	glutFullScreen();

	glutDisplayFunc(renderSceneMainWindow);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderAllScenes);
	computerFunc();

	subWindow = glutCreateSubWindow(mainWindow, 100, 100, 190, 190);
	glutDisplayFunc(renderSceneSubWindow);
	glutSpecialFunc(processSpecialKeys);
	glEnable(GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glHint(GLUT_MULTISAMPLE, 8);
	//glLineWidth(0.0000000001);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 1;
}
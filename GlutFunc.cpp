#include <glut.h>
#include <iostream>
void setOrthographicProjection(int w, int h) {
	//выбрать режим проекции
	glMatrixMode(GL_PROJECTION);
	//Сохраняем предыдущую матрицу, которая содерж
	//параметры перспективной проекции
	glPushMatrix();
	//обнуляем матрицу
	glLoadIdentity();
	//устанавливаем 2D ортогональную проекцию
	gluOrtho2D(0, w, h, 0);
	//выбираем режим обзора модели
	glMatrixMode(GL_MODELVIEW);
}

void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	//восстановить предыдущую матрицу проекции
	glPopMatrix();
	//вернуться в режим модели
	glMatrixMode(GL_MODELVIEW);
}

/*void changeSize(int w, int h) {

	glutWarpPointer(w / 2, h / 2);
	glutSetCursor(GLUT_CURSOR_NONE);
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void changeSize2D(int w, int h) {
	// предотвращение деления на ноль
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	// используем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	// обнуляем матрицу
	glLoadIdentity();
	// установить параметры вьюпорта
	glViewport(0, 0, w, h);
	// установить корректную перспективу
	gluOrtho2D(0, w, h, 0);
	// вернуться к матрице проекции
	glMatrixMode(GL_MODELVIEW);
}*/

void renderBitmapString2f(
	float x,
	float y,
	void* font,
	char* string) {

	char* c;
	glRasterPos3f(x, y, 0);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderBitmapString3f(
	float x,
	float y,
	float z,
	void* font,
	const char* string) {

	const char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}



void renderSceneTIW() {
	//очистить буфер цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// обнулить трансформацию
	glLoadIdentity();
	char s[256];
	sprintf_s(s, "HI");
	renderBitmapString2f(10, 10, GLUT_BITMAP_HELVETICA_18, s);
	glutSwapBuffers();
}

void renderTextInWindow(int mainWindow, int x, int y, int w, int h, void* font, char* string) {
	int subWindow1 = glutCreateSubWindow(mainWindow, x, y, x + w, y + h);
	glutDisplayFunc(renderSceneTIW);
	glutMainLoop();
}

void glCube(double x, double y, double z, double size)
{
	double s = size / 2;

	glBegin(GL_LINES);
	glVertex3f(x - s, y - s, z - s);
	glVertex3f(x + s, y - s, z - s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x + s, y - s, z - s);
	glVertex3f(x + s, y + s, z - s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x + s, y + s, z - s);
	glVertex3f(x - s, y + s, z - s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x - s, y + s, z - s);
	glVertex3f(x - s, y - s, z - s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x - s, y - s, z + s);
	glVertex3f(x + s, y - s, z + s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x + s, y - s, z + s);
	glVertex3f(x + s, y + s, z + s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x + s, y + s, z + s);
	glVertex3f(x - s, y + s, z + s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x - s, y + s, z + s);
	glVertex3f(x - s, y - s, z + s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x - s, y - s, z - s);
	glVertex3f(x - s, y - s, z + s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x + s, y - s, z + s);
	glVertex3f(x + s, y - s, z - s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x + s, y + s, z + s);
	glVertex3f(x + s, y + s, z - s);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x - s, y + s, z + s);
	glVertex3f(x - s, y + s, z - s);
	glEnd();

}


void glCubeCell(double x, double y, double z, double size, int rods)
{

	double s = size / 2;
	double rodStep = size / (rods + 1);
	glCube(x, y, z, size);
	for (int i = 1; i <= rods; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(x - s, y - s + rodStep * i, z - s);
		glVertex3f(x + s, y - s + rodStep * i, z - s);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(x - s, y - s, z - s + rodStep * i);
		glVertex3f(x + s, y - s, z - s + rodStep * i);
		glEnd();


		glBegin(GL_LINES);
		glVertex3f(x - s + rodStep * i, y - s, z - s);
		glVertex3f(x - s + rodStep * i, y + s, z - s);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(x - s, y - s, z - s + rodStep * i);
		glVertex3f(x - s, y + s, z - s + rodStep * i);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(x - s + rodStep * i, y - s, z - s);
		glVertex3f(x - s + rodStep * i, y - s, z + s);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(x - s, y - s + rodStep * i, z - s);
		glVertex3f(x - s, y - s + rodStep * i, z + s);
		glEnd();



		glBegin(GL_LINES);
		glVertex3f(x + s, y + s - rodStep * i, z + s);
		glVertex3f(x - s, y + s - rodStep * i, z + s);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(x + s, y + s, z + s - rodStep * i);
		glVertex3f(x - s, y + s, z + s - rodStep * i);
		glEnd();


		glBegin(GL_LINES);
		glVertex3f(x + s - rodStep * i, y + s, z + s);
		glVertex3f(x + s - rodStep * i, y - s, z + s);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(x + s, y + s, z + s - rodStep * i);
		glVertex3f(x + s, y - s, z + s - rodStep * i);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(x + s - rodStep * i, y + s, z + s);
		glVertex3f(x + s - rodStep * i, y + s, z - s);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(x + s, y + s - rodStep * i, z + s);
		glVertex3f(x + s, y + s - rodStep * i, z - s);
		glEnd();
	}
}

void glCubeSolid(double x, double y, double z, double size)
{
	double s = size / 2;

	glBegin(GL_QUADS);
	glVertex3f(x - s, y - s, z - s);
	glVertex3f(x + s, y - s, z - s);
	glVertex3f(x + s, y - s, z + s);
	glVertex3f(x - s, y - s, z + s);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(x - s, y - s, z - s);
	glVertex3f(x + s, y - s, z - s);
	glVertex3f(x + s, y + s, z - s);
	glVertex3f(x - s, y + s, z - s);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(x - s, y - s, z - s);
	glVertex3f(x - s, y + s, z - s);
	glVertex3f(x - s, y + s, z + s);
	glVertex3f(x - s, y - s, z + s);
	glEnd();



	glBegin(GL_QUADS);
	glVertex3f(x + s, y + s, z + s);
	glVertex3f(x - s, y + s, z + s);
	glVertex3f(x - s, y + s, z - s);
	glVertex3f(x + s, y + s, z - s);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(x + s, y + s, z + s);
	glVertex3f(x - s, y + s, z + s);
	glVertex3f(x - s, y - s, z + s);
	glVertex3f(x - s, y - s, z + s);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(x + s, y + s, z + s);
	glVertex3f(x + s, y - s, z + s);
	glVertex3f(x + s, y - s, z - s);
	glVertex3f(x + s, y + s, z - s);
	glEnd();
	glColor3f(1, 1, 1);
	glCube(x, y, z, size + 0.01f);
}

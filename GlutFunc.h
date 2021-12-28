#pragma once
#pragma once
void setOrthographicProjection(int w, int h);//3D->2D
void restorePerspectiveProjection();//Back to 3D
//void changeSize(int ww, int hh); //glutReshapeFunc(changeSize);
void renderBitmapString2f(
	float x,
	float y,
	void* font,
	char* string);//2D GLUT_BITMAP_HELVETICA_18
void renderBitmapString3f(
	float x,
	float y,
	float z,
	void* font,
	char* string);//3D
void glCube(double x, double y, double z, double size);
void glCubeCell(double x, double y, double z, double size, int rods);
void glCubeSolid(double x, double y, double z, double size);
void renderTextInWindow(int mainWindow, int x, int y, int w, int h, void* font, char* string);
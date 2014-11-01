#include "Window.h"
#include "parser.h"
#include "GL\glut.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(Window::getWidth(), Window::getHeight());
	glutCreateWindow("Rasterizer");

	//loadData();

	glutReshapeFunc(Window::reshapeCallback);
	glutDisplayFunc(Window::displayCallback);
	glutKeyboardFunc(Window::keyboardCallback);
	glutMainLoop();
}
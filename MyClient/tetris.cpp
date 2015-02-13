#include <GL/glut.h>
#include <GL/freeglut.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Painter.h"
using namespace std;
int stop = 0;
int speed = 0;

Painter *P = NULL;
Snake *S = NULL;
Figure *F = NULL;
Heap *Hp = NULL;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	P->DrawField();
	P->Draw(S);
	P->Draw(F);
//	glutSwapBuffers();
}

void keyboard(int  key,int x,int y)
{
    x++;
    y++;
	if (stop != 1)
	{
		switch(key)
		{       
			case GLUT_KEY_DOWN: { F->SetType(); break; }
            case GLUT_KEY_INSERT: { exit(0);break;}
		}
//		glutPostRedisplay();
	}
}


void KeyEvent(unsigned char key,int x,int y)
{
    x++;
    y++;
	if (stop != 1)
	{
		switch(key)
		{       
			case 'a': { S->Turn('l'); break; }
			case 'd': { S->Turn('r'); break; }
			case 'w': { S->Turn('u'); break; }
			case 's': { S->Turn('d'); break; }
			case 't': { F->SetType(); break; }
			case 'j': { F->Move('l', Hp, S); break; }
			case 'l': { F->Move('r', Hp, S); break; }
			case 'i': { F->Turn(); break; }
			case 'k': { F->Move('d', Hp, S); break; }
		}
//		glutPostRedisplay();
	}
}

void timer(int =0)
{
	S->Move(*Hp, *F);
	display();
//	glutTimerFunc(400, timer, 0);
}
/*
void Init(int argc, char** argv)
{
	int SizeX = 500;
	int SizeY = 500;

	Figure::Fullmas("settings.xml");
	P = new Painter;
	S = new Snake;
	Hp = new Heap;
	F = new Figure;
	srand(time(NULL));

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SizeX, SizeY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Tetris");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SizeX,0.0, SizeY, 1.0, -1.0);
	glutTimerFunc(200,timer,0);
	glutDisplayFunc(display);
	glutSpecialFunc(keyboard);
	glutKeyboardFunc(KeyEvent);
	glutMainLoop();
}

int main(int argc,char**argv)
{
	Init(argc, argv);
	int temp;
	cin >> temp;
  return 0;
}
*/

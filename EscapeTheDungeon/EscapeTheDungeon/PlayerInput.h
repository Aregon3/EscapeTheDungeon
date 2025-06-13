#pragma once
#include <GL/glut.h>
#include "DRaysWals.h"
#include"Map.h"
typedef struct
{
	int w, a, d, s;                     //button state on off
}
ButtonKeys;
ButtonKeys Keys;

void ButnDown(unsigned char key, int x, int y)                                  //keyboard button pressed down
{
	if (key == 'a') { Keys.a = 1; }
	if (key == 'd') { Keys.d = 1; }
	if (key == 'w') { Keys.w = 1; }
	if (key == 's') { Keys.s = 1; }
	if (key == 'e' && sp[0].state == 0)             //open doors
	{
		int xo = 0; if (pdx < 0) 
		{
			xo = -25;
		}
		else
		{ 
			xo = 25; 
		}
		int yo = 0;
		if (pdy < 0)
		{
			yo = -25;
		}
		else 
		{ 
			yo = 25; 
		}
		int ipx = px / 64.0, ipx_add_xo = (px + xo) / 64.0;
		int ipy = py / 64.0, ipy_add_yo = (py + yo) / 64.0;

		if (mapW[ipy_add_yo * mapX + ipx_add_xo] == 24)
		{ 
			mapW[ipy_add_yo * mapX + ipx_add_xo] = 0; 
		}
	}

	glutPostRedisplay();
}
void ButnUp(unsigned char key, int x, int y)                                    //keyboard button pressed up
{
	if (key == 'a') { Keys.a = 0; }
	if (key == 'd') { Keys.d = 0; }
	if (key == 'w') { Keys.w = 0; }
	if (key == 's') { Keys.s = 0; }
	glutPostRedisplay();
}
#include "Textures/background.ppm"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
void drawBackground()    
{
	int x, y;
	for (y = 0;y < 40;y++)
	{
		for (x = 0;x < 120;x++)
		{
			int xo = (int)pa * 2 - x; if (xo < 0) { xo += 120; } xo = xo % 120;
			int pixel = (y * 120 + xo) * 3;
			int red = background[pixel + 0];
			int green = background[pixel + 1];
			int blue = background[pixel + 2];

			glPointSize(8); 
			glColor3ub(red, green, blue);
			glBegin(GL_POINTS); 
			glVertex2i(x * 8, y * 8);
			glEnd();
		}
	}
}
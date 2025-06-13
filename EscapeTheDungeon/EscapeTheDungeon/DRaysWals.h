#pragma once

#define mapX  8
#define mapY  8    
#define mapS 64
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "Textures/AllTextures.ppm"
#define PI 3.1415926535
float px, py, pdx, pdy, pa;

float degToRad(float a)
{
	return a * PI / 180.0; 
}
float FixAng(float a) 
{ 
	if (a > 359) 
	{ 
		a -= 360;
	} 
	if (a < 0) 
	{
		a += 360; 
	}
	return a; 
}
float distance(float ax, float ay, float bx, float by, float ang)
{
	return cos(degToRad(ang)) * (bx - ax) - sin(degToRad(ang)) * (by - ay); 
}

typedef struct
{
	int type;
	int state;
	int map;
	float x, y, z;
}
sprite;
sprite sp[4];
int depth[120];

void drawRays()
{
	int r, mx, my, mp, dof, side;
	float vx, vy, rx, ry, ra, xo, yo, disV, disH;

	ra = FixAng(pa + 30);                                                              

	for (r = 0;r < 120;r++)
	{
		int vmt = 0, hmt = 0;                                                              
		dof = 0; side = 0; disV = 100000;
		float Tan = tan(degToRad(ra));
		if (cos(degToRad(ra)) > 0.001) 
		{
			rx = (((int)px >> 6) << 6) + 64;
			ry = (px - rx) * Tan + py; xo = 64;
			yo = -xo * Tan;
		}
		else if (cos(degToRad(ra)) < -0.001) 
		{ 
			rx = (((int)px >> 6) << 6) - 0.0001;
			ry = (px - rx) * Tan + py; xo = -64;
			yo = -xo * Tan; 
		}
		else 
		{ 
			rx = px;
			ry = py;
			dof = 8; 
		}                                                  

		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && mapW[mp]>0) 
			{ 
				vmt = mapW[mp] - 1;
				dof = 8; 
				disV = cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra)) * (ry - py); 
			}        
			else 
			{ 
				rx += xo;
				ry += yo; 
				dof += 1;
			}                                               
		}
		vx = rx;
		vy = ry;

		dof = 0;
		disH = 100000;
		Tan = 1.0 / Tan;

		if (sin(degToRad(ra)) > 0.001) 
		{ 
			ry = (((int)py >> 6) << 6) - 0.0001;
			rx = (py - ry) * Tan + px;
			yo = -64; xo = -yo * Tan; 
		}
		else if (sin(degToRad(ra)) < -0.001)
		{ 
			ry = (((int)py >> 6) << 6) + 64;
			rx = (py - ry) * Tan + px;
			yo = 64; xo = -yo * Tan; 
		}
		else 
		{ 
			rx = px; 
			ry = py; 
			dof = 8; 
		}                                                  

		while (dof < 8)
		{
			mx = (int)(rx) >> 6; my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && mapW[mp]>0) 
			{
				hmt = mapW[mp] - 1; 
				dof = 8; 
				disH = cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra)) * (ry - py); 
			}      
			else 
			{ 
				rx += xo;
				ry += yo;
				dof += 1;
			}                                               
		}

		float shade = 1;
		glColor3f(0, 0.8, 0);
		if (disV < disH) { hmt = vmt; shade = 0.5; rx = vx; ry = vy; disH = disV; glColor3f(0, 0.6, 0); }

		int ca = FixAng(pa - ra);
		disH = disH * cos(degToRad(ca));                            
		int lineH = (mapS * 640) / (disH);
		float ty_step = 32.0 / (float)lineH;
		float ty_off = 0;

		if (lineH > 640) 
		{ 
			ty_off = (lineH - 640) / 2.0; lineH = 640; 
		}
		int lineOff = 320 - (lineH >> 1);
		depth[r] = disH; 
		int y;
		float ty = ty_off * ty_step;
		float tx;
		if (shade == 1) 
		{
			tx = (int)(rx / 2.0) % 32;
			if (ra > 180)
			{ 
				tx = 31 - tx; 
			}
		}
		else 
		{
			tx = (int)(ry / 2.0) % 32; 
			if (ra > 90 && ra < 270) 
			{ 
				tx = 31 - tx;
			} 
		}
		for (y = 0;y < lineH;y++)
		{
			int pixel = ((int)ty * 32 + (int)tx) * 3 + (hmt * 32 * 32 * 3);
			int red = AllTextures[pixel + 0] * shade;
			int green = AllTextures[pixel + 1] * shade;
			int blue = AllTextures[pixel + 2] * shade;

			glPointSize(8); 
			glColor3ub(red, green, blue);
			glBegin(GL_POINTS);
			glVertex2i(r * 8, y + lineOff);
			glEnd();

			ty += ty_step;
		}

		
		for (y = lineOff + lineH;y < 640;y++)
		{
			float dy = y - (640 / 2.0), deg = degToRad(ra), raFix = cos(degToRad(FixAng(pa - ra)));
			tx = px / 2 + cos(deg) * 158 * 2 * 32 / dy / raFix;
			ty = py / 2 - sin(deg) * 158 * 2 * 32 / dy / raFix;

			int mp = mapF[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;
			int pixel = (((int)(ty) & 31) * 32 + ((int)(tx) & 31)) * 3 + mp * 3;
			int red = AllTextures[pixel + 0] * 0.7;
			int green = AllTextures[pixel + 1] * 0.7;
			int blue = AllTextures[pixel + 2] * 0.7;

			glPointSize(8); 
			glColor3ub(red, green, blue);
			glBegin(GL_POINTS);
			glVertex2i(r * 8, y);
			glEnd();

			
			mp = mapC[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;
			pixel = (((int)(ty) & 31) * 32 + ((int)(tx) & 31)) * 3 + mp * 3;
			red = AllTextures[pixel + 0];
			green = AllTextures[pixel + 1];
			blue = AllTextures[pixel + 2];

			if (mp > 0,red != 255, green != 0, blue != 255)
			{
				glPointSize(8);
				glColor3ub(red, green, blue);
				glBegin(GL_POINTS);
				glVertex2i(r * 8, 640 - y);
				glEnd(); 
			}
		}

		ra = FixAng(ra - 0.5);                                                               
	}
}

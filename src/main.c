/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "math.h"

struct fieldlike {
	Vector2 p;
	Vector2 direction;
	double Magnitude;
};

struct chargelike {
	Vector2 p;
	double charge;
};

#define gridsize 50
#define n 2

struct fieldlike fieldlines[gridsize][gridsize];
struct chargelike charges[n];

int main ()
{
	InitWindow(1200,1200,"basic window");
	SetTargetFPS(24);

	// body setup
	int i,j,k;

	for(i=0;i<n;i++) {
		charges[i].p = (Vector2){0.1,0.1};
		charges[i].charge = 1;
	}

	charges[0].p = (Vector2){0.5,0.4};
	charges[1].p = (Vector2){0.5,0.6};
	charges[1].charge = -1;

	for(i=0;i<gridsize;i++) {
		for(j=0;j<gridsize;j++) {
			#define fieldline fieldlines[i][j]
			fieldline.p = (Vector2){(double)(i)/gridsize,(double)(j)/gridsize};
			fieldline.direction = (Vector2){0,0};
			fieldline.Magnitude = 0;
		}
	}

	while (!WindowShouldClose())
	{

		BeginDrawing();
		ClearBackground(BLACK);

		for(i=0;i<gridsize;i++) {
			for(j=0;j<gridsize;j++) {
				#define fieldline fieldlines[i][j]
				fieldline.direction = (Vector2){0,0};
				Vector2 forceVector = (Vector2){0,0};
				for(k=0;k<n;k++) {
					#define chargebit charges[k]
					if (Vector2Length(Vector2Subtract(fieldline.p, chargebit.p)) < 0.01) {
						continue;
					}
					
					forceVector = Vector2Add(forceVector, Vector2Scale(Vector2Normalize(Vector2Subtract(fieldline.p, chargebit.p)), chargebit.charge/(pow(Vector2Distance(chargebit.p,fieldline.p),2))));
					#undef chargebit
				}
				fieldline.Magnitude = Vector2Length(forceVector);
				fieldline.direction = Vector2Normalize(forceVector);
				#undef fieldline
			}
		}

		for(k=0;k<n;k++) {
			DrawRectangle(1200*charges[k].p.x-20, 1200*charges[k].p.y-20, 40,40, RED);
		}
		double highestMag = 0;
		for(i=0;i<gridsize;i++) {
			for(j=0;j<gridsize;j++) {
				#define fieldline fieldlines[i][j]
				if (fieldline.Magnitude > highestMag) {
					highestMag = fieldline.Magnitude;
				}
				#undef fieldline
			}
		}
		for(i=0;i<gridsize;i++) {
			for(j=0;j<gridsize;j++) {
				#define fieldline fieldlines[i][j]
				// double length = (fieldline.Magnitude);
				double length = 0.01;
				#define clr CLITERAL(Color) {255,255,255, (int)(255*pow((fieldline.Magnitude/(highestMag)), 0.5))}
				// DrawRectangle(1200*fieldline.p.x, 1200*fieldline.p.y, 10,10, BLUE);
				
				DrawLine((int)(1200*fieldline.p.x), (int)(1200*fieldline.p.y), (int)(1200*fieldline.direction.x*0.01 + 1200*fieldline.p.x), (int)(1200*fieldline.direction.y*0.01+fieldline.p.y*1200), clr);
				#undef clr
				#undef fieldline
			}
		}
		// DrawLine(100,100,120,120,BLUE);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

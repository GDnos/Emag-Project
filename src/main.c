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

double log_scale(double input) {
    if (input >= 1.0) return 1.0;
    if (input <= 0.0) return 0.0;

    double log_val = log10(input);  // log10 of input
    double scaled = (log_val + 9.0) / 9.0;  // map -9.0 → 0..1
    return scaled;
}

Color GetRainbowColor(float t, double tr) {
    // Clamp t between 0 and 1
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // Map t to HSV hue from 0 (red) to 300 (purple), skipping magenta
    float hue = t * 300.0f;   // hue in degrees (0-360)
    float s = 1.0f;           // full saturation
    float v = 1.0f;           // full value

    // Convert HSV to RGB
    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(hue / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float rf, gf, bf;

    if (hue < 60)      { rf = c; gf = x; bf = 0; }
    else if (hue < 120){ rf = x; gf = c; bf = 0; }
    else if (hue < 180){ rf = 0; gf = c; bf = x; }
    else if (hue < 240){ rf = 0; gf = x; bf = c; }
    else if (hue < 300){ rf = x; gf = 0; bf = c; }
    else               { rf = c; gf = 0; bf = x; }

    // Convert to 0-255 range
    unsigned char r = (unsigned char)((rf + m) * 255);
    unsigned char g = (unsigned char)((gf + m) * 255);
    unsigned char b = (unsigned char)((bf + m) * 255);
    unsigned char a = (int)(255*tr);

    return CLITERAL(Color){ r, g, b, a };
}

int main ()
{
	#define WindowSize 1200
	InitWindow(WindowSize,WindowSize,"basic window");
	SetTargetFPS(24);

	// body setup
	int i,j,k;

	for(i=0;i<n;i++) {
		charges[i].p = (Vector2){0.1,0.1};
		charges[i].charge = 3;
	}

	charges[0].p = (Vector2){0.5,0.4};
	charges[1].p = (Vector2){0.5,0.6};
	charges[1].charge = -10;

	for(i=0;i<gridsize;i++) {
		for(j=0;j<gridsize;j++) {
			#define fieldline fieldlines[i][j]
			fieldline.p = (Vector2){((double)(i))/(double)(gridsize),((double)(j))/(double)(gridsize)};
			fieldline.direction = (Vector2){0,0};
			fieldline.Magnitude = 0;
		}
	}
	
	int rhythm = 0;

	while (!WindowShouldClose())
	{

		BeginDrawing();
		ClearBackground(BLACK);

		for(i=0;i<gridsize;i++) {
			for(j=0;j<gridsize;j++) {
				#define fieldline fieldlines[i][j]
				fieldline.direction = (Vector2){0.0,0.0};
				Vector2 forceVector = (Vector2){0.0,0.0};
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
		if(rhythm > 60) {
			rhythm = 0;
		}
		else {
			rhythm += 1;
		}
		// for(i=0;i<gridsize;i++) {
		// 	for(j=0;j<gridsize;j++) {
		// 		#define fieldline fieldlines[i][j]
		// 		if (fieldline.Magnitude > highestMag) {
		// 			highestMag = fieldline.Magnitude;
		// 		}
		// 		#undef fieldline
		// 	}
		// }
		for (i=0;i<n;i++) {
			#define charg charges[i]

			if(fabs(100*charg.charge) > highestMag) {
				highestMag = fabs(100*charg.charge);
			}

			#undef charg
		}
		for(i=0;i<gridsize;i++) {
			for(j=0;j<gridsize;j++) {
				#define fieldline fieldlines[i][j]
				// double length = (fieldline.Magnitude);
				double length = (((double)rhythm/60)*0.8)/((double)gridsize);
				#define clr GetRainbowColor(log_scale(fieldline.Magnitude/(highestMag)), 1)
				// #define clr CLITERAL(Color) {255,255,255,255*log_scale(fieldline.Magnitude/(highestMag))}
				// #define clr 
				// DrawRectangle(1200*fieldline.p.x, 1200*fieldline.p.y, 10,10, BLUE);
				
				DrawLine((int)(WindowSize*fieldline.p.x), (int)(WindowSize*fieldline.p.y), (int)(WindowSize*fieldline.direction.x*length + WindowSize*fieldline.p.x), (int)(WindowSize*fieldline.direction.y*length + fieldline.p.y*WindowSize), clr);
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

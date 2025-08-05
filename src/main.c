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

struct masslike {
	Vector2 p;
	Vector2 v;
	Vector2 v0;
	Vector2 a;
	Vector2 size;
	double density;
	Color color;
};

#define n 3

double getMass(struct masslike m) {
	return m.density*m.size.x*m.size.y;
}

struct masslike masses[n];

int main ()
{
	InitWindow(1200,1200,"basic window");
	SetTargetFPS(24);

	double time;
	double timemax = 1000;

	double dt = 0.03;
	Vector2 positions[n][(int)(timemax/dt)];

	// body setup
	int i,j;
	for(i = 0; i < n; i++)
    {
		masses[i].p = (Vector2){0.1,0.1};
		masses[i].v = (Vector2){0,0};
		masses[i].a = (Vector2){0,0};
		masses[i].v0 = (Vector2){0,0};
		masses[i].color = WHITE;

        masses[i].density = 1;
        masses[i].size = (Vector2){0.02,0.02};
    }

	masses[0].density = 5;
	masses[0].p = (Vector2){0.2, 0.2};
	masses[0].v = (Vector2){0.0,0.0};
	masses[0].color = RED;

	masses[1].density=5;
	masses[1].p = (Vector2){0.5, 0.5};
	masses[1].size = (Vector2){0.1,0.1};
	masses[1].v = (Vector2){0.0,0.01};
	masses[1].color = BLUE;

	for (time = 0; time < timemax; time += dt) {
		for (i=0;i<n;i++) {
			//Calculate mutual forces
			#define m1 masses[i]
			for (j=0;j<n;j++) {
				if(i==j) {
					continue;
				}
				
				#define m2 masses[j]

				m1.a = Vector2Scale(Vector2Normalize(Vector2Subtract(m2.p, m1.p)), (getMass(m2))/(Vector2Distance(m1.p,m2.p)));
				m1.v = Vector2Add(m1.v, Vector2Scale(m1.a, dt));
				
				

				//Check for collision

				if (CheckCollisionRecs(
					(Rectangle){m1.p.x, m1.p.y, m1.size.x, m1.size.y},
					(Rectangle){m2.p.x, m2.p.y, m2.size.x, m2.size.y}
				)) {
					#define mass1 getMass(m1)
					#define mass2 getMass(m2)

					m1.v.x = m1.v.x*(mass1-mass2)/(mass1+mass2) + m2.v0.x*(2*mass2)/(mass1+mass2);
					m1.v.y = m1.v.y*(mass1-mass2)/(mass1+mass2) + m2.v0.y*(2*mass2)/(mass1+mass2);
					

					#undef mass1
					#undef mass2
				}

				#undef m2
				
			}
			if(m1.p.x>1-m1.size.x || m1.p.x<0) {
				m1.v.x = -0.5*m1.v.x;
				if(m1.p.x>1-m1.size.x) {
					m1.p.x=1-m1.size.x;
				}
				if(m1.p.x<0) {
					m1.p.x=0;
				}
			}
			if(m1.p.y>1-m1.size.y || m1.p.y<0){
				m1.v.y = -0.5*m1.v.y;
				if(m1.p.y>1-m1.size.y) {
					m1.p.y=1-m1.size.y;
				}
				if(m1.p.y<0) {
					m1.p.y=0;
				}
			}

			#undef m1
		}
		for (i=0;i<n;i++) {
			#define m1 masses[i]
			m1.p = Vector2Add(m1.p, Vector2Scale(m1.v, dt));
			m1.v0 = m1.v;
			positions[i][(int)(time/dt)] = m1.p;
		}
	}
	time = 0;
	while (!WindowShouldClose())
	{
		time = time + 10*dt;

		BeginDrawing();
		ClearBackground(BLACK);
		if (time < timemax) {
			for(i=0;i<n;i++){
				DrawRectangle(1200*positions[i][(int)(time/dt)].x,1200*positions[i][(int)(time/dt)].y,masses[i].size.x*1200,masses[i].size.y*1200,masses[i].color);
			}
			
		}
		
		// DrawLine(100,100,120,120,BLUE);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

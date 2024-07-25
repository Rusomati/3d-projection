//does not care about optimisation

#include <raylib.h>
#include <stdio.h>
#include <math.h>

const float halfPI = M_PI/2.0;

Vector2 project(Vector3 Pcords, Vector3 Ccords, Vector2 Crot, Vector2 screen, Vector2 fov)
{
	Vector3 Tcords;
	Vector2 res;
	float lenject;
	
	//applying camera "translation"
	Tcords.x = Pcords.x - Ccords.x;
	Tcords.y = Pcords.y - Ccords.y;
	Tcords.z = Pcords.z - Ccords.z;

	//applying camera "rotation"
	Vector2 len = {
		.x=sqrt(Tcords.x*Tcords.x+Tcords.z*Tcords.z),
	};

	Vector2 ang = {
		.x=atan2(Tcords.z, Tcords.x),
	};

	Vector2 rot = {
		.x = ang.x-Crot.x,
	};

	Tcords.x = cos(rot.x) * len.x;
	Tcords.z = sin(rot.x) * len.x;

	len.y = sqrt(Tcords.z*Tcords.z+Tcords.y*Tcords.y);
	
	ang.y = atan2(Tcords.y, Tcords.z);

	rot.y = ang.y-Crot.y;

	Tcords.y = sin(rot.y) * len.y;
	Tcords.z = cos(rot.y) * len.y;

	if (Tcords.z>0){
	//"projection"
	lenject = sqrt(Tcords.x*Tcords.x+Tcords.y*Tcords.y+Tcords.z*Tcords.z);
	res.x = screen.x*asin(Tcords.x/lenject)/fov.x;
	res.y = screen.y*asin(Tcords.y/lenject)/fov.x;

	//raylib "transform"
	res.x += screen.x/2;
	res.y = (screen.y/2)-res.y;

	}
	else{
		res.x = NAN;//nonstandard
		res.y = NAN;
	}
		
	return res;
}

int main()
{
	Vector2 screen = {.x = 768, .y = 432};
    	Vector2 fov = {
		.x = M_PI/6.0,
		.y = fov.x*(float)(screen.y/screen.x)//this is a result of trial and error lol
	};

	Vector3 Ccords = {0,0,0};
	Vector2 Crot = {0,0};

	//verticies of pyramid
	Vector3 Pcords[5] = {
	[0] = {0,0,0},
	[1] = {1,0,0},
	[2] = {0,0,1},
	[3] = {1,0,1},
	[4] = {0.5,1,0.5}
	};

	Vector2 res[5];

	const float speed = 1/60.0;
	const float rotspeed = M_PI/360.0;

	InitWindow((int)screen.x, (int)screen.y, "projection attempt");

	while(!WindowShouldClose())
	{
		if(IsKeyDown(KEY_UP))Crot.y+=rotspeed;
		if(IsKeyDown(KEY_DOWN))Crot.y-=rotspeed;
		if(IsKeyDown(KEY_RIGHT))Crot.x-=rotspeed;
		if(IsKeyDown(KEY_LEFT))Crot.x+=rotspeed;

		//(not normalized)
		if(IsKeyDown(KEY_W)){
			Ccords.z+=speed*sin(Crot.x+halfPI);
			Ccords.x+=speed*cos(Crot.x+halfPI);
		}
		if(IsKeyDown(KEY_S)){
			Ccords.z-=speed*sin(Crot.x+halfPI);
			Ccords.x-=speed*cos(Crot.x+halfPI);
		}
		if(IsKeyDown(KEY_D)){
			Ccords.z+=speed*sin(Crot.x);
			Ccords.x+=speed*cos(Crot.x);
		}
		if(IsKeyDown(KEY_A)){
			Ccords.z-=speed*sin(Crot.x);
			Ccords.x-=speed*cos(Crot.x);
		}
		if(IsKeyDown(KEY_SPACE))Ccords.y+=speed;
		if(IsKeyDown(KEY_LEFT_SHIFT))Ccords.y-=speed;
	
		printf("{%lf, %lf, %lf}, {%lf, %lf}\n",
			Ccords.x, Ccords.y, Ccords.z,
			Crot.x, Crot.y);

		for(int i=0;i<5;i++)
			res[i] = project(Pcords[i], Ccords, Crot, screen, fov);

		BeginDrawing();
		ClearBackground(BLACK);

		DrawTriangleLines(res[0], res[1], res[2], RED);
		DrawTriangleLines(res[1], res[2], res[3], RED);
		DrawLineV(res[1], res[2], BLACK);
		DrawTriangleLines(res[0], res[1], res[4], RED);
		DrawTriangleLines(res[0], res[2], res[4], RED);
		DrawTriangleLines(res[2], res[3], res[4], RED);
		DrawTriangleLines(res[1], res[3], res[4], RED);

		//Debug stuff
		/*
		DrawCircle(screen.x/2+(Ccords.x*5),screen.y/2-(Ccords.z*5), 1, WHITE);
		DrawCircle(screen.x/2+(0.5*5),screen.y/2-(0.5*5), 1, GREEN);
		
		DrawLine(screen.x/2+(Ccords.x*5),screen.y/2-(Ccords.z*5),
			screen.x/2+(Ccords.x*5)+cos(Crot.x+halfPI)*100,
			screen.y/2-(Ccords.z*5)-sin(Crot.x+halfPI)*100, YELLOW);
		DrawLine(screen.x/2+(Ccords.x*5),screen.y/2-(Ccords.z*5),
			screen.x/2+(Ccords.x*5)+cos(Crot.x+halfPI+M_PI/6.0)*100,
			screen.y/2-(Ccords.z*5)-sin(Crot.x+halfPI+M_PI/6.0)*100, YELLOW);
		DrawLine(screen.x/2+(Ccords.x*5),screen.y/2-(Ccords.z*5),
			screen.x/2+(Ccords.x*5)+cos(Crot.x+halfPI-M_PI/6.0)*100,
			screen.y/2-(Ccords.z*5)-sin(Crot.x+halfPI-M_PI/6.0)*100, YELLOW);

		DrawLine(screen.x/2+(Ccords.x*5),screen.y/2-(Ccords.z*5),
			screen.x/2+(Ccords.x*5)-cos(Crot.x+halfPI)*100,
			screen.y/2-(Ccords.z*5)+sin(Crot.x+halfPI)*100, BLUE);
		DrawLine(screen.x/2+(Ccords.x*5),screen.y/2-(Ccords.z*5),
			screen.x/2+(Ccords.x*5)-cos(Crot.x+halfPI+M_PI/6.0)*100,
			screen.y/2-(Ccords.z*5)+sin(Crot.x+halfPI+M_PI/6.0)*100, BLUE);
		DrawLine(screen.x/2+(Ccords.x*5),screen.y/2-(Ccords.z*5),
			screen.x/2+(Ccords.x*5)-cos(Crot.x+halfPI-M_PI/6.0)*100,
			screen.y/2-(Ccords.z*5)+sin(Crot.x+halfPI-M_PI/6.0)*100, BLUE);
		*/

		EndDrawing();
	}

	return 0;
}

/*
Author: Spencer Scott
*/

#ifndef snowballh
#define snowballh
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "enemy.h"

class snowballs
{
public:
	snowballs();
	~snowballs();
	void DrawSnowballs();
	void FireSnowballs(ALLEGRO_BITMAP *gun, double gunAngle, int startX, int startY);
	void UpdateSnowball(int WIDTH, int HEIGHT);
	void CollideSnowball(enemy enemies[], int cSize);
private:
	int x;
	int y;
	int boundx;
	int boundy;
	bool live;
	int speed;
	double radian_angle;
	ALLEGRO_BITMAP *image;
	ALLEGRO_SAMPLE *pew;
	ALLEGRO_SAMPLE *boom;
};

#endif
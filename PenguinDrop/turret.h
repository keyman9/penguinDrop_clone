#ifndef turreth
#define turreth
#include<allegro5\allegro.h>
#include<allegro5\allegro_image.h>
class turret{
public:
	turret(int boardX, int boardY);
	~turret();
	void drawTurret();
	void rotateTurret(float adjust);
	int getBoundX(){ return boundx; }
	int getBoundY(){ return boundy; }
	int getX(){ return x; }
	int getY(){ return y; }
private:
	int x;
	int y;
	int boundx;
	int boundy;
	bool alive;
	int score;
	float angle;
	ALLEGRO_BITMAP *image;
};

#endif
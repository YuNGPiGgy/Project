#ifndef PONG_H
#define PONG_H
#include <string>
#include "websocket.h"
using namespace std;

class Pong{
public:
	Pong(unsigned int width, unsigned int height);
	~Pong();
	void updateBall(double ballX, double ballY, double ballVelX, double ballVelY);
	void updatePaddle(double paddleTop);
	void updateInputs(string inputs);
	void init();
	string getGameState();
	void movePlayer(unsigned int user, unsigned int keyCode, unsigned int latency);
	bool update();
	ostringstream getData();

private:
	bool Intersect(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh);

    struct paddle{
        double top;
        double left;
        double height;
        double width;
    };

	struct ball{
        ball(double x, double y, double velX, double velY, double speed=2, double radius=20){
            this->x = x;
            this->y = y;
            this->v.x = velX;
            this->v.y = velY;
            this->speed = speed;
            this->radius = radius;
        }
		double x;
		double y;
		struct velocity{
			double x;
			double y;
		};
		velocity v;
		double speed = 2;
		double radius = 20;
	};

	struct score{
		unsigned int p1 = 0;
		unsigned int p2 = 0;
		unsigned int p3 = 0;
		unsigned int p4 = 0;
	};

	unsigned int width;
	unsigned int height;
	ball gameBall;
	paddle player1left;
	paddle player2right;
	paddle player3top;
	paddle player4bottom;
score score;
};
#endif
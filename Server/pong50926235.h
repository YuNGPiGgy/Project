#ifndef PONG_H
#define PONG_H
#include <string>
#include "websocket.h"
using namespace std;

class Pong{
public:
	enum PLAYER{ p1, p2, p3, p4 };
	Pong();
	Pong(unsigned int width, unsigned int height);
	~Pong();
	void updateBall(double ballX, double ballY, double ballVelX, double ballVelY);
	void updatePaddle(PLAYER player, int paddleMove);
	void updateInputs(PLAYER player, string inputs);
	void init();
	string getGameState();
	bool update();
	void playerScore(PLAYER player);
	ostringstream getData();
	float randomDirection(float Min, float Max);

private:

    struct paddle{
        int top;
        int left;
        int height;
        int width;
    };

	struct ball{
		ball() {
			this->x = 0;
			this->y = 0;
			this->v.x = 2;
			this->v.y = 2;
			this->speed = 2;
			this->radius = 20;
			this->owner = p1;
		}
        ball(double x, double y, double velX, double velY, PLAYER owner, double speed=2, double radius=20){
            this->x = x;
            this->y = y;
            this->v.x = velX;
            this->v.y = velY;
            this->speed = speed;
            this->radius = radius;
			this->owner = owner;
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
		PLAYER owner;
	};

	struct score{
		unsigned int p1 = 0;
		unsigned int p2 = 0;
		unsigned int p3 = 0;
		unsigned int p4 = 0;
	};

	struct board {
		unsigned int width;
		unsigned int height;
	};


	bool Intersect(paddle paddle, ball ball); //check intersect of any two objects
	unsigned int width;
	unsigned int height;
	ball gameBall;
	paddle player1left;
	paddle player2right;
	paddle player3top;
	paddle player4bottom;
	board gameBoard;
score score;
};
#endif
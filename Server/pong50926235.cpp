//
// Created by domla on 2/15/2018.
//

#include "pong50926235.h"
#include <math.h>
#include <string>
using namespace std;


	Pong::Pong(){
	}
	Pong::Pong(unsigned int width, unsigned int height) {
		this->gameBoard.width = width;
		this->gameBoard.height = height;
	}
	Pong::~Pong(){}

	void Pong::updateBall(double ballX, double ballY, double ballVelX, double ballVelY){
		double newBallXPos = ballX;
		double newBallYPos = ballY;
		double newBallVelX = ballVelX;
		double newBallVelY = ballVelY;

		if (newBallXPos + gameBall.radius > gameBoard.width) {
			gameBall.v.x = -abs(gameBall.v.x);
		}
		if (newBallYPos + gameBall.radius > gameBoard.height) {
			gameBall.v.y = -abs(gameBall.v.y);
		}
		if (newBallXPos < 0) {
			gameBall.v.x = abs(gameBall.v.x);
		}
		if (newBallYPos < 0) {
			gameBall.v.y = abs(gameBall.v.y);
		}
		gameBall.x += gameBall.v.x;
		gameBall.y += gameBall.v.y;

		//Check if the ball has hit a goal
		if (gameBall.x <  gameBall.radius) {
			if (Intersect(p1))
				gameBall.owner = p1;
			else
				playerScore(gameBall.owner);
		}
		else if (gameBall.x + gameBall.radius > gameBoard.width - gameBall.radius ) {
			if (Intersect(p2))
				gameBall.owner = p2;
			else
				playerScore(gameBall.owner);
		}
		else if (gameBall.y < gameBall.radius ) {
			if (Intersect(p4))
				gameBall.owner = p4;
			else
				playerScore(gameBall.owner);
		}
		else if (gameBall.y + gameBall.radius > gameBoard.height - gameBall.radius && !Intersect(p3)) {
			if (Intersect(p3))
				gameBall.owner = p3;
			else
				playerScore(gameBall.owner);
		}
	}

	void Pong::playerScore(PLAYER player) {

	}

	void Pong::updatePaddle(PLAYER player, double paddleTop){}

	void Pong::updateInputs(PLAYER player, string inputs){}

	void Pong::init(){}

	string Pong::getGameState() { return " "; }

	void  Pong::movePlayer(PLAYER player, unsigned int keyCode, unsigned int latency){}

	bool Pong::update() { return false; }

	bool Pong::Intersect(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh) { return false; }
	bool Pong::Intersect(PLAYER player) { return false; }


//
// Created by domla on 2/15/2018.
//

#include "pong50926235.h"
#include <string>
using namespace std;


	Pong::Pong(){
		gameBall = new ball(0,0,0,0);
	}
	void Pong::updateBall(double ballX, double ballY, double ballVelX, double ballVelY){
		gameBall->x = ballX;
		gameBall->y = ballY;
		gameBall->v.x = ballVelX;
		gameBall->v.y = ballVelY;
	}


	void Pong::updatePaddle(double paddleTop){}
	void Pong::updateInputs(string inputs){}
	void Pong::init(){}
	string Pong::getGameState(){}
	void  Pong::movePlayer(unsigned int user, unsigned int keyCode, unsigned int latency){}
	bool Pong::update(){}


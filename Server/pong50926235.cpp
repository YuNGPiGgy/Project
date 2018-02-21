//
// Created by domla on 2/15/2018.
//

#include "pong50926235.h"
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
		gameBall.x = ballX;
		gameBall.y = ballY;
		gameBall.v.x = ballVelX;
		gameBall.v.y = ballVelY;
	}


	void Pong::updatePaddle(PLAYER player, double paddleTop){}

	void Pong::updateInputs(PLAYER player, string inputs){}

	void Pong::init(){}

	string Pong::getGameState() { return " "; }

	void  Pong::movePlayer(PLAYER player, unsigned int keyCode, unsigned int latency){}

	bool Pong::update() { return false; }


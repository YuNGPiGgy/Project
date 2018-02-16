//
// Created by domla on 2/15/2018.
//

#include "pong50926235.h"
#include <string>
using namespace std;


	pong50926235::Pong(unsigned int width, unsigned int height){

	}
	void pong50926235::updateBall(double ballX, double ballY, double ballVelX, double ballVelY){
		this.gameBall.x = ballX;
		this.gameBall.y = ballY;
		this.gameBall.v.x = ballVelX;
		this.gameBall.v.y = ballVelY;
	}


	void pong50926235::updatePaddle(double paddleTop){}
	void pong50926235::updateInputs(string inputs){}
	void pong50926235::init(){}
	string pong50926235::getGameState(){}
	void  pong50926235::movePlayer(unsigned int user, unsigned int keyCode, unsigned int latency){}
	bool pong50926235::update(){}
	ostringstream pong50926235::getData(){}

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
			if (Intersect(player1left, gameBall))
				gameBall.owner = p1;
			else
				playerScore(gameBall.owner);
		}
		else if (gameBall.x + gameBall.radius > gameBoard.width - gameBall.radius ) {
			if (Intersect(player2right, gameBall))
				gameBall.owner = p2;
			else
				playerScore(gameBall.owner);
		}
		else if (gameBall.y < gameBall.radius ) {
			if (Intersect(player4bottom, gameBall))
				gameBall.owner = p4;
			else
				playerScore(gameBall.owner);
		}
		else if (gameBall.y + gameBall.radius > gameBoard.height - gameBall.radius) {
			if (Intersect(player3top, gameBall))
				gameBall.owner = p3;
			else
				playerScore(gameBall.owner);
		}
	}

	float Pong::randomDirection(float Min, float Max)
	{
		return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
	}

	void Pong::playerScore(PLAYER player) {
		//reset ball to center screen and choose random direction
		gameBall.x = this->width / 2;
		gameBall.y = this->height / 2;
		gameBall.v.y = randomDirection(gameBall.speed * -1, gameBall.speed);
		gameBall.v.x = randomDirection(gameBall.speed * -1, gameBall.speed);

		// player scores 
		switch (player) {
		case p1: score.p1++;
			gameBall.owner = p1;
			break;
		case p2: score.p2++;
			gameBall.owner = p2;
			break;
		case p3: score.p3++;
			gameBall.owner = p3;
			break;
		case p4: score.p4++;
			gameBall.owner = p4;
			break;					
		}
	}

	void Pong::updatePaddle(PLAYER player, int paddleMove){
		switch (player) {
		case p1: if (paddleMove == 1)
						this->player1left.top--;
					else if (paddleMove == 3)
						this->player1left.top++;
			break;
		case p2: if (paddleMove == 1)
						this->player2right.top--;
					else if (paddleMove == 3)
						this->player2right.top++;
			break;
		case p3: if (paddleMove == 0)
						this->player3top.left--;
					else if (paddleMove == 2)
						this->player3top.left++;
			break;
		case p4: if (paddleMove == 0)
						this->player4bottom.left--;
					else if (paddleMove == 2)
						this->player4bottom.left++;
			break;
		}
	}

	void Pong::updateInputs(PLAYER player, string inputs){
		int input = -1;
		for (int i = 0; i < inputs.length(); i++) {
			switch (inputs.at(i)) {
			case 'l':
			case 'L': input = 0; //LEFT
				break; 
			case 'u':
			case 'U': input = 1; //UP
				break;
			case 'r':
			case 'R': input = 2; //RIGHT
				break;
			case 'd': 
			case 'D': input = 3; //DOWN
				break;
			default: input = -1;
			}
			if (input != -1) {
				updatePaddle(player, input);
			}
		}
	}

	void Pong::init(){}

	/***************************************************************
		Send gamestate as string in format (without spaces):
		"ballPosX | ballPosY | ballDirX | ballDirY | paddle1top | paddle2top | paddle3left | paddle4left"
		where:
		paddle1 is p1(left)
		paddle2 is p2(right)
		paddle3 is p3(top)
		paddle4 is p4(bottom)
	****************************************************************/
	string Pong::getGameState() 
	{ 
		string returnString = ""; 
		returnString += gameBall.x + '|' + gameBall.y + '|' + gameBall.v.x + '|' + gameBall.v.y + '|' + player1left.top + '|' + player2right.top + '|' + player3top.left + '|' + player4bottom.left;
		return returnString; 
	}

	bool Pong::update() { return false; }

	
	bool Pong::Intersect(paddle paddle, ball ball) {
		return (ball.x < paddle.left + paddle.width && ball.y - ball.radius < paddle.top && ball.y > paddle.top - paddle.height) ||		//intersection on left 
					(ball.x + ball.radius > paddle.left && ball.y - ball.radius < paddle.top && ball.y > paddle.top - paddle.height) ||	//intersection on right
					(ball.y > paddle.top - paddle.height && ball.x + ball.radius > paddle.left && ball.x < paddle.left + paddle.width) ||  //intersection at top
					(ball.y - ball.radius < paddle.top && ball.x + ball.radius > paddle.left && ball.x < paddle.left + paddle.width);		//intersection at bottom
	}



//
// Created by domla on 2/15/2018.
//

#include "pong50926235.h"
#include <math.h>
#include <string>
using namespace std;


	Pong::Pong(){
		init();
	}
	Pong::Pong(unsigned int width, unsigned int height) {
		this->gameBoard.width = width;
		this->gameBoard.height = height;
		init();
	}
	Pong::~Pong(){}

	void Pong::updateBall(int ballX, int ballY, int ballVelX, int ballVelY){
		int newBallXPos = ballX;
		int newBallYPos = ballY;
		int newBallVelX = ballVelX;
		int newBallVelY = ballVelY;

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
		case p1: if (paddleMove == -1)
						this->player1left.top--;
					else if (paddleMove == 1)
						this->player1left.top++;
			break;
		case p2: if (paddleMove == -1)
						this->player2right.top--;
					else if (paddleMove == 1)
						this->player2right.top++;
			break;
		case p3: if (paddleMove == -1)
						this->player3top.left--;
					else if (paddleMove == 1)
						this->player3top.left++;
			break;
		case p4: if (paddleMove == -1)
						this->player4bottom.left--;
					else if (paddleMove == 1)
						this->player4bottom.left++;
			break;
		}
	}

	void Pong::updateInputs(PLAYER player, string inputs){
		int input = 0;
		for (int i = 0; i < inputs.length(); i++) {
			switch (inputs.at(i)) {
			case 'l':
			case 'L': input = -1; //LEFT
				break; 
			case 'u':
			case 'U': input = 1; //UP
				break;
			case 'r':
			case 'R': input = 1; //RIGHT
				break;
			case 'd': 
			case 'D': input = -1; //DOWN
				break;
			default: input = 0;
			}
			if (input != 0) {
				updatePaddle(player, input);
			}
		}
	}

	void Pong::init(){
		gameBall.owner = p1;
		gameBall.x = width / 2;
		gameBall.y = height / 2;
		gameBall.v.x = randomDirection(-2, 2);
		gameBall.v.y = randomDirection(-2, 2);
		player1left.top = 0;
		player1left.left = 0;
		player1left.height = 200;
		player1left.width = 10;
		player2right.top = 0;
		player2right.left = width - 10;
		player2right.height = 200;
		player2right.width = 10;
		player3top.top = 0;
		player3top.left = 0;
		player3top.height = 10;
		player3top.width = 200;
		player4bottom.top = height - 10;
		player4bottom.left = 0;
		player4bottom.height = 10;
		player4bottom.width = 200;
		score.p1 = 0;
		score.p2 = 0;
		score.p3 = 0;
		score.p4 = 0;
	}

	void Pong::pause() {
		init();
		gameBall.v.x = 0;
		gameBall.v.y = 0;
	}

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
		updateBall(gameBall.x, gameBall.y, gameBall.v.x, gameBall.v.y);
		if (player1left.buttonDownMovement != 0)
			updatePaddle(p1, player1left.buttonDownMovement);
		if (player2right.buttonDownMovement != 0)
			updatePaddle(p2, player2right.buttonDownMovement);
		if (player3top.buttonDownMovement != 0)
			updatePaddle(p3, player3top.buttonDownMovement);
		if (player4bottom.buttonDownMovement != 0)
			updatePaddle(p4, player4bottom.buttonDownMovement);

		string returnString = ""; 
		returnString += to_string(gameBall.x)			+ '|' + 
								to_string(gameBall.y)			+ '|' + 
								to_string(gameBall.v.x)			+ '|' + 
								to_string(gameBall.v.y)			+ '|' + 
								to_string(player1left.top)		+ '|' + 
								to_string(player2right.top)	+ '|' + 
								to_string(player3top.left)		+ '|' + 
								to_string(player4bottom.left);
		return returnString; 
	}

	
	bool Pong::Intersect(paddle paddle, ball ball) {
		return (ball.x < paddle.left + paddle.width && ball.y - ball.radius < paddle.top && ball.y > paddle.top - paddle.height) ||		//intersection on left 
					(ball.x + ball.radius > paddle.left && ball.y - ball.radius < paddle.top && ball.y > paddle.top - paddle.height) ||	//intersection on right
					(ball.y > paddle.top - paddle.height && ball.x + ball.radius > paddle.left && ball.x < paddle.left + paddle.width) ||  //intersection at top
					(ball.y - ball.radius < paddle.top && ball.x + ball.radius > paddle.left && ball.x < paddle.left + paddle.width);		//intersection at bottom
	}



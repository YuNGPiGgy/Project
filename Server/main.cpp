#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include <windows.h>
#include <process.h>
#include "pong50926235.h"
#include <queue>
#define INTERVAL_MS 10

using namespace std;

long artificialLatency(long timestamp, int type, int min, int max);

/*********************************************

				HELPER CLASSES 

**********************************************/
struct player {
	int clientID;
	string username;
};

class input {
public:
	int playerNum;
	string inputChar;
	long time;
	input(int p, string inputChar, long time) {
		this->playerNum = p;
		this->inputChar = inputChar;
		this->time = time;
	}
};


class Compare
{
public:
	bool operator() (input input1, input input2)
	{
		return input1.time < input2.time;
	}
};

/**************************************************************

						Globals

***************************************************************/
webSocket server;
map<int, string> clientID_username_map;
Pong pong(600, 800);
int playerCount = 0;
priority_queue<input, vector<input>, Compare> inputTimeQueue;

void parseStringUpdatePacket(int clientID, string message);
vector<string> split(string toSplit);

player * players;

/* called when a client connects */
void openHandler(int clientID){
	int player_num = -1;
	for (int i = 0; i < 4; ++i) {
		if (clientID == players[i].clientID)
			player_num = i;
	}
	if (player_num == -1) {
		if (playerCount > 4)
			return;
		else if (playerCount == 4)
			pong.init();
		else {
			players[playerCount].clientID = clientID;
			player_num = playerCount++;
		}
	}

    server.wsSend(clientID, to_string(player_num));
}


/* called when a client disconnects */
void closeHandler(int clientID){
	pong.pause();    

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID )
            server.wsSend(clientIDs[i], "pause");
    }
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){	
    parseStringUpdatePacket(clientID, message);
}

/* called once per select() loop */
int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;

void runInputQueue() {
	int size = inputTimeQueue.size();
	for (int i = 0; i < size; i++) {
		input in = inputTimeQueue.top();
		pong.updateInputs(static_cast<Pong::PLAYER>(in.playerNum), in.inputChar);
		inputTimeQueue.pop();
	}
}

void periodicHandler(){
    static clock_t next = clock() + interval_clocks;
    clock_t current = clock();
    if (current >= next){
		runInputQueue();
        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++)
            server.wsSend(clientIDs[i], pong.getGameState());

        next = clock() + 10;
    }
}

/***********************************************************************
 *  Parses strings in the format (without spaces):
 *  " username | ballPosX | ballPosY | ballDirX | ballDirY | paddleTop | paddleLeft | INPUT_KEYS_STRING"
 *  the INPUT_KEYS_STRING will be a string of key characters (i.e. w or W or s or S) since last update packet
 *
 *  will send input data to pong object for simulation through methods:
 *  updateBall(double, double, double, double)
 *  updatePaddle(double)
 *  updateInputs(string)
 *
 ***********************************************************************/
void parseStringUpdatePacket(int clientID, string message){
    vector<string> tokens = split(message);

	int player_num = -1;
	for (int i = 0; i < 4; ++i) {
		if (clientID == players[i].clientID)
			player_num = i;
	}
	if (player_num == -1) {
		if (playerCount >= 4)
			return;
		else {
			players[playerCount].clientID = clientID;
			players[playerCount].username = tokens[0];
			player_num = playerCount++;
		}
	}
	Pong::PLAYER player = static_cast<Pong::PLAYER>(player_num);
	if (tokens.size() >= 3) {
		long timestamp = 0;
		timestamp = std::stol(tokens[2]);
		timestamp = artificialLatency(timestamp, 0 /*fixed*/, 0, 0); //0 =fixed, 1=random, 2=incremental (min, max) for incremental
		inputTimeQueue.push(input(player_num, tokens[1], timestamp));
	}
}

/***********************************************************************
 * Split string into tokenized vector<string>
 ***********************************************************************/
vector<string> split(string toSplit){
    stringstream ss(toSplit);
    string item;
    vector<string> tokens;
    while(getline(ss, item, '|')){
        tokens.push_back(item);
    }
    return tokens;
}


/************************************************************************

	Artificial latency 
	types:
	0 = fixed		(does not use min and max)
	1 = random		(uses min and max)
	2 = incremental (uses min and max)
*************************************************************************/
int FIXED_LATENCY = 15;
int incrementalLatStep = 0;

long artificialLatency(long timestamp, int type, int min, int max) {
	long toReturn = timestamp;
	switch (type) {
	case 0: 
		toReturn += FIXED_LATENCY;
		break;

	case 1: 
		toReturn += rand() % max + min;
		break;

	case 2: 
		if (incrementalLatStep + 1 + min > max)
			incrementalLatStep = 0;
		toReturn += min + incrementalLatStep++;
	}

	return toReturn;
}

int main(int argc, char *argv[])
{
	players = new player[4];

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
    server.setPeriodicHandler(periodicHandler);


    /* start the chatroom server, listen to ip '127.0.0.1' and ports '8000'-'8003' */
    int ports[] = { 8000, 8001, 8002, 8003 };

	server.startServer(ports);

    return 1;
}

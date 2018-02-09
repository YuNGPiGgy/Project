#ifndef PONG_H
#define PONG_H
#include <string>
using namespace std;

class Pong{
public:
	Pong(unsigned int width, unsigned int height);
	~Pong();
	void init();
	void movePlayer(unsigned int user, unsigned int keyCode, unsigned int latency);
	bool update();
	ostringstream getData();

private:
	bool Intersect(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh);

	unsigned int width;
	unsigned int height;

	struct ball{
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

	player p1;
	player p2;
	player p3;
	player p4;
	ball b;
	score s;
};
#endif
#include <time.h>
#include <conio.h>
#include <iostream>
using namespace std;
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };

class cBall {
private:
	int x, y;
	int originalX, originalY;
	eDir direction;
public:
	cBall(int posX, int posY) {
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		direction = STOP;
	}
	void Reset() {
		x = originalX; y = originalY;
		direction = STOP;
	}
	void changeDirection(eDir d){
		direction = d;
	}
	inline int getX() {
		return x;
	}
	inline int getY() {
		return y;
	}
	inline eDir getDirection() {
		return direction;
	}
	void randomDirection() {
		direction = (eDir)((rand() % 6) + 1);
	}
	void Move() {
		switch (direction) {
		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UPLEFT:
			x--;
			y--;
			break;
		case DOWNLEFT:
			x--;
			y++;
			break;
		case UPRIGHT:
			x++;
			y--;
			break;
		case DOWNRIGHT:
			x++;
			y++;
			break;
		default:
			break;
		}
	}
	friend ostream & operator<<(ostream & o, cBall c) {
		o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]";
		return o;
	}

};

class cPaddle {
private:
	int x, y;
	int originalX, originalY;
public:
	cPaddle() {
		x = y = 0;
	}
	cPaddle(int posX, int posY) : cPaddle() {
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void Reset() { x = originalX; y = originalY; }
	inline int getY() { return y; }
	inline int getX() { return x; }
	inline void moveUp() { y--; }
	inline void moveDown() { y++; }
	friend ostream& operator<<(ostream& o, cPaddle c) {
		o << "Paddle [" << c.x << "," << c.y << "]";
		return o;
	}
};
class cGameManager {
private:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	cBall* ball;
	cPaddle* player1;
	cPaddle* player2;
public:
	cGameManager(int h, int w) {
		srand(time(NULL));
		quit = false;
		up1 = 'w'; up2 = 'i'; 
		down1 = 's' down2 = 'k';
		score1 = score2 = 0;
		width = w; height = h;
		ball = new cBall(w / 2, h / 2);
		player1 = new cPaddle(1, h / 2 - 3);
		player2 = new cPaddle(w-2, h / 2 - 3);
	}
	//cGameManager Destructor - required to prevent memory leakage
	~cGameManager() {
		delete ball, player1, player2;
	}

	void ScoreUp(cPaddle* player) {
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;

		ball->Reset();
		player1->Reset();
		player2->Reset();
	}
};

int main()
{
	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

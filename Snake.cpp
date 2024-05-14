#include "Map.h"
#include <vector>
#include <iostream>
#include <Windows.h>


class SnakeHead {
public:
	int x;
	int y;
	char head = '@';


	SnakeHead(int startX, int startY) : x(startX), y(startY) {}

	void move(int dx, int dy) {
		x += dx;
		y += dy;
	}

	void teleportMove(int dx, int dy) {
		x = dx;
		y = dy;
	}
};

class SnakeBody {
public:
	int x;
	int y;
	char body = 'O';

	SnakeBody() : x(0), y(0) {}
	SnakeBody(int startX, int startY) : x(startX), y(startY) {}

	void move(int dx, int dy) {
		x = dx;
		y = dy;
	}
};


class Snake {
public:
	enum snakeDirection {
		LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, STOP = 4
	};

	// �� ó���� �������� ���� ���·� ����..
	snakeDirection currDir = STOP;
	SnakeHead head;
	std::vector<SnakeBody> body;
	bool isDie = false;
	int tempTailX, tempTailY;

	int snakeSize;

	Snake(int startX, int startY) : head(startX, startY) {
		// ���� �� ó�� ũ��
		snakeSize = 3;

		// ���� ũ�� 3���� ����..
		body.resize(2);

		// ���� �� ó�� ������ �� �������� ���� ���� ���̹Ƿ�
		// x ��ǥ ��ġ�� -i ���ش�..
		body[0] = { startX - 1, startY };
		body[1] = { startX - 2, startY };


		tempTailX = body.back().x;
		tempTailY = body.back().y;
	}


	int moveX = 0;
	int moveY = 0;
	int gateCount = 0;


public:
	void tick(int tick) {
		Sleep(tick);
	}

	void ReSetSnake() {
		snakeSize = 3;
		gateCount = 0;
		body.resize(2);

		tempTailX = body.back().x;
		tempTailY = body.back().y;
	}

	void gotoxy(int x, int y) {
		COORD pos = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}

	void draw() {
		gotoxy(head.x, head.y);
		std::cout << head.head;

		for (int i = 0; i < snakeSize - 1; i++) {
			gotoxy(body[i].x, body[i].y);
			std::cout << body[i].body;
		}
	}

	void removeTail() {
		gotoxy(tempTailX, tempTailY);
		std::cout << ' ';
	}

	void removeTail(int x, int y) {
		gotoxy(x, y);
		std::cout << ' ';
	}

	void setPos(snakeDirection dir) {
		currDir = dir;

		switch (dir) {
		case LEFT:
			if (currDir == RIGHT) {
				isDie = true;
				return;
			}
			else {
				moveX = -1;
				moveY = 0;
			}
			break;

		case RIGHT:
			if (currDir == LEFT) {
				isDie = true;
				return;
			}
			else {
				moveX = 1;
				moveY = 0;
			}
			break;

		case UP:
			if (currDir == DOWN) {
				isDie = true;
				return;
			}
			else {
				moveX = 0;
				moveY = -1;
			}
			break;

		case DOWN:
			if (currDir == UP) {
				isDie = true;
				return;
			}
			else {
				moveX = 0;
				moveY = 1;
			}
			break;
		}
	}

	void move(int x, int y) {
		tempTailX = body.back().x;
		tempTailY = body.back().y;


		// head ���� �����̱�..
		int prevX = head.x;
		int prevY = head.y;
		head.move(x, y);

		for (int i = 0; i < snakeSize - 1; i++) {
			int tempX = body[i].x;
			int tempY = body[i].y;

			body[i].move(prevX, prevY);
			prevX = tempX;
			prevY = tempY;
		}

		removeTail();
		draw();

	}

	// �� �Լ��� �� �Ӹ��� ����Ʈ�� ������ ����� ��..
	void teleportMove(int x, int y) {
		tempTailX = body.back().x;
		tempTailY = body.back().y;

		// head ���� �����̱�..
		int prevX = head.x;
		int prevY = head.y;
		head.teleportMove(x, y);

		for (int i = 0; i < snakeSize - 1; i++) {
			int tempX = body[i].x;
			int tempY = body[i].y;

			body[i].move(prevX, prevY);
			prevX = tempX;
			prevY = tempY;
		}

		removeTail();
		draw();
	}

	bool snakeTouchWall() {
		if (head.x <= 0 || head.x >= Map::MAPWIDTH || head.y <= 0 || head.y >= Map::MAPHEIGHT) {
			isDie = true;
			return true;
		}
		return false;
	}

	void snakeTouchFreshApple() {
		snakeSize += 1;

		// �������� �þ�� �ϹǷ� ���� �̵� ����� �ݴ�Ǵ� ������ �þ��
		// ���� ���, ���� �̵� ������ LEFT �� moveX = -1, moveY = 0 �� ��, -moveX �ؼ� +1, -moveY �ؼ� 0
		// �̰� �� ������ �ٵ��� x, y �� �����ָ� �� �������� ������ �ϳ� ����..
		body.emplace_back(body.back().x - moveX, body.back().y - moveY);
	}
	void snakeTouchGoldApple() {
		snakeSize += 2;

		// �������� �þ�� �ϹǷ� ���� �̵� ����� �ݴ�Ǵ� ������ �þ��
		// ���� ���, ���� �̵� ������ LEFT �� moveX = -1, moveY = 0 �� ��, -moveX �ؼ� +1, -moveY �ؼ� 0
		// �̰� �� ������ �ٵ��� x, y �� �����ָ� �� �������� ������ �ϳ� ����..
		body.emplace_back(body.back().x - moveX, body.back().y - moveY);
		body.emplace_back(body.back().x - moveX, body.back().y - moveY);
	}

	void snakeTouchPoisonApple() {
		snakeSize -= 1;

		// �� ������ ������ ��ġ�� ����س���
		SnakeBody temp = body.back();
		// �� ������ ������ ��ġ�� ���� ������ �����
		removeTail(temp.x, temp.y);
		// �� ������ ������ �߶�..
		body.pop_back();
	}
	// ������ �Ӹ��� ���� ��ġ ����, ���� ���� �ݴ�� ���� (���� ����x, �Ӹ� ����� �ݴ�)
	void snakeTouchChangeApple(int x, int y, Snake* snake) {
		
		
		tempTailX = body.back().x;
		tempTailY = body.back().y;

		// head ���� �����̱�..
		int prevX = head.x;
		int prevY = head.y;
		head.teleportMove(tempTailX, tempTailY);
		body.back().x = prevX;
		body.back().y = prevY;
		

		if (currDir == RIGHT) {
			(*snake).setPos(Snake::LEFT);
		}
		else if (currDir == LEFT) {
			(*snake).setPos(Snake::RIGHT);
		}
		else if (currDir == UP) {
			(*snake).setPos(Snake::DOWN);
		}
		else if (currDir == DOWN) {
			(*snake).setPos(Snake::UP);
		}
		
		

	}

	// gate1 �� ���� ���� ����Ʈ, gate2 �� ���� ������ ����Ʈ, snakeMoveX �� snakeMoveY �� ���� ���� �̵��ϴ� ����
	void snakeTouchGate(Gate* gate1, Gate* gate2, int snakeMoveX, int snakeMoveY, Snake* snake) {
		gateCount += 1;

		// ����Ʈ�� ��ġ�� �����س���.. �̸� �� ���ǹ����� ���� ������Ʈ(���� �Ӹ��� �̵��� ���� ���� ������..)
		// �׷� ���� �� ������ ���� ������ �ݺ����� ���鼭 �̿��� ��(*���� ���� ������ �κ��� ��ġ�� �� ������ �������� �ݺ��� �����ϴ� ������ �̿�..)
		int tempGate2PosX = (*gate2).gatePosX;
		int tempGate2PosY = (*gate2).gatePosY;

		// �� isSnakeMoved ������ true �� ������ ����Ʈ ��ġ�� ���� ������Ʈ �Ǹ� �ȵ�..
		(*gate1).isSnakeMoved = true;
		(*gate2).isSnakeMoved = true;

		// ���� ���� ����Ʈ�� �������� �������� ��, ���� ��������� ���������� ���� ���� �̵������� �ݴ���..
		// ��, ���� ���� �̵� ������ LEFT ���, currSnakeInputDir �� ���� �ݴ��� RIGHT..
		if (snakeMoveX == -1 && snakeMoveY == 0)
			(*gate1).currSnakeInputDir = Gate::RIGHT;
		else if (snakeMoveX == 1 && snakeMoveY == 0)
			(*gate1).currSnakeInputDir = Gate::LEFT;
		else if (snakeMoveX == 0 && snakeMoveY == -1)
			(*gate1).currSnakeInputDir = Gate::DOWN;
		else if (snakeMoveX == 0 && snakeMoveY == 1)
			(*gate1).currSnakeInputDir = Gate::UP;


		
		// ���� gate1 ���� �����Ƿ�.. gate2 �� ���;���..
		// �׷��� ��ȹ���δ� snakeHead Ŭ������ teleportMove ��� �Լ��� ���� �������� �ƿ� ��ġ�� �ű⵵�� �ؾ��� ��!
		// snake Ŭ�������� teleportMove �Լ� ���� ���� ȣ���� ��..
		if ((*gate2).isEdgeGate) {
			// ���� ������ ����Ʈ�� �� �����ڸ��� ���� ��..
			// ���� �Ӹ��� ���� �̵��ؾ� �ϳĸ�.. gate2 �� ��ġ �ٷ� �� ��ġ.. (���� gate2 �� �ϴ� �����ڸ��� ������ �� ��ġ �ٷ� ��, ���� �����ڸ��� �� ��ġ �ٷ� ������)

			if ((*gate2).gatePosX == 0) {
				// ���� ������ ����Ʈ�� ���� �����ڸ��� ������ ���� �̵������� RIGHT �� �ٲ������..
				(*snake).setPos(Snake::RIGHT);
				(*snake).teleportMove(1, (*gate2).gatePosY);

				tempGate2PosX += 1;
			}
			else if ((*gate2).gatePosX == 45) {
				// ���� ������ ����Ʈ�� ���� �����ڸ��� ������ ���� �̵������� LEFT �� �ٲ������..
				(*snake).setPos(Snake::LEFT);
				(*snake).teleportMove(44, (*gate2).gatePosY);

				tempGate2PosX -= 1;
			}
			else if ((*gate2).gatePosY == 0) {
				// ���� ������ ����Ʈ�� ��� �����ڸ��� ������ ���� �̵������� DOWN ���� �ٲ������..
				(*snake).setPos(Snake::DOWN);
				(*snake).teleportMove((*gate2).gatePosX, 1);

				tempGate2PosY += 1;
			}
			else if ((*gate2).gatePosY == 22) {
				// ���� ������ ����Ʈ�� �ϴ� �����ڸ��� ������ ���� �̵������� UP ���� �ٲ������..
				(*snake).setPos(Snake::UP);
				(*snake).teleportMove((*gate2).gatePosX, 21);

				tempGate2PosY -= 1;
			}
		}
		else {
			// �� �۵��ϴ��� Ȯ���ϱ� ���� �ӽ� �ڵ�..
			/*std::cout << "�� ���ο� ������ ����Ʈ�� �־ƾ�!!!" << "\n";
			Sleep(10000);*/


			// ���� ������ ����Ʈ�� �� �����ڸ��� �ƴ� �� ���ο� ���� ��..
			// �̶� ��μ� ���� ���� ����Ʈ(==gate1)�� currSnakeInputDir ������ ����� ����!!
			
			if ((*gate2).verticalBlocked == false && (*gate2).horizontalBlocked == false) {
				// ���� ���� ������ �����ο� ���..
				// ���� ���� ����������� ����..
				// ��, ���⼱ snake �� setPos �Լ��� ȣ���� �ʿ䰡 ����..
				// �׳� snake �� teleportMove �Լ��� ȣ���ϸ� ��..

				// ���� ���� �̵� ������ �������� ���·� ���� ����Ʈ�� ���� ��
				// ���� ���� ������ ����Ʈ(==gate2) �� �ڽ��� �̵� ������ ���� ������ �����ž���..
				// ��, �̵� ������ �������̹Ƿ� ���� moveX=1, moveY=0 ��.
				// ��, �̸� ������ ����Ʈ�� ��ġ�� �����ָ�, ������ ����Ʈ ��ġ �ٷ� �����ʿ� ���� ������!!!
			}
			else if ((*gate2).verticalBlocked == true && (*gate2).horizontalBlocked == false) {
				// ���� ������ ����Ʈ�� ���Ʒ��� ����������
				// ���� ���� ���� ������ �¿���..
				
				switch ((*gate1).currSnakeInputDir) {
					// ���� ���� ����Ʈ�� ���� �Ǵ� ���������� ������ ���� ���� �̵� ������ �״�� �����ϸ� �ǹǷ� ���� case �� ������ �ʿ� ����..
					// ���� ������ �����ϱ�..
				case Gate::UP:
					// ���� ���� ���� ����Ʈ�� ���ʿ��� ���� �����ϸ�, ������ ����Ʈ�� �������� �����ؾ���..
					// �� ��쿡�� ���� �̵������� �������� �����������..
					
					(*snake).setPos(Snake::LEFT);

					break;
				case Gate::DOWN:
					// ���� ���� ���� ����Ʈ�� �Ʒ��ʿ��� ���� �����ϸ�, ������ ����Ʈ�� ���������� �����ؾ���..
					// �� ��쿡�� ���� �̵������� ���������� �����������..

					(*snake).setPos(Snake::RIGHT);

					break;
				}
			}
			else if ((*gate2).verticalBlocked == false && (*gate2).horizontalBlocked == true) {
				// ���� ������ ����Ʈ�� �翷�� ����������
				// ���� ���� ���� ������ ������..

				switch ((*gate1).currSnakeInputDir) {
					// ���� ���� ����Ʈ�� ���� �Ǵ� ���������� ������ ���� ���� �̵� ������ �״�� �����ϸ� �ǹǷ� ���� case �� ������ �ʿ� ����..
					// ���� ������ �����ϱ�..
				case Gate::LEFT:
					// ���� ���� ���� ����Ʈ�� ���ʿ��� ���� �����ϸ�, ������ ����Ʈ�� �Ʒ������� �����ؾ���..
					// ���� �̵������� �Ʒ������� �����������..

					(*snake).setPos(Snake::DOWN);

					break;
				case Gate::RIGHT:
					// ���� ���� ���� ����Ʈ�� �����ʿ��� ���� �����ϸ�, ������ ����Ʈ�� �������� �����ؾ���..
					// ���� �̵������� �������� �����������..

					(*snake).setPos(Snake::UP);

					break;
				}
			}

			// �� ������ ���� �κ��̶� ������ ���� �� ��..
			(*snake).teleportMove((*gate2).gatePosX + (*snake).moveX, (*gate2).gatePosY + (*snake).moveY);

			tempGate2PosX += (*snake).moveX;
			tempGate2PosY += (*snake).moveY;
		}


		while (true) {
			// ���� ������ ������ ����Ʈ �ٷ� ���� ��ġ�� ������ ������ ��ȯ�� ������..
			if ((*snake).body.back().x == tempGate2PosX && (*snake).body.back().y == tempGate2PosY) {
				(*gate1).isSnakeMoved = false;
				(*gate2).isSnakeMoved = false;
				break;
			}

			(*snake).move((*snake).moveX, (*snake).moveY);
			Sleep(150);
		}
		Sleep(300);
	}
};
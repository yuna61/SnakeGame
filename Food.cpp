#include "Map.h"
#include <random>

class Food {
public:
	// foodId:3 -> �̽��ѻ��, foodId:4 -> ���� ��� foodId: 5 -> Ȳ�ݻ�� foodId:6 --> �ݴ�� ���
	int foodId;

	int foodPosX;
	int foodPosY;

	int prevPosX;
	int prevPosY;

public:
	Food() {
		foodPosX = 4;
		foodPosY = 4;
		foodId = 3;
		prevPosX = foodPosX;
		prevPosY = foodPosY;
	}

	Food(int x, int y)
		: foodPosX{ 4 }, foodPosY{ 4 }, foodId{ 3 }
	{
		prevPosX = foodPosX;
		prevPosY = foodPosY;
	}

	void setFoodInfo(int x, int y, int id) {
		// �̰� ȣ�� ������ ���� ��ġ�� �������� �������� ��..

		// ��.. Ǫ�� ���̵�� �̽��� ����� ���� ����� �������� ���� �� �ֵ��� ���� �Լ� ����ؾ� �� ���ϴ�..
		// ��ġ�� ��������..

		foodId = id;

		foodPosX = x;
		foodPosY = y;
	}
};
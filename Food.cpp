#include "Map.h"
#include <random>

class Food {
public:
	// foodId:3 -> 싱싱한사과, foodId:4 -> 썩은 사과 foodId: 5 -> 황금사과 foodId:6 --> 반대로 사과
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
		// 이게 호출 됐으면 이전 위치의 아이템은 없어져야 함..

		// 음.. 푸드 아이디는 싱싱한 사과랑 썩은 사과가 랜덤으로 나올 수 있도록 랜덤 함수 사용해야 할 듯하다..
		// 위치도 랜덤으로..

		foodId = id;

		foodPosX = x;
		foodPosY = y;
	}
};
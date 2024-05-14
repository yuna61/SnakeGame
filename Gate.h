#pragma once

#include "GateEmergeEnablePos.h"
#include <vector>


class Gate {
public:
	Gate(int x, int y);

	// 이 함수는 map 클래스의 changeMapState 함수가 실행된 후에 실행되는 함수
	// 즉, main.cpp 파일에서 changeMapState 함수 호출한 바로 다음에 호출해야 할 듯.
	// 호출할 때 매개변수로 main.cpp 에서 생성해놓은 map 인스턴스의 map 배열을 던져줘야 함..
	void setGateEmergeVec(int map[][23][46], int mapLevel);

	// 이 함수는 main.cpp 에서 호출하는 함수..
	// 게이트 한 쌍의 위치가 업데이트 되면, 게이트가 가지는 정보도 업데이트가 필요하므로..
	void setGateInfo(int map[][23][46], int mapLevel, Gate* gate);

public:
	int gatePosX;
	int gatePosY;

	// 이전 위치의 게이트를 삭제하기 위한 변수..
	int prevGatePosX;
	int prevGatePosY;

	std::vector<GateEmergeEnablePos> gateEmergeVec;

	enum snakeInputDir {
		LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3
	};

	// 스네이크가 게이트에 수직 방향으로만 접근할 수 있는 상태
	bool horizontalBlocked;

	// 스네이크가 게이트에 수평 방향으로만 접근할 수 있는 상태
	bool verticalBlocked;

	// 스네이크가 게이트에 들어왔는지 여부 확인용 변수
	bool isSnakeMoved;

	// 게이트가 가장자리에 있는지 여부 확인용 변수
	// 이 변수는 뱀이 들어가는 게이트일 땐 안 쓰고, 뱀이 나가는 게이트일 때 쓸 것..
	bool isEdgeGate;

	// 뱀이 어느 게이트의 사분면 중 어느 쪽으로 들어가는지
	// 현재 뱀의 이동 방향의 반대임(뱀의 이동 방향 업데이트 될 때마다 이 변수도 업데이트 필요..)
	// (뱀이 오른쪽으로 이동하고 있을 땐, 게이트의 사분면 중 왼쪽으로만 들어갈 수 있으므로..)
	snakeInputDir currSnakeInputDir;
};
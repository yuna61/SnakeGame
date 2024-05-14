#pragma once

#include "GateEmergeEnablePos.h"
#include <vector>


class Gate {
public:
	Gate(int x, int y);

	// �� �Լ��� map Ŭ������ changeMapState �Լ��� ����� �Ŀ� ����Ǵ� �Լ�
	// ��, main.cpp ���Ͽ��� changeMapState �Լ� ȣ���� �ٷ� ������ ȣ���ؾ� �� ��.
	// ȣ���� �� �Ű������� main.cpp ���� �����س��� map �ν��Ͻ��� map �迭�� ������� ��..
	void setGateEmergeVec(int map[][23][46], int mapLevel);

	// �� �Լ��� main.cpp ���� ȣ���ϴ� �Լ�..
	// ����Ʈ �� ���� ��ġ�� ������Ʈ �Ǹ�, ����Ʈ�� ������ ������ ������Ʈ�� �ʿ��ϹǷ�..
	void setGateInfo(int map[][23][46], int mapLevel, Gate* gate);

public:
	int gatePosX;
	int gatePosY;

	// ���� ��ġ�� ����Ʈ�� �����ϱ� ���� ����..
	int prevGatePosX;
	int prevGatePosY;

	std::vector<GateEmergeEnablePos> gateEmergeVec;

	enum snakeInputDir {
		LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3
	};

	// ������ũ�� ����Ʈ�� ���� �������θ� ������ �� �ִ� ����
	bool horizontalBlocked;

	// ������ũ�� ����Ʈ�� ���� �������θ� ������ �� �ִ� ����
	bool verticalBlocked;

	// ������ũ�� ����Ʈ�� ���Դ��� ���� Ȯ�ο� ����
	bool isSnakeMoved;

	// ����Ʈ�� �����ڸ��� �ִ��� ���� Ȯ�ο� ����
	// �� ������ ���� ���� ����Ʈ�� �� �� ����, ���� ������ ����Ʈ�� �� �� ��..
	bool isEdgeGate;

	// ���� ��� ����Ʈ�� ��и� �� ��� ������ ������
	// ���� ���� �̵� ������ �ݴ���(���� �̵� ���� ������Ʈ �� ������ �� ������ ������Ʈ �ʿ�..)
	// (���� ���������� �̵��ϰ� ���� ��, ����Ʈ�� ��и� �� �������θ� �� �� �����Ƿ�..)
	snakeInputDir currSnakeInputDir;
};
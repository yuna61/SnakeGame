#include "Gate.h"
#include <vector>



Gate::Gate(int x, int y)
    : gatePosX(x), gatePosY(y), gateEmergeVec{}
{
    prevGatePosX = gatePosX;
    prevGatePosY = gatePosY;
}

void Gate::setGateInfo(int map[][23][46], int mapLevel, Gate* gate) {
    // �ϴ� �⺻ ������ �� �ʱ���·� ��������(isEdgeGate �� main.cpp ���� ���� �����ϰ� ����.. ���� ���� �Ǵ� �� ���⼭ �������� ���� ������ ��..)
    (*gate).horizontalBlocked = false;
    (*gate).verticalBlocked = false;
    
    // ����Ʈ�� ���Ʒ��� �����ִ���, �翷�� �����ִ��� �Ǵ��� ����..
    if (map[mapLevel][(*gate).gatePosY + 1][(*gate).gatePosX] == 1 || map[mapLevel][(*gate).gatePosY - 1][(*gate).gatePosX] == 1)
        (*gate).verticalBlocked = true;
    if (map[mapLevel][(*gate).gatePosY][(*gate).gatePosX + 1] == 1 || map[mapLevel][(*gate).gatePosY][(*gate).gatePosX - 1] == 1)
        (*gate).horizontalBlocked = true;
}

void Gate::setGateEmergeVec(int map[][23][46], int mapLevel) {
    // ���ο� ���� �ݿ��ؾ� �ϴƷ� ������ ����Ʈ ���� ���� ��ġ �ν��Ͻ��� �� ������..
    gateEmergeVec.clear();

    for (int i = 0; i < 23; i++) {
        for (int j = 0; j < 46; j++) {
            if (map[mapLevel][i][j] == 1) {
                gateEmergeVec.emplace_back(j, i);
            }
        }
    }
}
#include "Gate.h"
#include <vector>



Gate::Gate(int x, int y)
    : gatePosX(x), gatePosY(y), gateEmergeVec{}
{
    prevGatePosX = gatePosX;
    prevGatePosY = gatePosY;
}

void Gate::setGateInfo(int map[][23][46], int mapLevel, Gate* gate) {
    // 일단 기본 정보는 다 초기상태로 돌려놓기(isEdgeGate 는 main.cpp 에서 따로 관리하고 있음.. 수정 여부 판단 후 여기서 관리할지 말지 결정할 것..)
    (*gate).horizontalBlocked = false;
    (*gate).verticalBlocked = false;
    
    // 게이트의 위아래가 막혀있는지, 양옆이 막혀있는지 판단할 것임..
    if (map[mapLevel][(*gate).gatePosY + 1][(*gate).gatePosX] == 1 || map[mapLevel][(*gate).gatePosY - 1][(*gate).gatePosX] == 1)
        (*gate).verticalBlocked = true;
    if (map[mapLevel][(*gate).gatePosY][(*gate).gatePosX + 1] == 1 || map[mapLevel][(*gate).gatePosY][(*gate).gatePosX - 1] == 1)
        (*gate).horizontalBlocked = true;
}

void Gate::setGateEmergeVec(int map[][23][46], int mapLevel) {
    // 새로운 맵을 반영해야 하늠로 기존의 게이트 생성 가능 위치 인스턴스는 다 버리기..
    gateEmergeVec.clear();

    for (int i = 0; i < 23; i++) {
        for (int j = 0; j < 46; j++) {
            if (map[mapLevel][i][j] == 1) {
                gateEmergeVec.emplace_back(j, i);
            }
        }
    }
}
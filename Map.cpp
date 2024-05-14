#include <iostream>
#include <windows.h>
#include "Map.h"
#include "Gate.h"
#include "Food.cpp"


Map::Map() {
    mapLevel = 0;
}

// main ���� �� ó�� ���۽� ȣ��ǰ�..
// �ʿ� ������ ���� ������ ȣ��Ǵ� �Լ�..
void Map::setMap() {
    draw();
}


void Map::gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Map::draw() {
    system("cls");

    // ȭ�� ����� 1�� ��ٸ���..
    Sleep(100);
    
    for (int i = 0; i < MAPHEIGHT; i++)
        {
        for (int j = 0; j < MAPWIDTH; j++)
            {
                if (map[mapLevel][i][j] == 1 || map[mapLevel][i][j] == 2) {
                    printf("��");
                }
                else if (map[mapLevel][i][j] == 3) {
                    printf("��");
                }
                else if (map[mapLevel][i][j] == 4) {
                    printf("��");
                }
                else if (map[mapLevel][i][j] == 7) {
                    printf("��");
                }
                else if (map[mapLevel][i][j] == 5) {
                    printf("��");
                }
                else if (map[mapLevel][i][j] == 6) {
                    printf("��");
                }
                else {
                    printf(" ");
                }
            }
            printf("\n");
        }

    }


void Map::changeMapItemState(int mapLevel, int x, int y, int prevX, int prevY, int itemId) {
    map[mapLevel][prevY][prevX] = 0;

    map[mapLevel][y][x] = itemId;
}

void Map::changeMapGateState(int mapLevel, GateEmergeEnablePos gate1, GateEmergeEnablePos gate2, int gate1PrevPosX, int gate1PrevPosY, int gate2PrevPosX, int gate2PrevPosY) {
    // ����Ʈ ��ġ �� ���ϴ��� Ȯ�ο� �ڵ�..
    /*std::cout << "���� ����Ʈ ��ġ : " << "(" << gate1PrevPosX << ", " << gate1PrevPosY << ")" << "\n";
    std::cout << "���� ����Ʈ ��ġ : " << "(" << gate2PrevPosX << ", " << gate2PrevPosY << ")" << "\n";
    std:Sleep(1000);*/

    map[mapLevel][gate1PrevPosY][gate1PrevPosX] = 1;
    map[mapLevel][gate2PrevPosY][gate2PrevPosX] = 1;

    map[mapLevel][gate1.gateEmergeEnablePosY][gate1.gateEmergeEnablePosX] = 7;
    map[mapLevel][gate2.gateEmergeEnablePosY][gate2.gateEmergeEnablePosX] = 7;
}
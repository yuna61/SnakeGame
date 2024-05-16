#include <Windows.h>
#include <iostream>
#include <vector>
#include <ctime>

#include "GateEmergeEnablePos.h"
#include "Gate.h"
#include "Snake.cpp"
#include "Food.cpp"
#include "UserInfo.cpp"
#include <ctime>
#include "Map.h"



using namespace std;


void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


int main() {
    // �� ����
    Map mapInstance;
    mapInstance.setMap();


    // ������ ���� 
    Food food;

    // ����� ���� ����
    UserInfo Info;

    // �������� ���̵� ���� ����
    random_device randomId;
    mt19937 genItemId(randomId());
    uniform_int_distribution<int> idDis(3, 6);

    // �������� ��ġ ���� ����
    random_device randomPosX;
    random_device randomPosY;
    mt19937 genItemPosX(randomPosX());
    mt19937 genItemPosY(randomPosY());
    uniform_int_distribution<int> posXDis(2, 44);
    uniform_int_distribution<int> posYDis(2, 21);


    // �ð� ���� ����
    clock_t startTime = clock();
    clock_t tempTime{};

    clock_t itemEmergeTime{};
    clock_t gateEmergeTime{};


    // ������ ����
    // �� ó�� ��ġ�� 10, 10 ����..
    Snake snake(10, 10);


    // ����Ʈ ����
    Gate gate1(2, 0);
    Gate gate2(2, 22);
    //GateEmergeEnablePos prevGate1(2, 0);
    //GateEmergeEnablePos prevGate2(2, 23);

    gate1.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);
    gate2.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);

    bool gateMapStarted = false;

    // ����Ʈ ��ġ ���� ����
    random_device randomVecIndex;
    mt19937 genVecIndex(randomVecIndex());
    //uniform_int_distribution<int> vecIndexDis(0);

    // Mission �ʱⰪ
    int BGoal = 4;
    int PlusGoal = 2;
    int MinusGoal = 1;
    int Ggoal = 1;
    int Tick = 150;

    while (true) {
        tempTime = clock();
        Info.setTime(difftime(tempTime, startTime) / 1000.0);

        // �ʴ���
        // ���� �������� 5�� ������ maplevel�� 2�� �ǵ���..
        // ������ ����..(5�� ��������..)
        if (mapInstance.mapLevel == 0 && difftime(tempTime, startTime) / 1000.0 > 5) {
            // ���� �ʷ����� 2�� ��..
            mapInstance.mapLevel++;
            mapInstance.setMap();
        }
        // ���� �������� 40�� ������ maplevel�� 3�� �ǵ���..
        // ����Ʈ ����..
        if (mapInstance.mapLevel == 1 && difftime(tempTime, startTime) / 1000.0 > 10) {
            mapInstance.mapLevel++;
            mapInstance.setMap();
        }

        if ((mapInstance.mapLevel == 1 || mapInstance.mapLevel >= 2) && difftime(tempTime, itemEmergeTime) / 1000.0 > 7) {
            // itemEmergeTime �� tempTime ���� �ٲ������ν�.. ����ؼ� 7���̳��� map �� ���� ����Ʈ �ϵ���..
            itemEmergeTime = tempTime;

            // food �� setFoodInfo() �Լ��� ����ؼ� food �� ���� �ʱ�ȭ..
            // random ���� ������ �� ����� �Լ��� �Ѱ��ֱ�..

            int tempRandomPosX;
            int tempRandomPosY;

            tempRandomPosX = posXDis(randomPosX);
            tempRandomPosY = posYDis(randomPosY);

            
            if (mapInstance.map[mapInstance.mapLevel][tempRandomPosY][tempRandomPosX] == 1) {
                // �������� �־��� ��ġ�� ���� ������ ������ �����Ǹ� �ȵǴϱ�, ���� �ƴ� ������ while �� ���� setFoodInfo �Լ��� �Ѱ��� ����..
                while (true) {
                    if (mapInstance.map[mapInstance.mapLevel][tempRandomPosY][tempRandomPosX] == 0)
                        break;

                    tempRandomPosX = posXDis(randomPosX);
                    tempRandomPosY = posYDis(randomPosY);
                }
            }

            food.setFoodInfo(tempRandomPosX, tempRandomPosY, idDis(randomId));
            mapInstance.changeMapItemState(mapInstance.mapLevel, food.foodPosX, food.foodPosY, food.prevPosX, food.prevPosY, food.foodId);


            // ������ ���� ���� �� �� ���� �� ���� �׸���..
            mapInstance.setMap();


            // �ð��� ������ �ʿ��� ���ֱ� ���� ���� ��ġ�� �����ϱ�..
            food.prevPosX = food.foodPosX;
            food.prevPosY = food.foodPosY;

        }


        if (mapInstance.mapLevel >= 2 && difftime(tempTime, gateEmergeTime) / 1000.0 > 10) {
            // gateTime �� tempTime ���� �ٲ������ν�.. ����ؼ� 10 ���� ���� map �� ���� ����Ʈ �ϵ���..
            gateEmergeTime = tempTime;



            // ���� ����Ʈ�� ����ϰ� ���� ���� ��, ����Ʈ�� ��ġ�� �ٲ��ִ� ������ ������ ����..
            if (!gate1.isSnakeMoved) {
                uniform_int_distribution<int> vecIndexDis(0, gate1.gateEmergeVec.size() - 1);
                int tempGate1Index = vecIndexDis(randomVecIndex);
                int tempGate2Index = vecIndexDis(randomVecIndex);

                // ���� ���E���� �־��� ����Ʈ�� ��ġ�� ������..
                if (tempGate1Index == tempGate2Index) {
                    // ����Ʈ�� ��ġ�� ���� �޶��� ������ tempGate2Index �� ���ο� ���� ���� ���� �Ҵ��� ��..
                    while (true) {
                        tempGate2Index = vecIndexDis(randomVecIndex);

                        // ����Ʈ�� ��ġ�� ���� �޶������Ƿ� break �� ���� while ���� ����������..
                        if (tempGate2Index != tempGate1Index)
                            break;
                    }
                }


                gate1.gatePosX = gate1.gateEmergeVec[tempGate1Index].gateEmergeEnablePosX;
                gate1.gatePosY = gate1.gateEmergeVec[tempGate1Index].gateEmergeEnablePosY;
                gate2.gatePosX = gate2.gateEmergeVec[tempGate2Index].gateEmergeEnablePosX;
                gate2.gatePosY = gate2.gateEmergeVec[tempGate2Index].gateEmergeEnablePosY;


                GateEmergeEnablePos tempGate1{ gate1.gatePosX, gate1.gatePosY };
                GateEmergeEnablePos tempGate2{ gate2.gatePosX, gate2.gatePosY };

                mapInstance.changeMapGateState(mapInstance.mapLevel, tempGate1, tempGate2, gate1.prevGatePosX, gate1.prevGatePosY, gate2.prevGatePosX, gate2.prevGatePosY);


                // ����Ʈ�� ���� ������Ʈ �Ǿ�� �ϹǷ�.. ����Ʈ ���� ���� �Լ��� ���� ���� ȣ���� ��..
                // ����Ʈ�� �翷�� �����ִ���, ���Ʒ��� �����ִ��� �Ǵ��ؼ� ���� ������Ʈ �� ����..
                gate1.setGateInfo(mapInstance.map, mapInstance.mapLevel, &gate1);
                gate2.setGateInfo(mapInstance.map, mapInstance.mapLevel, &gate2);

                if ((gate1.gatePosX == 0 || gate1.gatePosX == 45) || (gate1.gatePosY == 0 || gate1.gatePosY == 22)) {
                    gate1.isEdgeGate = true;
                }
                else {
                    gate1.isEdgeGate = false;
                }

                if ((gate2.gatePosX == 0 || gate2.gatePosX == 45) || (gate2.gatePosY == 0 || gate2.gatePosY == 22)) {
                    gate2.isEdgeGate = true;
                }
                else {
                    gate2.isEdgeGate = false;
                }


                // ���� ����Ʈ�� ������ �� �ִ� ��ġ ���� ������Ʈ..
                gate1.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);
                gate2.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);


                // ����Ʈ ���� ���� �� �� ���� �� ���� �׸���..
                mapInstance.setMap();

            }

            // �ð��� ������ �ʿ��� ���ֱ� ���� ���� ��ġ�� �����ϱ�..
            gate1.prevGatePosX = gate1.gatePosX;
            gate1.prevGatePosY = gate1.gatePosY;
            gate2.prevGatePosX = gate2.gatePosX;
            gate2.prevGatePosY = gate2.gatePosY;
        }



        if (food.foodId == 3 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // ���� �Ӹ��� �̽��ѻ���� �ε�����(== ���� �Ӹ��� �̽��ѻ���� ��ġ�� ������)
            // snake �� snakeTouchApple() �Լ� �����ϱ�..
            snake.snakeTouchFreshApple();

            Info.setCurrentLength(snake.snakeSize);
            Info.setMaxLen(snake.snakeSize);
            Info.IncreaseGetGrowthItem();
        }

        if (food.foodId == 4 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // ���� �Ӹ��� ��������� �ε�����(== ���� �Ӹ��� ��������� ��ġ�� ������)
            // ���� ����� �پ��.
            snake.snakeTouchPoisonApple();

            Info.setCurrentLength(snake.snakeSize);
            Info.IncreaseGetPoisonItem();
        }
        if (food.foodId ==  5 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // ���� �Ӹ��� Ȳ�ݻ���� �ε�����(== ���� �Ӹ��� Ȳ�ݻ���� ��ġ�� ������)
            // ���� ����� +2.
            snake.snakeTouchGoldApple();
            Info.setCurrentLength(snake.snakeSize);
            Info.setMaxLen(snake.snakeSize);
            Info.IncreaseGetGrowthItem();
            
        }
        if (food.foodId == 6 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // ���� �Ӹ��� �ݴ�λ���� �ε�����(== ���� �Ӹ��� �ݴ�λ���� ��ġ�� ������)
            // ���� �Ӹ��� ���� ��ġ �ٲ�. ���ÿ� ���� ������ �ٲ��.
            snake.snakeTouchChangeApple(snake.head.x,snake.head.y,&snake);
        }

        if (snake.snakeSize < 3) {
            snake.isDie = true;
            gotoxy(MAPWIDTH * 2 + 8, 7);
            cout << "Game Over!";

            // ���� ũ�Ⱑ 3���� �۾����� ���� ��~~!
            break;
        }

        // ���� �Ӹ��� ���� ������ �ε����� ���� ����!
        // snake.moveX==0, snake.moveY==0 �� ���ÿ� ������ ���� ���� ���� ���� ����Ű�� ������ ����ä�� �����ִ� ����..
        // �̶��� �Ӹ��� ���̶� ��Ƶ� ������ �ȵǴϱ� ���� ���ǹ� �ɾ��� ��..
        if (snake.moveX == 0 && snake.moveY == 0) {}
        else {
            for (int i = 0; i < snake.snakeSize - 1; i++) {
                if ((snake.head.x == snake.body[i].x) && (snake.head.y == snake.body[i].y)) {
                    snake.isDie = true;
                    gotoxy(MAPWIDTH * 2 + 8, 7);
                    cout << "game Over!";
                    return 0;
                }
            }
        }


        // ���� ���� gate1 ���� �� ��..
        // ���� ���� ���� �̵� ������ LEFT �� snake �� moveX=-1, moveY=0 ��..
        if ((snake.head.y + snake.moveY == gate1.gatePosY) && (snake.head.x + snake.moveX == gate1.gatePosX)) {
            snake.snakeTouchGate(&gate1, &gate2, snake.moveX, snake.moveY, &snake);
            Info.setGateUse(snake.gateCount - 1);
            Info.IncreaseGateCount();

        }
        else if ((snake.head.y + snake.moveY == gate2.gatePosY) && (snake.head.x + snake.moveX == gate2.gatePosX)) {
            snake.snakeTouchGate(&gate2, &gate1, snake.moveX, snake.moveY, &snake);
            Info.setGateUse(snake.gateCount - 1);
            Info.IncreaseGateCount();

        }


        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            snake.setPos(Snake::UP);
        }
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            snake.setPos(Snake::DOWN);
        }
        else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            snake.setPos(Snake::LEFT);
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            snake.setPos(Snake::RIGHT);
        }

        snake.move(snake.moveX, snake.moveY);


        // �� ������ �״°�. ���ǻ� �ϴ� �ּ�ó��
        if (mapInstance.map[mapInstance.mapLevel][snake.head.y][snake.head.x]==1) {
            gotoxy(MAPWIDTH * 2 + 8, 7);
            std::cout << "Game Over" << std::endl;
            break; // ���� ����
        }

        if (snake.isDie) {
            gotoxy(MAPWIDTH * 2 + 8, 7);
            std::cout << "Game Over!" << std::endl;
            break; // ���� ����
        }
        // ���� ���� ���
        Info.ShowInfo();

        //5�ܰ� �߰����. �׳� �������� ���� ����ϱ�
        int stageLevel = mapInstance.mapLevel;
        if (stageLevel == 0 || stageLevel == 1 || stageLevel==2) {
            stageLevel = 1;
        }
        else {
            stageLevel -= 1;
        }

        if (stageLevel != 4) {
            gotoxy(MAPWIDTH * 2 + 8, 15);
            std::cout << "Stage level: " << stageLevel << std::endl;
        }
        else if (stageLevel == 4) {
            gotoxy(MAPWIDTH * 2 + 8, 15);
            std::cout << "Stage level: Last Stage" << std::endl;
        }
        

        // �̼� ���� ���
        Info.Mission(BGoal,PlusGoal,MinusGoal,Ggoal);
        if (Info.IsGoal()) {
            mapInstance.mapLevel++;
            if (mapInstance.mapLevel == 6) {
                gotoxy(MAPWIDTH * 2 + 8, 7);
                std::cout << "Game Clear!" << "\n";
                Sleep(1000);
                break;
            }
            gotoxy(MAPWIDTH * 2 + 8, 7);
            std::cout << "Mission Complete!" << "\n";
            Sleep(100);
            Info.ReSetInfo();
            snake.ReSetSnake();

            BGoal = rand() % 4 + 4;
            PlusGoal = rand() % 4 + 1;
            MinusGoal = rand() % 2 + 1;
            Ggoal = rand() % 2 + 1;
            //�̼� �޼� �� �� �ӵ� ����
            Tick -= 20;
            
            mapInstance.setMap();
        }
        // �� �̵� �ӵ�
        snake.tick(Tick);
    }


    return 0;
}
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
    // 맵 생성
    Map mapInstance;
    mapInstance.setMap();


    // 아이템 생성 
    Food food;

    // 사용자 정보 생성
    UserInfo Info;

    // 아이템의 아이디 랜덤 변수
    random_device randomId;
    mt19937 genItemId(randomId());
    uniform_int_distribution<int> idDis(3, 6);

    // 아이템의 위치 랜덤 변수
    random_device randomPosX;
    random_device randomPosY;
    mt19937 genItemPosX(randomPosX());
    mt19937 genItemPosY(randomPosY());
    uniform_int_distribution<int> posXDis(2, 44);
    uniform_int_distribution<int> posYDis(2, 21);


    // 시간 관련 변수
    clock_t startTime = clock();
    clock_t tempTime{};

    clock_t itemEmergeTime{};
    clock_t gateEmergeTime{};


    // 지렁이 생성
    // 맨 처음 위치는 10, 10 으로..
    Snake snake(10, 10);


    // 게이트 생성
    Gate gate1(2, 0);
    Gate gate2(2, 22);
    //GateEmergeEnablePos prevGate1(2, 0);
    //GateEmergeEnablePos prevGate2(2, 23);

    gate1.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);
    gate2.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);

    bool gateMapStarted = false;

    // 게이트 위치 랜덤 변수
    random_device randomVecIndex;
    mt19937 genVecIndex(randomVecIndex());
    //uniform_int_distribution<int> vecIndexDis(0);

    // Mission 초기값
    int BGoal = 4;
    int PlusGoal = 2;
    int MinusGoal = 1;
    int Ggoal = 1;
    int Tick = 150;

    while (true) {
        tempTime = clock();
        Info.setTime(difftime(tempTime, startTime) / 1000.0);

        // 초단위
        // 게임 시작한지 5초 지나면 maplevel이 2가 되도록..
        // 아이템 등장..(5초 간격으로..)
        if (mapInstance.mapLevel == 0 && difftime(tempTime, startTime) / 1000.0 > 5) {
            // 이제 맵레벨은 2가 됨..
            mapInstance.mapLevel++;
            mapInstance.setMap();
        }
        // 게임 시작한지 40초 지나면 maplevel이 3이 되도록..
        // 게이트 등장..
        if (mapInstance.mapLevel == 1 && difftime(tempTime, startTime) / 1000.0 > 10) {
            mapInstance.mapLevel++;
            mapInstance.setMap();
        }

        if ((mapInstance.mapLevel == 1 || mapInstance.mapLevel >= 2) && difftime(tempTime, itemEmergeTime) / 1000.0 > 7) {
            // itemEmergeTime 을 tempTime 으로 바꿔줌으로써.. 계속해서 7차이나면 map 을 새로 프린트 하도록..
            itemEmergeTime = tempTime;

            // food 의 setFoodInfo() 함수를 사용해서 food 의 정보 초기화..
            // random 으로 임의의 수 만들고 함수로 넘겨주기..

            int tempRandomPosX;
            int tempRandomPosY;

            tempRandomPosX = posXDis(randomPosX);
            tempRandomPosY = posYDis(randomPosY);

            
            if (mapInstance.map[mapInstance.mapLevel][tempRandomPosY][tempRandomPosX] == 1) {
                // 랜덤으로 주어진 위치에 벽이 있으면 아이템 생성되면 안되니까, 벽이 아닐 때까지 while 문 돌고 setFoodInfo 함수에 넘겨줄 것임..
                while (true) {
                    if (mapInstance.map[mapInstance.mapLevel][tempRandomPosY][tempRandomPosX] == 0)
                        break;

                    tempRandomPosX = posXDis(randomPosX);
                    tempRandomPosY = posYDis(randomPosY);
                }
            }

            food.setFoodInfo(tempRandomPosX, tempRandomPosY, idDis(randomId));
            mapInstance.changeMapItemState(mapInstance.mapLevel, food.foodPosX, food.foodPosY, food.prevPosX, food.prevPosY, food.foodId);


            // 아이템 정보 변경 해 준 다음 맵 새로 그리기..
            mapInstance.setMap();


            // 시간이 지나면 맵에서 없애기 위해 이전 위치를 저장하기..
            food.prevPosX = food.foodPosX;
            food.prevPosY = food.foodPosY;

        }


        if (mapInstance.mapLevel >= 2 && difftime(tempTime, gateEmergeTime) / 1000.0 > 10) {
            // gateTime 을 tempTime 으로 바꿔줌으로써.. 계속해서 10 차이 나면 map 을 새로 프린트 하도록..
            gateEmergeTime = tempTime;



            // 뱀이 게이트를 통과하고 있지 않을 때, 게이트의 위치를 바꿔주는 로직을 실행할 것임..
            if (!gate1.isSnakeMoved) {
                uniform_int_distribution<int> vecIndexDis(0, gate1.gateEmergeVec.size() - 1);
                int tempGate1Index = vecIndexDis(randomVecIndex);
                int tempGate2Index = vecIndexDis(randomVecIndex);

                // 만약 랜덭으로 주어진 게이트의 위치가 같으면..
                if (tempGate1Index == tempGate2Index) {
                    // 게이트의 위치가 서로 달라질 때까지 tempGate2Index 에 새로운 랜덤 값을 새로 할당할 것..
                    while (true) {
                        tempGate2Index = vecIndexDis(randomVecIndex);

                        // 게이트의 위치가 서로 달라졌으므로 break 을 통해 while 구문 빠져나가기..
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


                // 게이트가 새로 업데이트 되어야 하므로.. 게이트 정보 세팅 함수를 따로 만들어서 호출할 것..
                // 게이트의 양옆이 막혀있는지, 위아래가 막혀있는지 판단해서 정보 업데이트 할 것임..
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


                // 다음 게이트가 등장할 수 있는 위치 새로 업데이트..
                gate1.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);
                gate2.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);


                // 게이트 정보 변경 해 준 다음 맵 새로 그리기..
                mapInstance.setMap();

            }

            // 시간이 지나면 맵에서 없애기 위해 이전 위치를 저장하기..
            gate1.prevGatePosX = gate1.gatePosX;
            gate1.prevGatePosY = gate1.gatePosY;
            gate2.prevGatePosX = gate2.gatePosX;
            gate2.prevGatePosY = gate2.gatePosY;
        }



        if (food.foodId == 3 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // 뱀의 머리랑 싱싱한사과가 부딪히면(== 뱀의 머리랑 싱싱한사과의 위치가 같으면)
            // snake 의 snakeTouchApple() 함수 실행하기..
            snake.snakeTouchFreshApple();

            Info.setCurrentLength(snake.snakeSize);
            Info.setMaxLen(snake.snakeSize);
            Info.IncreaseGetGrowthItem();
        }

        if (food.foodId == 4 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // 뱀의 머리랑 썩은사과가 부딪히면(== 뱀의 머리랑 썩은사과의 위치가 같으면)
            // 뱀의 사이즈가 줄어듦.
            snake.snakeTouchPoisonApple();

            Info.setCurrentLength(snake.snakeSize);
            Info.IncreaseGetPoisonItem();
        }
        if (food.foodId ==  5 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // 뱀의 머리랑 황금사과랑 부딪히면(== 뱀의 머리랑 황금사과의 위치가 같으면)
            // 뱀의 사이즈가 +2.
            snake.snakeTouchGoldApple();
            Info.setCurrentLength(snake.snakeSize);
            Info.setMaxLen(snake.snakeSize);
            Info.IncreaseGetGrowthItem();
            
        }
        if (food.foodId == 6 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // 뱀의 머리랑 반대로사과랑 부딪히면(== 뱀의 머리랑 반대로사과의 위치가 같으면)
            // 뱀의 머리와 꼬리 위치 바뀜. 동시에 진행 방향이 바뀐다.
            snake.snakeTouchChangeApple(snake.head.x,snake.head.y,&snake);
        }

        if (snake.snakeSize < 3) {
            snake.isDie = true;
            gotoxy(MAPWIDTH * 2 + 8, 7);
            cout << "Game Over!";

            // 뱀의 크기가 3보다 작아지면 게임 끝~~!
            break;
        }

        // 뱀의 머리랑 뱀의 몸통이 부딪히면 게임 종료!
        // snake.moveX==0, snake.moveY==0 이 동시에 만족될 때는 게임 시작 직후 방향키를 누르지 않은채로 멈춰있는 상태..
        // 이때는 머리랑 몸이랑 닿아도 죽으면 안되니까 따로 조건문 걸어준 것..
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


        // 만약 뱀이 gate1 으로 들어갈 때..
        // 만약 현재 뱀의 이동 방향이 LEFT 면 snake 의 moveX=-1, moveY=0 임..
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


        // 벽 닿으면 죽는거. 편의상 일단 주석처리
        if (mapInstance.map[mapInstance.mapLevel][snake.head.y][snake.head.x]==1) {
            gotoxy(MAPWIDTH * 2 + 8, 7);
            std::cout << "Game Over" << std::endl;
            break; // 게임 종료
        }

        if (snake.isDie) {
            gotoxy(MAPWIDTH * 2 + 8, 7);
            std::cout << "Game Over!" << std::endl;
            break; // 게임 종료
        }
        // 유저 정보 출력
        Info.ShowInfo();

        //5단계 추가요소. 그냥 스테이지 레벨 출력하기
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
        

        // 미션 정보 출력
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
            //미션 달성 시 뱀 속도 증가
            Tick -= 20;
            
            mapInstance.setMap();
        }
        // 뱀 이동 속도
        snake.tick(Tick);
    }


    return 0;
}
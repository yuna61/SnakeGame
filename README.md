# Snake Game
### C++ 프로그래밍 언어로 Snake Game을 구현.

------------------------------------------------------------------------------------------------------------------------------------------------------------------


[스네이크 게임 임의 조건]
1. 맵은 46x23 크기로 설정
   (맨 처음 맵, 아이템 맵, 아이템&게이트 맵으로 구분, * 맵은 3차원 배열; map[][][]의 요소는 정수를 가질 것)

   *(map[][][] 이 가질 수 있는 정수 요소: 0, 1, 2, 3, 4, 5)

   *(0: 뱀이 지나다닐 수 있음을 의미)

   *(1: 뱀이 지나다닐 수 없은 벽을 의미)

   *(2: 게이트로 변할 수 없는 벽을 의미)

   *(3: 싱싱한사과를 의미)

   *(4: 썩은사과를 의미)

   *(5: 게이트를 의미)

2. 0.15 초가 지날 때마다 뱀이 움직이도록 함.   
3. 맨 처음 뱀의 이동 방향은 RIGHT(--> 왼쪽방향키를 눌러서 게임을 시작하면 죽음.. 수정 여부 확인 후 조건 바꿀지 말지 결정할 것..)
4. 맨 처음 뱀의 크기는 3 (Snanke 클래스의 SnakeHead 변수 초기화 해주었고, body 라는 이름의 벡터 변수에 몸통 2개를 넣어놓음(==SnakeBody 인스턴스 2개를 넣어놓았다는 의미.))
5. 맨 처음 시작했을 때, 방향키를 누르기 전 까지는 뱀이 움직이지 않도록 Snake 클래스의 moveX, moveY 변수 값을 0으로 설정해놓았음.
6. ■ 는 벽, ★ 은 싱싱한사과, ◎는 썩은사과, ♡는 게이트를 의미함.
7. Food 클래스의 foodId 는 3과 4를 값으로 가질 수 있음(* 3은 싱싱한사과, 4는 썩은 사과).

------------------------------------------------------------------------------------------------------------------------------------------------------------------

[스네이크 게임 프로젝트 내용 설명]

아래 코드는 main.cpp 의 변수 선언 부분을 가져옴.

```
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
uniform_int_distribution<int> idDis(3, 4);

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
```


1. 맨 처음 뱀의 이동 방향은 RIGHT로 설정하였음.
   
2. 즉, 맨 처음 게임을 시작할 때 왼쪽 방향키 누르면 죽음..(이건 수정의 여부 판단 후 바꿀지 말지 결정)
   
3. 맵 레벨이 존재한다. 0, 1, 2 레벨로 이루어져 있음(0Lv:맨 처음으로 주어지는 맵, 1Lv:아이템이 등장하는 맵, 2Lv:아이템과 게이트가 등장하는 맵).
   
4. 게임이 시작한 후 일정 시간이 지나면 맵 레벨이 오름(맵 레벨 1 조건: 게임 플레이 시간 5s, 맵 레벨 2 조건: 게임 플레이 시간 40s).
  
5.  SnakeHead와 SnakeBody 클래스를 따로 만들어서 관리함(SnakeBody 는 Snake 클래스에서 vector를 통해 관리함. SnakeHead 는 Snake 클래스에서 단일 변수로 관리하고 있음.)
   
6.  방향키를 누르면 main.cpp 의 main() 함수에서 snake 클래스의 setPos(snankeDirection dir) 을 호출함. 이때, snakeDirection 은 직접 정의한 enum 타입으로, LEFT=0 RIGHT=1 UP=2 DOWN=3 으로 설정했음. setPos(snakeDirection) 함수는 입력된 방향키에 따라 snake 클래스의 moveX, moveY의 값을 바꾸어 뱀이 이동하도록 하는 역할을 함.

```
// main.cpp 의 방향키에 따른 뱀 이동 방향 설정 코드
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
```

```
// Snake 클래스의 setPos 함수 코드
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
```


8.  setPos(snakeDirection) 함수로 snake 의 이동방향과 moveX, moveY 값을 설정한 후, snake 클래스의 move(int x, int y) 함수를 호출함. snake 클래스의 move 함수는 전체적인 뱀의 이동을 관리하는 함수임.

```
// Snake 클래스의 move 함수 코드
void move(int x, int y) {
	tempTailX = body.back().x;
	tempTailY = body.back().y;


	// head 먼저 움직이기..
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
```

즉 이 함수에서 snakeHead 클래스의 move(int dx, int dy) 함수와 snakeBody 클래스의 move(int dx, int dy) 함수를 호출함. snakeHead 클래스의 move 함수는 snakeHead 의 위치를 전달된 매개변수의 값과 더해 다시 위치 변수에 대입하고, snakeBody 클래스의 move 함수는 body 의 위치 변수에 전달된 매개변수의 값을 대입함. snakeHead 의 위치를 덧셈연산을 수행하여 바꿔주면, snakeBody 는 이동 전 뱀의 머리 위치로 이동하기만 하면 되기 때문에 둘 함수의 행동이 다르다.

```
// SnakeHead 클래스의 move 함수 코드
void move(int dx, int dy) {
	x += dx;
	y += dy;
}
```

```
// SnakeBody 클래스의 move 함수 코드
void move(int dx, int dy) {
	x = dx;
	y = dy;
}
```

  즉, snakeHead 와 snakeBody 클래스를 총괄하는 snake 클래스의 move 함수에서는 prevX, prevY, tempX, tempY 변수를 선언하여 뱀의 전체적인 위치를 뱀의 사이즈만큼 반복문을 돌면서 업데이트 해줌.
  
  snake 클래스의 move 함수 마지막에는 removeTail() 과 draw() 함수가 호출되는데 removeTail 은, 미리 저장해둔 뱀의 꼬리 위치로 이동하여 공백을 출력해서, 뱀이 이동하는 것처럼 보이게 하기 위함이다. draw 함수는 전체적인 뱀의 위치 이동이 끝나면 비로소 호출되어 뱀의 사이즈만큼 반복문을 돌아 커서를 이동시켜 출력하는 것이다. 즉, removeTail() 과 draw() 를 통해서 커서를 계속 움직이면서 뱀을 콘솔 창에 프린트 해줌.

```
// Snake 클래스의 removeTail 함수 코드
void removeTail() {
	gotoxy(tempTailX, tempTailY);
	std::cout << ' ';
}
```

```
// Snake 클래스의 gotoxy 함수 코드
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
```

```
// Snake 클래스의 draw 함수 코드
void draw() {
	gotoxy(head.x, head.y);
	std::cout << head.head;

	for (int i = 0; i < snakeSize - 1; i++) {
		gotoxy(body[i].x, body[i].y);
		std::cout << body[i].body;
	}
}
```

10.  일정 시간이 지나면 맵 레벨이 증가하면서 맵의 형태도 바뀜. 즉 맵의 형태가 바뀔 때마다 콘솔 창에 다시 맵을 프린트 해주어야함. 그 역할을 Map 클래스의 setMap() 함수가 수행함.
   
11.  setMap() 함수에서 Map 클래스의 draw() 함수를 호출함. draw() 함수는 호출 되자마자 바로 콘솔 창에 프린트 되었던 모든 것들을 지움. 지우고 난 후 Sleep(100) 을 통해 텀을 줌. 텀이 지나면 다시 맵이 화면에 출력 됨. switch 로 분기를 나눠서 맵 레벨에 따라 map 배열의 요소를 프린트 하는 로직을 각각 작성함.

```
// Map 클래스의 draw 함수
void Map::draw() {
    system("cls");

    // 화면 지우고 1초 기다리기..
    Sleep(100);

    switch (Map::mapLevel) {
    case 0:
        for (int i = 0; i < MAPHEIGHT; i++)
        {
            for (int j = 0; j < MAPWIDTH; j++)
            {
                if (map[mapLevel][i][j] == 1 || map[mapLevel][i][j] == 2)
                    printf("■");
                else
                    printf(" ");
            }
            printf("\n");
        }

        break;
    case 1:
        for (int i = 0; i < MAPHEIGHT; i++)
        {
            for (int j = 0; j < MAPWIDTH; j++)
            {
                if (map[mapLevel][i][j] == 1 || map[mapLevel][i][j] == 2) {
                    printf("■");
                }
                else if (map[mapLevel][i][j] == 3) {
                    printf("★");
                }
                else if (map[mapLevel][i][j] == 4) {
                    printf("◎");
                }
                else {
                    printf(" ");
                }
            }
            printf("\n");
        }

        break;
    case 2:
        for (int i = 0; i < MAPHEIGHT; i++)
        {
            for (int j = 0; j < MAPWIDTH; j++)
            {
                if (map[mapLevel][i][j] == 1 || map[mapLevel][i][j] == 2) {
                    printf("■");
                }
                else if (map[mapLevel][i][j] == 3) {
                    printf("★");
                }
                else if (map[mapLevel][i][j] == 4) {
                    printf("◎");
                }
                else if (map[mapLevel][i][j] == 5) {
                    printf("♡");
                }
                else {
                    printf(" ");
                }
            }
            printf("\n");
        }

        break;
    }
}
```
    
12.  main.cpp 파일에서는 시간과 관련된 변수들(startTime, tempTime, itemEmergeTime, gateEmergeTime) 을 선언하여 관리함(* 앞 전 main.cpp 에서 선언된 변수 목록 코드에 존재함을 볼 수 있음).
 
13. while(true) 에서 tempTime = clock() 을 통해 계속 현재 시간을 측정함. 즉, 이 현재 시간에 미리 측정해 놓았던 startTime 을 빼서 게임이 시작된 후 흐른 시간을 계산함.

14. itemEmergeTime 변수는 맵 레벨이 1이 된 후부터 값이 대입되기 시작함. 맨 처음에 itemEmergeTime 에 startTime 변수 값을 대입해 놓음. 그 후로 tempTime-itemEmergeTime 이 7s 차이가 날 때마다 itemEmergeTime 변수에 tempTime 변수의 값을 대입함. 그 후, Food 클래스의 setFoodInfo(int x, int y, int id) 함수를 호출하여 food 인스턴스의 foodId, foodPosX, foodPosY 변수 값을 업데이트 함.

```
// main.cpp 파일의 아이템 등장시키는 부분 로직 코드
if ((mapInstance.mapLevel == 1 || mapInstance.mapLevel == 2) && difftime(tempTime, itemEmergeTime) / 1000.0 > 7) {
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
```
    
15. setFoodInfo(int x, int y, int id) 의 매개변수로 랜덤 값을 넣어줘야 함. 즉, 미리 선언해 놓은 random_device 타입의 randomId, randomPosX, randomPosY 변수를 각각 미리 선언한 uniform_int_distribution<int> 타입의 idDis(3, 4), posXDis(2, 44), posYDis(2, 21) 변수에 전달해줌.

즉, food.setFoodInfo(posXDis(randomPosX), posYDis(randomPosY), idDis(randomId)); 구문은 랜덤한 x, y의 위치값과, 랜덤한 아이템 아이디를 매개변수로 전달하여 food 인스턴스의 foodId, foodPosX, foodPosY 변수 값을 업데이트 하는 것임. (* x 값이 가지는 범위는 (2, 44), y 값이 가지는 범위는 (2, 21), id 값이 가지는 범위는 (3, 4)임.)
    
14. setFoodInfo 함수를 통해 food 인스턴스의 변수들이 업데이트 완료되면, 다음으로 map.changeMapState(map.mapLevel, food.foodPosX, food.foodPosY, food.prevPosX, food.prevPosY, food.foodId); 구문을 통해 맵의 상태를 변화시킴. changeMapState(int mapLevel, int x, int y, int prevX, int prevY, int itempId) 함수는 map[mapLevel][prevY][prevX]=0; 구문을 통해 이전 위치에 있던 음식을 없애고, map[mapLevel][y][x]=itemid; 구문을 통해 새로 입력 받은 x, y 위치에 아이템을 놓음.
    
15. 맵의 상태 변화를 마친 후 다음으로 map.setMap(); 구문을 통해 콘솔 창에 맵을 다시 그림. 그 다음으로 food.prevPosX = food.foodPosX; food.prevPosY = food.foodPosY; 구문을 통해 음식의 현재 위치를 이전 위치로 저장함(다음에 changeMapState 함수가 실행될 때, 현재 음식의 위치에 있던 음식을 없애기 위해, 현재 위치를 이전 위치로 저장하는 것!)

16. 게임 진행 도중 뱀의 머리가 싱싱한 사과랑 부딪히면(==뱀의 머리와 싱싱한사과의 위치가 같으면) snake 의 snakeTouchApple() 함수를 호출하여 뱀의 사이즈를 1 늘려주고, body 벡터에 새로운 snakeBody 인스턴스를 생성하여 넣어줌. (* 이때 새로운 인스턴스의 위치는 꼬리쪽이 늘어나는 것이므로 현재 이동 방향과 반대되는 곳으로 늘어나야함. 즉, 현재 이동 방향이 LEFT 라고 예를 들면, moveX=-1, moveY = 0 일 때, -moveX 해서 +1, -moveY 해서 0 이 되고. 이를 새로운 인스턴스를 body 에 넣기 전 body 의 마지막에 있던 인스턴스의 x, y 에 더해주면 맨 마지막에 꼬리가 하나 붙음.)

```
// main.cpp 파일의 싱싱한 사과와 부딪혔을 때 발생하는 이벤트 관련 부분 로직 코드
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

if (snake.snakeSize < 3) {
    snake.isDie = true;
    gotoxy(MAPWIDTH * 2 + 8, 6);
    cout << "Game Over!";

    // 뱀의 크기가 3보다 작아지면 게임 끝~~!
    break;
}
```

```
// Snake 클래스의 snankeTouchFreshApple 함수 코드
void snakeTouchFreshApple() {
	snakeSize += 1;

	// 꼬리쪽이 늘어나야 하므로 현재 이동 방향과 반대되는 곳으로 늘어나기
	// 예를 들면, 현재 이동 방향이 LEFT 로 moveX = -1, moveY = 0 일 때, -moveX 해서 +1, -moveY 해서 0
	// 이걸 맨 마지막 바디의 x, y 에 더해주면 맨 마지막에 꼬리가 하나 붙음..
	body.emplace_back(body.back().x - moveX, body.back().y - moveY);
}

void snakeTouchPoisonApple() {
	snakeSize -= 1;

	// 맨 마지막 꼬리의 위치를 기억해놓고
	SnakeBody temp = body.back();
	// 맨 마지막 꼬리의 위치로 가서 꼬리를 지우고
	removeTail(temp.x, temp.y);
	// 맨 마지막 꼬리를 잘라냄..
	body.pop_back();
}
```
    
17. 게임 진행 도중 뱀의 머리가 썩은 사과랑 부딪히면(== 뱀의 머리와 썩은 사과의 위치가 같으면) 뱀의 사이즈가 -1 됨. 즉, snake 클래스의 snakeTouchPoisonApple() 함수를 호출하여 뱀의 사이즈를 1만큼 줄이고, 맨 마지막 꼬리 위치로 가서 공백을 출력하여 꼬리가 잘린 것처럼 보이도록 함. 최종적으로 body.pop_back(); 구문을 통해 실질적으로 꼬리가 잘리게 됨

```
// main.cpp 파일의 썩은은 사과와 부딪혔을 때 발생하는 이벤트 관련 부분 로직 코드
if (food.foodId == 4 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
    // 뱀의 머리랑 썩은사과가 부딪히면(== 뱀의 머리랑 썩은사과의 위치가 같으면)
    // 뱀의 사이즈가 줄어듦.
    snake.snakeTouchPoisonApple();

    Info.setCurrentLength(snake.snakeSize);
    Info.IncreaseGetPoisonItem();
}
```

```
// Snake 클래스의 snakeTouchPoisionApple 함수 코드
void snakeTouchPoisonApple() {
	snakeSize -= 1;

	// 맨 마지막 꼬리의 위치를 기억해놓고
	SnakeBody temp = body.back();
	// 맨 마지막 꼬리의 위치로 가서 꼬리를 지우고
	removeTail(temp.x, temp.y);
	// 맨 마지막 꼬리를 잘라냄..
	body.pop_back();
}
```
    
18. 만약 뱀의 사이즈가 3보다 작아지면 snake.isDie = true; 구문을 통해 while 문을 벗어나서 게임을 종료함. 

```
// main.cpp 파일의 뱀의 사이즈가 3보다 작아졌을 경우의 이벤트 처리하는 부분 코드
if (snake.snakeSize < 3) {
    snake.isDie = true;
    gotoxy(MAPWIDTH * 2 + 8, 6);
    cout << "Game Over!";

    // 뱀의 크기가 3보다 작아지면 게임 끝~~!
    break;
}
```
   
19. 게임 진행 도중 뱀의 머리랑 뱀의 몸통이 부딪히면 게임을 종료해야함. 그러나 snake.moveX==0, snake.moveY==0 이 동시에 만족될 때(==게임 시작 직후 방향키를 누르기 전 상태) 에는 머리랑 몸이 닿아도 죽으면 안 되므로 따로 조건문을 걸어주어야 함. (즉, 0,0 을 제외한 경우에서는 뱀의 사이즈-1 만큼 반복문을 돌면서 뱀의 머리와 뱀의 몸의 위치가 같은지를 판단 할 것임. 만약 같다면 snake.isDie=true; 구문을 통해 while 문을 빠져나와 게임을 종료함.)

```
// main.cpp 파일의 뱀의 머리랑 뱀의 몸통이 부딪힌 경우 발생하는 이벤트를 처리하는 코코드
if (snake.moveX == 0 && snake.moveY == 0) {}
else {
    for (int i = 0; i < snake.snakeSize - 1; i++) {
        if ((snake.head.x == snake.body[i].x) && (snake.head.y == snake.body[i].y)) {
            snake.isDie = true;
            gotoxy(MAPWIDTH * 2 + 8, 6);
            cout << "game Over!";
            return 0;
        }
    }
}
```
    
20. 게임 진행 도중 뱀의 머리가 벽과 부딪히면 게임을 종료해야함. 조건문을 통해 뱀의 머리가 벽과 닿았는지의 여부를 확인함. 만약 닿았다면 isDie=true; 구문을 통해 while 문을 빠져나와 게임을 종료함.

```
// main.cpp 파일의 뱀의 머리가 벽과 부딪힌 경우 발생하는 이벤트 처리하는 부분 코드
if (mapInstance.map[mapInstance.mapLevel][snake.head.y][snake.head.x]==1) {
    gotoxy(MAPWIDTH * 2 + 8, 6);
    std::cout << "Game Over" << std::endl;
    break; // 게임 종료
}
```

21. main.cpp 파일에서 gate 인스턴스를 2개 만들어서 관리함(각각 이름은 gate1, gate2) --> 이도 맨 처음 main.cpp 파일에 선언된 변수 목록 보이는 코드에 있는 걸 볼 수 있음.

22. Gate 클래스가 필요로 하는 변수는 horizontalBlocked, verticalBlocked, isSnakeMoved, isEdgeGate, currSnakeInputDir, gateEmergeVec 임.

23. horizontalBlocked, verticalBlocked 변수는 bool 타입 변수로 스네이크가 게이트에 수직 방향으로만 접근 할 수 있는지, 수평 방향으로만 접근 할 수 있는지의 여부를 나타내기 위한 변수임.

24. isSnakeMoved 변수는 bool 타입 변수로 뱀이 게이트에 들어왔는지 여부를 확인하기 위한 변수임. isEdgeGate 변수도 bool 타입 변수로 게이트가 가장자리에 있는지 여부를 확인하기 위한 변수임.

25. currSnakeInputDir 변수는 사용자 정의 타입인 snakeInputDir 타입 변수(LEFT=0, RIGHT=1, UP=2, DOWN=3) 로 뱀이 게이트의 사분면 중 어느 쪽으로 들어가는지를 확인하기 위한 변수임(* 현재 뱀의 이동방향의 반대임. 즉, 뱀이 오른쪽으로 이동하고 있을 때 이 변수는 LEFT 값을 가짐. --> 뱀이 오른쪽으로 이동하고 있을 땐, 게이트의 사분면 중 왼쪽으로만 들어갈 수 있기 때문!)

26. gateEmergeVec 은 GateEmergeEnablePos 라는 클래스를 통해 만들어진 인스턴스를 저장하는 벡터 변수임.

```
// Gate 클래스 코드
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
```

27. GateEmergeEnablePos 클래스는 gateEmergeEnablePosX, gateEmergeEnablePosY 라는 변수를 가짐.

```
// GateEmergeEnablePos 클래스 코드
class GateEmergeEnablePos {
public:
	GateEmergeEnablePos(int x, int y);

public:
	int gateEmergeEnablePosX;
	int gateEmergeEnablePosY;
};
```

28. Gate 클래스가 필요로 하는 함수는 setGateInfo 와 setGateEmergeVec 임

29. setGateEmergeVec(int map[][23][46], int mapLevel) 함수는 main.cpp 파일에서 map 클래스의 changeMapState 함수가 호출 된 다음으로 호출되는 함수임. 함수로 진입하면 gateEmergeVec 벡터 변수에 들어있던 인스턴스들을 gateEmergeVec.clear(); 라는 구문을 통해 다 지워버림(새로운 맵을 반영해야 하므로 기존의 게이트 생성 가능 위치 인스턴스를 다 버리는 것..)

30. 그 후 맵 사이즈 만큼 2차 반복문을 돌면서 map[mapLevel][i][j] 의 값이 1이면 gateEmergeVec.emplace_back(j, i); 구문을 통해 새로운 인스턴스를 생성해 다시 벡터 변수에 넣음(* 1은 벽을 뜻한다고 앞에서 언급함, 게이트는 벽에만 생성될 수 있으므로 조건문에서 map의 요소가 1인지 판단한 것!).

```
// Gate 클래스의 setGateEmergeVec 함수 코드
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
```

31. setGateInfo(int map[][23][46], int mapLevel, Gate* gate) 함수는 main.cpp 파일에서 호출되는 함수임. main.cpp 파일에서 changeMapGateState 함수가 호출된 후에 호출하는 함수임(--> 게이트가 새로 업데이트 되었으므로, 게이트가 가지는 정보도 업데이트 해줘야 하기 때문! 이 함수에서는 게이트의 양옆이 막혀있는지, 위아래가 막혀있는지 판단해서 변수를 업데이트 함!).

```
// Gate 클래스의 setGateInfo 함수 코드
void Gate::setGateInfo(int map[][23][46], int mapLevel, Gate* gate) {
    (*gate).horizontalBlocked = false;
    (*gate).verticalBlocked = false;
    
    // 게이트의 위아래가 막혀있는지, 양옆이 막혀있는지 판단할 것임..
    if (map[mapLevel][(*gate).gatePosY + 1][(*gate).gatePosX] == 1 || map[mapLevel][(*gate).gatePosY - 1][(*gate).gatePosX] == 1)
        (*gate).verticalBlocked = true;
    if (map[mapLevel][(*gate).gatePosY][(*gate).gatePosX + 1] == 1 || map[mapLevel][(*gate).gatePosY][(*gate).gatePosX - 1] == 1)
        (*gate).horizontalBlocked = true;
}
```


32. setGateInfo 함수가 호출되자마자 (*gate).horizontalBlocked = false; (*gate).verticalBlocked = false; 구문이 실행되는데, 이는 기본 정보를 다 초기 상태로 돌려놓기 위함임(isEdgeGate 변수는 main.cpp 에서 따로 업데이트 해주기 때문에 이 함수에서는 신경쓰지 않음).

33. 조건문을 통해 게이트 y 위치 위 또는 아래가 1이면 (*gate).verticalBlocked=true; 구문으로, 게이트 x 위치 왼쪽 또는 오른쪽이 1이면 (*gate).horizontalBlocked=true; 구문으로 상태를 업데이트 해줄 것임.

34. main.cpp 파일에서 setGateInfo 함수가 호출 된 다음 조건문을 통해 gate 가 맵 가장자리에 있는지 판단함(* 코드는 아래와 같음.)

```
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
```

35. 그 다음 main.cpp 파일에서 게이트 클래스의 setGateEmergeVec 함수를 호출하여 다음 게이트가 등장할 수 있는 위치를 새로 업데이트 해줌. 업데이트 후 mapInstance 의 setMap() 함수를 호출하여 맵을 새로 그림.

36. 시간이 지나면 맵에서 이전 게이트를 없애야 하므로 현재 게이트의 위치를 이전 위치를 저장하는 변수에 대입하여 저장함(gate1.prevGatePosX=gate1.gatePosX; gate1.prevGatePosY=gate1.gatePosY; 와 같은 구문이 이를 수행함.)

37.
현재 뱀의 위치에 뱀의 이동방향을 더한 값이 게이트의 위치와 같으면 실행될 수 있도록 조건문을 작성함. 조건이 만족되면 snakeTouchGate 함수가 호출됨.

```
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
```

38. snakeTouchGate 함수는 Snake.cpp 파일에 정의되어 있음. 함수의 원형은 void snakeTouchGate(Gate* gate1, Gate* gate2, int snakeMoveX, int snakeMoveY, Snake* snake) 임.

39. gate1 은 뱀이 들어가는 게이트, gate2 는 뱀이 나가는 게이트, snakeMoveX, snakeMoveY 는 현재 뱀이 이동하는 방향을 의미함.

40. 함수가 시작되면, 게이트의 위치를 임시로 저장해놓는 변수를 선언 및 초기화함(이 변수는 조건문에서 뱀의 머리가 이동한 곳과 값이 같도록 값을 업데이틑 할 필요가 있음.)

```
int tempGate2PosX = (*gate2).gatePosX;
int tempGate2PosY = (*gate2).gatePosY;
```

41. snakeTouchGate 함수에서는 게이트 인스턴스의 isSnakeMoved 변수의 값을 true 로 바꿔줌(isSnakeMoved 변수는 main.cpp 에서 조건문과 함께 쓰이면서 게이트의 위치를 바꾸는 구문을 수행할지 말지를 결정하는 역할을 함. 이 변수의 값이 true 일 때에는 게이트 위치가새로 업데이트 되면 안 됨.)

42. 게이트의 어느 쪽에서 뱀이 들어오는지를 확인하여 게이트 인스턴스의 currSnakeInputDir 변수에 값을 대입해야함(currSnakeInputDir 은 Gate 클래스의 snakeInputDir enum 타입임. LEFT=0, RIGHT=1, UP=2, DOWN=3 으로 설정해놨음)

43. 뱀이 들어가는 게이트를 기준으로 생각했을 때, 뱀이 어느쪽으로 들어오는지는 현재 뱀의 이동방향을 반전시킴으로써 얻을 수 있음. 즉, 현재 뱀의 이동방향이 LEFT 라면, currSnakeInputDir 는 그의 반대인 RIGHT 임. 뱀의 이동 방향이 LEFT 라는 것은 snakeMoveX=-1, snakeMoveY=0 이라는 뜻과 같음. 즉, 조건문에 진입해서 뱀이 들어가는 게이트의 currSnakeInputDir 변수 값을 RIGHT 로 업데이트 해줌.

```
if (snakeMoveX == -1 && snakeMoveY == 0)
	(*gate1).currSnakeInputDir = Gate::RIGHT;
else if (snakeMoveX == 1 && snakeMoveY == 0)
	(*gate1).currSnakeInputDir = Gate::LEFT;
else if (snakeMoveX == 0 && snakeMoveY == -1)
	(*gate1).currSnakeInputDir = Gate::DOWN;
else if (snakeMoveX == 0 && snakeMoveY == 1)
	(*gate1).currSnakeInputDir = Gate::UP;
```

 44. snakeTouchGate 함수에서 매개변수로 주어지는 gate1 은 뱀이 들어가는 게이트, gate2 는 뱀이 나가는 게이트를 의미함을 앞전에 설명했다. 즉, 뱀이 gate1 으로 들어갔으므로, gate2 로 나와야 함.

45. 뱀이 나가는 게이트가 맵의 가장자리에 있는 게이트인지 아닌지를 조건문으로 판별하여, 두 경우를 나눠서 로직을 작성함. 다음은 뱀이 나가는 게이트가 맵의 가장자리에 있는 게이트일 경우의 로직임(* 로직 설명은 제시되는 코드 밑에 있음)

```
    if ((*gate2).isEdgeGate) {
	// 뱀이 나가는 게이트가 맵 가장자리에 있을 때..
	// 뱀의 머리가 어디로 이동해야 하냐면.. gate2 의 위치 바로 전 위치.. (만약 gate2 가 하단 가장자리에 있으면 그 위치 바로 위, 좌측 가장자리면 그 위치 바로 오른쪽)

	if ((*gate2).gatePosX == 0) {
		// 뱀이 나가는 게이트가 좌측 가장자리에 있으면 뱀의 이동방향은 RIGHT 로 바꿔줘야함..
		(*snake).setPos(Snake::RIGHT);
		(*snake).teleportMove(1, (*gate2).gatePosY);

		tempGate2PosX += 1;
	}
	else if ((*gate2).gatePosX == 45) {
		// 뱀이 나가는 게이트가 우측 가장자리에 있으면 뱀의 이동방향은 LEFT 로 바꿔줘야함..
		(*snake).setPos(Snake::LEFT);
		(*snake).teleportMove(44, (*gate2).gatePosY);

		tempGate2PosX -= 1;
	}
	else if ((*gate2).gatePosY == 0) {
		// 뱀이 나가는 게이트가 상단 가장자리에 있으면 뱀의 이동방향은 DOWN 으로 바꿔줘야함..
		(*snake).setPos(Snake::DOWN);
		(*snake).teleportMove((*gate2).gatePosX, 1);

		tempGate2PosY += 1;
	}
	else if ((*gate2).gatePosY == 22) {
		// 뱀이 나가는 게이트가 하단 가장자리에 있으면 뱀의 이동방향은 UP 으로 바꿔줘야함..
		(*snake).setPos(Snake::UP);
		(*snake).teleportMove((*gate2).gatePosX, 21);

		tempGate2PosY -= 1;
	}
}
```

* 로직에서는 setPos 와 teleportMove 라는 함수가 사용됨. 즉, 로직을 설명하기 위해서는 함수의 설명이 우선되어야함. setPos 는 앞전에 방향키를 받아서 뱀의 방향을 정할 때 소개했으므로 생략하고 teleportMove 를 설명하겠음. 

46. teleportMove 함수는 Snake 클래스가 가지는 함수임. 이 함수는 뱀 머리랑 게이트가 만나면 실행됨. 기존 Snake 클래스의 move 함수와 전체적인 로직은같으나 쓰이는 함수가 다름. void teleportMove(int x, int y) 함수 속에는 head.teleportMove(x, y); 라는 구문이 존재함. 이는 SnakeHead 클래스의 teleportMove(int dx, int dy) 함수를 호출 하는 것.

47. SnakeHead 클래스의 teleportMove 함수는 기존의 move 함수와는 다르게 뱀 머리의 위치 변수에 직접 매개변수의 값을 대입함(move 함수는 뱀 머리의 위치 변수에 매개변수의 값을 더하는 식으로 작동했음.)

48. 즉, 함수가 실행되면 Snake 인스턴스가 가지는 SnakeHead 클래스의 head 인스턴스(뱀 머리) 의 위치 변수의 값이, 입력 받은 위치로 업데이트 됨.

49. 뱀 머리의 위치가 변하면, 뱀 몸체의 위치도 바뀌어야 하는데 이는 기존 move 함수와 동일한 로직을 사용함. prevX, prevY 에 뱀 머리가 움직이기 전의 위치를 저장해놓고, 뱀 몸체 사이즈만큼 반복문을 돌면서, tmepX, tempY 라는 변수를 이용해 순차적으로 뱀 몸체의 위치를 바꿔줌. teleportMove 함수는 다음과 같음.

```
void teleportMove(int x, int y) {
	tempTailX = body.back().x;
	tempTailY = body.back().y;

	// head 먼저 움직이기..
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
```

50. 이제 다시 Snake 의 snakeTouchGate 함수로 돌아가서 설명을 이을 것임. 함수를 다시 보면 다음과 같음(*뱀이 나가는 게이트가 맵 가장자리에 있는 경우를 먼저 설명하고 있는 중).

```
if ((*gate2).isEdgeGate) {
	// 뱀이 나가는 게이트가 맵 가장자리에 있을 때..
	// 뱀의 머리가 어디로 이동해야 하냐면.. gate2 의 위치 바로 전 위치.. (만약 gate2 가 하단 가장자리에 있으면 그 위치 바로 위, 좌측 가장자리면 그 위치 바로 오른쪽)

	if ((*gate2).gatePosX == 0) {
		// 뱀이 나가는 게이트가 좌측 가장자리에 있으면 뱀의 이동방향은 RIGHT 로 바꿔줘야함..
		(*snake).setPos(Snake::RIGHT);
		(*snake).teleportMove(1, (*gate2).gatePosY);

		tempGate2PosX += 1;
	}
	else if ((*gate2).gatePosX == 45) {
		// 뱀이 나가는 게이트가 우측 가장자리에 있으면 뱀의 이동방향은 LEFT 로 바꿔줘야함..
		(*snake).setPos(Snake::LEFT);
		(*snake).teleportMove(44, (*gate2).gatePosY);

		tempGate2PosX -= 1;
	}
	else if ((*gate2).gatePosY == 0) {
		// 뱀이 나가는 게이트가 상단 가장자리에 있으면 뱀의 이동방향은 DOWN 으로 바꿔줘야함..
		(*snake).setPos(Snake::DOWN);
		(*snake).teleportMove((*gate2).gatePosX, 1);

		tempGate2PosY += 1;
	}
	else if ((*gate2).gatePosY == 22) {
		// 뱀이 나가는 게이트가 하단 가장자리에 있으면 뱀의 이동방향은 UP 으로 바꿔줘야함..
		(*snake).setPos(Snake::UP);
		(*snake).teleportMove((*gate2).gatePosX, 21);

		tempGate2PosY -= 1;
	}
}
```

뱀이 나가는 게이트가 맵 가장자리에 있을 때 뱀의 머리가 이동해야 하는 위치는 뱀이 나가는 게이트 위치의 바로 전 위치임(*만약 gate2 가 하단 가장자리에 있으면 그 위치 바로 위).

뱀이 나가는 게이트의 x 위치가 0 이면(==뱀이 나가는 게이트가 맵의 좌측 가장자리에 있으면) 뱀의 이동방향을 RIGHT 로 바꿔줘야함. 즉, 이는 코드에서 보이는 것과 같이 (*snake).setPos(Snake::RIGHT); 구문으로 수행함.

다음으로는 (*snake).teleportMove(1, (*gate2).gatePosY); 구문이 호출되는데, teleportMove 함수의 로직은 바로 위에서 설명하였음.

tempGate2PosX += 1; 과 같은 구문은 미리 저장해둔 게이트의 위치에 연산을 하는 것임. 이와 같은 연산이 필요한 이유는 뱀 머리가 이동한 위치랑 같도록 만들어서, 뱀의 꼬리가 이 위치에 도달했을 때 함수를 종료하기 위함임(* 자세한 설명은 밑에서 이을 것).


즉, 위와 같이 뱀이 나가는 게이트가 맵의 가장자리에 위치하는 4가지 경우(좌측 가장자리, 우측 가장자리, 상단 가장자리, 하단 가장자리) 는 모두 위와 같은 로직으로 동작함.


51. 위와 같은 로직을 수행 한 후, 맨 처음 조건문 if((*gate2).isEdgeGate) 를 빠져나온 다음에, 다음과 같은 로직 만나게 됨.

```
while (true) {
	// 뱀의 꼬리가 나가는 게이트 바로 직전 위치에 도달할 때까지 순환문 돌도록..
	if ((*snake).body.back().x == tempGate2PosX && (*snake).body.back().y == tempGate2PosY) {
		(*gate1).isSnakeMoved = false;
		(*gate2).isSnakeMoved = false;
		break;
	}

	(*snake).move((*snake).moveX, (*snake).moveY);
	Sleep(150);
}
Sleep(300);
```

이는 뱀의 맨 마지막 몸통이 뱀 머리가 이동한 위치(* 뱀이 나가는 게이트 위치의 바로 직전 부분) 에 도달할 때까지 뱀의 move 함수를 호출함. 도달하면 (*gate1).isSnakeMoved = false;
(*gate2).isSnakeMoved = false;
구문이 수행되고 break 문으로 while 문을 종료함(* isSnakeMoved 변수는 main.cpp 에서 조건문과 함께 사용되어 게이트의 위치를 바꾸는 로직을 수행할지 말지를 결정하는데 사용하는 변수라고 앞전에 설명함).

즉, 이제 게이트의 위치를 바꿀 시간이 되었을 때 isSnakeMoved 변수의 값이 false 이므로, 게이트의 위치르르 바꾸는 로직이 수행되어 게이트의 위치가 바뀜.

51. 지금까지는 뱀이 나가는 게이트가 맵 가장자리에 있는 경우를 먼저 설명하였음. 다음부터는 뱀이 나가는 게이트가 맵 내부에 있는 경우를 설명할 것임. 코드는 다음과 같음.

```
else {
	// 뱀이 나가는 게이트가 맵 가장자리가 아닌 맵 내부에 있을 때..
	// 이때 비로소 뱀이 들어가는 게이트(==gate1)의 currSnakeInputDir 변수를 사용할 것임!!
	
	if ((*gate2).verticalBlocked == false && (*gate2).horizontalBlocked == false) {
		// 뱀의 진출 방향이 자유로운 경우..
		// 현재 뱀의 진행방향으로 진출..
		// 즉, 여기선 snake 의 setPos 함수를 호출할 필요가 없음..
		// 그냥 snake 의 teleportMove 함수만 호출하면 됨..

		// 만약 뱀의 이동 방향이 오른쪽인 상태로 들어가는 게이트에 들어갔을 때
		// 뱀은 뱀이 나가는 게이트(==gate2) 에 자신의 이동 방향을 더한 곳으로 텔포돼야함..
		// 즉, 이동 방향이 오른쪽이므로 뱀의 moveX=1, moveY=0 임.
		// 즉, 이를 나가는 게이트의 위치에 더해주면, 나가는 게이트 위치 바로 오른쪽에 뱀이 텔포됨!!!
	}
	else if ((*gate2).verticalBlocked == true && (*gate2).horizontalBlocked == false) {
		// 뱀이 나가는 게이트의 위아래가 막혀있으면
		// 뱀의 진출 가능 방향은 좌우임..
		
		switch ((*gate1).currSnakeInputDir) {
			// 뱀이 들어가는 게이트의 왼쪽 또는 오른쪽으로 진입할 때는 뱀의 이동 방향을 그대로 유지하면 되므로 따로 case 로 구분할 필요 없음..
			// 변경 사항이 없으니까..
		case Gate::UP:
			// 만약 뱀이 들어가는 게이트의 위쪽에서 뱀이 진입하면, 나가는 게이트의 왼쪽으로 진출해야함..
			// 이 경우에는 뱀의 이동방향을 왼쪽으로 변경해줘야함..
			
			(*snake).setPos(Snake::LEFT);

			break;
		case Gate::DOWN:
			// 만약 뱀이 들어가는 게이트의 아래쪽에서 뱀이 진입하면, 나가는 게이트의 오른쪽으로 진출해야함..
			// 이 경우에는 뱀의 이동방향을 오른쪽으로 변경해줘야함..

			(*snake).setPos(Snake::RIGHT);

			break;
		}
	}
	else if ((*gate2).verticalBlocked == false && (*gate2).horizontalBlocked == true) {
		// 뱀이 나가는 게이트의 양옆이 막혀있으면
		// 뱀의 진출 가능 방향은 상하임..

		switch ((*gate1).currSnakeInputDir) {
			// 뱀이 들어가는 게이트의 위쪽 또는 오른쪽으로 진입할 때는 뱀의 이동 방향을 그대로 유지하면 되므로 따로 case 로 구분할 필요 없음..
			// 변경 사항이 없으니까..
		case Gate::LEFT:
			// 만약 뱀이 들어가는 게이트의 왼쪽에서 뱀이 진입하면, 나가는 게이트의 아래쪽으로 진출해야함..
			// 뱀의 이동방향을 아래쪽으로 변경해줘야함..

			(*snake).setPos(Snake::DOWN);

			break;
		case Gate::RIGHT:
			// 만약 뱀이 들어가는 게이트의 오른쪽에서 뱀이 진입하면, 나가는 게이트의 위쪽으로 진출해야함..
			// 뱀의 이동방향을 위쪽으로 변경해줘야함..

			(*snake).setPos(Snake::UP);

			break;
		}
	}

	// 이 로직은 공통 부분이라 밖으로 따로 뺀 것..
	(*snake).teleportMove((*gate2).gatePosX + (*snake).moveX, (*gate2).gatePosY + (*snake).moveY);

	tempGate2PosX += (*snake).moveX;
	tempGate2PosY += (*snake).moveY;
}
```

뱀이 나가는 게이트가 맵 내부에 있을 때, 게이트 상태의 가능한 경우는 다음과 같이 세가지임

1. 나가는 게이트의 양옆과, 위아래가 막혀있지 않은 자유로운 상태
2. 나가는 게이트의 위아래가 막혀있는 상태
3. 나가는 게이트의 양옆이 막혀있는 상태

이와 같이 3가지의 경우로 나누어서 조건문에 사용하고 있음. 자세한 코드 설명은 코드 안에 있음.

52. 위의 로직이 다 수행된 후, 나가는 게이트가 맵 가장자리에 있었던 경우와 마찬가지로 다음의 로직이 수행됨. 자세한 설명은 위에서 했으므로 여기선 생략.

```
while (true) {
	// 뱀의 꼬리가 나가는 게이트 바로 직전 위치에 도달할 때까지 순환문 돌도록..
	if ((*snake).body.back().x == tempGate2PosX && (*snake).body.back().y == tempGate2PosY) {
		(*gate1).isSnakeMoved = false;
		(*gate2).isSnakeMoved = false;
		break;
	}

	(*snake).move((*snake).moveX, (*snake).moveY);
	Sleep(150);
}
Sleep(300);
```

다음은 Snake 클래스의 snakeTouchGate 함수 코드 전부을 가져온 것. 지금까지 설명한 부분 다 들어있는 코드(*이해에 도움이 되기 위해 제시함)

```
// gate1 은 뱀이 들어가는 게이트, gate2 는 뱀이 나가는 게이트, snakeMoveX 랑 snakeMoveY 는 현재 뱀이 이동하는 방향
void snakeTouchGate(Gate* gate1, Gate* gate2, int snakeMoveX, int snakeMoveY, Snake* snake) {
	gateCount += 1;

	// 게이트의 위치를 저장해놓고.. 이를 밑 조건문에서 값을 업데이트(뱀의 머리가 이동한 곳과 값이 같도록..)
	// 그럼 이제 이 변수를 뱀의 몸통이 반복문을 돌면서 이용할 것(*뱀의 몸통 마지막 부분의 위치가 이 변수와 같아지면 반복문 종료하는 식으로 이용..)
	int tempGate2PosX = (*gate2).gatePosX;
	int tempGate2PosY = (*gate2).gatePosY;

	// 이 isSnakeMoved 변수가 true 일 때에는 게이트 위치가 새로 업데이트 되면 안됨..
	(*gate1).isSnakeMoved = true;
	(*gate2).isSnakeMoved = true;

	// 뱀이 들어가는 게이트를 기준으로 생각했을 때, 뱀이 어느쪽으로 들어오는지는 현재 뱀의 이동방향의 반대임..
	// 즉, 현재 뱀의 이동 방향이 LEFT 라면, currSnakeInputDir 는 그의 반대인 RIGHT..
	if (snakeMoveX == -1 && snakeMoveY == 0)
		(*gate1).currSnakeInputDir = Gate::RIGHT;
	else if (snakeMoveX == 1 && snakeMoveY == 0)
		(*gate1).currSnakeInputDir = Gate::LEFT;
	else if (snakeMoveX == 0 && snakeMoveY == -1)
		(*gate1).currSnakeInputDir = Gate::DOWN;
	else if (snakeMoveX == 0 && snakeMoveY == 1)
		(*gate1).currSnakeInputDir = Gate::UP;


	
	// 뱀이 gate1 으로 들어갔으므로.. gate2 로 나와야함..
	// 그래서 계획으로는 snakeHead 클래스에 teleportMove 라는 함수를 만들어서 그쪽으로 아예 위치를 옮기도록 해야할 듯!
	// snake 클래스에도 teleportMove 함수 따로 만들어서 호출할 것..
	if ((*gate2).isEdgeGate) {
		// 뱀이 나가는 게이트가 맵 가장자리에 있을 때..
		// 뱀의 머리가 어디로 이동해야 하냐면.. gate2 의 위치 바로 전 위치.. (만약 gate2 가 하단 가장자리에 있으면 그 위치 바로 위, 좌측 가장자리면 그 위치 바로 오른쪽)

		if ((*gate2).gatePosX == 0) {
			// 뱀이 나가는 게이트가 좌측 가장자리에 있으면 뱀의 이동방향은 RIGHT 로 바꿔줘야함..
			(*snake).setPos(Snake::RIGHT);
			(*snake).teleportMove(1, (*gate2).gatePosY);

			tempGate2PosX += 1;
		}
		else if ((*gate2).gatePosX == 45) {
			// 뱀이 나가는 게이트가 우측 가장자리에 있으면 뱀의 이동방향은 LEFT 로 바꿔줘야함..
			(*snake).setPos(Snake::LEFT);
			(*snake).teleportMove(44, (*gate2).gatePosY);

			tempGate2PosX -= 1;
		}
		else if ((*gate2).gatePosY == 0) {
			// 뱀이 나가는 게이트가 상단 가장자리에 있으면 뱀의 이동방향은 DOWN 으로 바꿔줘야함..
			(*snake).setPos(Snake::DOWN);
			(*snake).teleportMove((*gate2).gatePosX, 1);

			tempGate2PosY += 1;
		}
		else if ((*gate2).gatePosY == 22) {
			// 뱀이 나가는 게이트가 하단 가장자리에 있으면 뱀의 이동방향은 UP 으로 바꿔줘야함..
			(*snake).setPos(Snake::UP);
			(*snake).teleportMove((*gate2).gatePosX, 21);

			tempGate2PosY -= 1;
		}
	}
	else {
		// 뱀이 나가는 게이트가 맵 가장자리가 아닌 맵 내부에 있을 때..
		// 이때 비로소 뱀이 들어가는 게이트(==gate1)의 currSnakeInputDir 변수를 사용할 것임!!
		
		if ((*gate2).verticalBlocked == false && (*gate2).horizontalBlocked == false) {
			// 뱀의 진출 방향이 자유로운 경우..
			// 현재 뱀의 진행방향으로 진출..
			// 즉, 여기선 snake 의 setPos 함수를 호출할 필요가 없음..
			// 그냥 snake 의 teleportMove 함수만 호출하면 됨..

			// 만약 뱀의 이동 방향이 오른쪽인 상태로 들어가는 게이트에 들어갔을 때
			// 뱀은 뱀이 나가는 게이트(==gate2) 에 자신의 이동 방향을 더한 곳으로 텔포돼야함..
			// 즉, 이동 방향이 오른쪽이므로 뱀의 moveX=1, moveY=0 임.
			// 즉, 이를 나가는 게이트의 위치에 더해주면, 나가는 게이트 위치 바로 오른쪽에 뱀이 텔포됨!!!
		}
		else if ((*gate2).verticalBlocked == true && (*gate2).horizontalBlocked == false) {
			// 뱀이 나가는 게이트의 위아래가 막혀있으면
			// 뱀의 진출 가능 방향은 좌우임..
			
			switch ((*gate1).currSnakeInputDir) {
				// 뱀이 들어가는 게이트의 왼쪽 또는 오른쪽으로 진입할 때는 뱀의 이동 방향을 그대로 유지하면 되므로 따로 case 로 구분할 필요 없음..
				// 변경 사항이 없으니까..
			case Gate::UP:
				// 만약 뱀이 들어가는 게이트의 위쪽에서 뱀이 진입하면, 나가는 게이트의 왼쪽으로 진출해야함..
				// 이 경우에는 뱀의 이동방향을 왼쪽으로 변경해줘야함..
				
				(*snake).setPos(Snake::LEFT);

				break;
			case Gate::DOWN:
				// 만약 뱀이 들어가는 게이트의 아래쪽에서 뱀이 진입하면, 나가는 게이트의 오른쪽으로 진출해야함..
				// 이 경우에는 뱀의 이동방향을 오른쪽으로 변경해줘야함..

				(*snake).setPos(Snake::RIGHT);

				break;
			}
		}
		else if ((*gate2).verticalBlocked == false && (*gate2).horizontalBlocked == true) {
			// 뱀이 나가는 게이트의 양옆이 막혀있으면
			// 뱀의 진출 가능 방향은 상하임..

			switch ((*gate1).currSnakeInputDir) {
				// 뱀이 들어가는 게이트의 위쪽 또는 오른쪽으로 진입할 때는 뱀의 이동 방향을 그대로 유지하면 되므로 따로 case 로 구분할 필요 없음..
				// 변경 사항이 없으니까..
			case Gate::LEFT:
				// 만약 뱀이 들어가는 게이트의 왼쪽에서 뱀이 진입하면, 나가는 게이트의 아래쪽으로 진출해야함..
				// 뱀의 이동방향을 아래쪽으로 변경해줘야함..

				(*snake).setPos(Snake::DOWN);

				break;
			case Gate::RIGHT:
				// 만약 뱀이 들어가는 게이트의 오른쪽에서 뱀이 진입하면, 나가는 게이트의 위쪽으로 진출해야함..
				// 뱀의 이동방향을 위쪽으로 변경해줘야함..

				(*snake).setPos(Snake::UP);

				break;
			}
		}

		// 이 로직은 공통 부분이라 밖으로 따로 뺀 것..
		(*snake).teleportMove((*gate2).gatePosX + (*snake).moveX, (*gate2).gatePosY + (*snake).moveY);

		tempGate2PosX += (*snake).moveX;
		tempGate2PosY += (*snake).moveY;
	}


	while (true) {
		// 뱀의 꼬리가 나가는 게이트 바로 직전 위치에 도달할 때까지 순환문 돌도록..
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
```


---------------------------------------------------------------------------스텝 4끝------------------------------------------------------------------------

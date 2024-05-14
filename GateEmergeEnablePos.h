#pragma once

// 게이트가 등장할 수 있는 map 요소의 값은 1
// map 을 반복문 돌면서 1 이면 그 위치를 저장할 것임..
// 그럼 이제 생성된 인스턴스를 를 GateEmergeEnablePos 타입의 벡터에 넣어줄 것임..
// 즉, Gate 클래스에는 gateEmergeEnablePos 타입의 인스턴스를 저장할 벡터가 필요..
class GateEmergeEnablePos {
public:
	GateEmergeEnablePos(int x, int y);

public:
	int gateEmergeEnablePosX;
	int gateEmergeEnablePosY;
};
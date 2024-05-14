#include <iostream>
#include <windows.h>
#define MAPWIDTH 21
using namespace std;


class UserInfo {
private:
    int current_length;
    int max_length;
    int GrowthItemC;
    int PoisonItemC;
    int GateUseC;
    int current_Time;
    bool goalsCompleted[4]; // �� ��ǥ�� �޼� ����
    int infoStartX; // ���� ��� ���� X ��ǥ
    bool IsCompelet;

public:
    void gotoxy(int x, int y) {
        COORD pos = { x, y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }

    UserInfo() {
        current_length = 3;
        max_length = 3;
        GrowthItemC = 0;
        PoisonItemC = 0;
        GateUseC = 0;
        current_Time = 0;
        infoStartX = MAPWIDTH * 2 + 8;
        IsCompelet = false;
        for (int i = 0; i < 4; ++i) {
            goalsCompleted[i] = false;
        }
    }
    void ReSetInfo() {
        current_length = 3;
        max_length = 3;
        GrowthItemC = 0;
        PoisonItemC = 0;
        GateUseC = 0;
        current_Time = 0;
        infoStartX = MAPWIDTH * 2 + 8;
        IsCompelet = false;
        for (int i = 0; i < 4; ++i) {
            goalsCompleted[i] = false;
        }
    }

    bool IsGoal() {
        return IsCompelet;
    }
    // ���� ����
    void setCurrentLength(const int current_length) {
        this->current_length = current_length;
    }

    // �ִ� ����
    void setMaxLen(const int current_len) {
        if (current_len > max_length) {
            max_length = current_len;
        }
    }

    // ������ ����
    void setGrowthItem(const int current_C) {
        this->GrowthItemC = current_C;
    }
    void setPoisonItem(const int current_C) {
        this->PoisonItemC = current_C;
    }

    // ������ ȹ�� �޼���
    void IncreaseGetGrowthItem() {
        GrowthItemC++;
    }
    void IncreaseGetPoisonItem() {
        PoisonItemC++;
    }

    // ����Ʈ ��� ����
    int getGateUse() {
        return GateUseC;
    }
    void setGateUse(const int current_C) {
        this->GateUseC = current_C;
    }
    void IncreaseGateCount() {
        GateUseC++;
    }

    // �ð� ����
    void setTime(const int current_T) {
        this->current_Time = current_T;
    }

    // ���� ���
    void ShowInfo() {
        gotoxy(infoStartX, 0);
        cout << "Score Board" << "\n";

        gotoxy(infoStartX, 1);
        cout << "B: " << current_length <<" / "<< max_length << "\n";

        gotoxy(infoStartX, 2);
        cout << "+: " << GrowthItemC << "\n";

        gotoxy(infoStartX, 3);
        cout << "-: " << PoisonItemC << "\n";

        gotoxy(infoStartX, 4);
        cout << "G: " << GateUseC << "\n";

        gotoxy(infoStartX, 5);
        cout << "Time: " << current_Time << "\n";
    }

    // �̼� ���� ���
    void Mission(int Bgoal, int Pgoal, int MinGoal, int Ggoal) {
        gotoxy(infoStartX, 9);
        cout << "Mission" << "\n";
        PrintMissionInfo("B", max_length, Bgoal, 0);
        PrintMissionInfo("+", GrowthItemC, Pgoal, 1);
        PrintMissionInfo("-", PoisonItemC, MinGoal, 2);
        PrintMissionInfo("G", GateUseC, Ggoal, 3);

        IsCompelet = true;
        for (int i = 0; i < 4; ++i) {
            if (!goalsCompleted[i]) {
                IsCompelet = false;
                break;
            }
        }
    }

private:

    // �̼� ���� ��� ���� �Լ�
    int GetYPosition(const string& label) {
        if (label == "B") return 10;
        else if (label == "+") return 11;
        else if (label == "-") return 12;
        else if (label == "G") return 13;
        else return -1;
    }

    void PrintMissionInfo(const string& label, int current, int goal, int goalIndex) {
        gotoxy(MAPWIDTH * 2 + 8, GetYPosition(label));

        cout << label << ": " << goal << " ";
        if (goalsCompleted[goalIndex]) {
            cout << "(v)\n";
            return;
        }
        else {
            if (current < goal) {
                cout << "( )\n";
            }
            else {
                cout << "(v)\n";
                goalsCompleted[goalIndex] = true;
            }
        }
    }

};
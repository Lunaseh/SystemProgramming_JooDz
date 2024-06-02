#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include "../server.h"
#include "modified_linetracing.h"

int my_score = 0;
int direction = 0;
int next_action;
int start_location; // 시작 지점이 (0,0)이면 0, (4,4)이면 1
client_info player_you;
client_info player_me;
enum Action currentAction;

void drive(int row, int col) {
    int dir = [0, 0, 0, 0];
    //동남서북으로 떨어진 거리를 계산
    if (row - player_me.row >= 0) {
        dir[0] = row - player_me.row;
    }
    else {
        dir[2] = (row - player_me.row) * -1;
    }

    if (row - player_me.row >= 0) {
        dir[3] = col - player_me.col;
    }
    else {
        dir[1] = (col - player_me.col) * -1;
    }

    //현재 방향이 맞으면 계속 직진
    for (int i = 0; i < dir.size(); i++) {
        if (dir[i] > 0 && direction == i) {
            next_action = 0;
            return;
        }
    }

    //현재 방향과 목표에 따른 방향 전환
    //남
    if (direction == 1) {
        if (dir[0] > 0) {
            next_action = -1;
        }
        else if (dir[2] > 0) {
            next_action = 1;
        }
        else {
            next_action = 1;
        }
        return;
    }
    //북
    else if (direction == 3) {
        if (dir[0] > 0) {
            next_action = 1;
        }
        else if (dir[2] > 0) {
            next_action = -1;
        }
        else {
            next_action = -1;
        }
        return;
    }
    //동
    else if (direction == 0) {
        if (dir[1] > 0) {
            next_action = 1;
        }
        else if (dir[3] > 0) {
            next_action = -1;
        }
        else {
            next_action = -1;
        }
        return;
    }
    //서
    else if (direction == 2) {
        if (dir[1] > 0) {
            next_action = -1;
        }
        else if (dir[3] > 0) {
            next_action = 1;
        }
        else {
            next_action = 1;
        }
        return;
    }
}

// 함정 체크를 위한 함수
bool isTrap(int row, int col, Item board[MAP_ROW][MAP_COL]) {
    return board[row][col].status == trap;
}

// 아이템 체크를 위한 함수
bool hasItem(int row, int col, Item board[MAP_ROW][MAP_COL])) {
    return board[row][col].status == item;
}

// 현재 위치에서 한 칸 이내의 모든 가능한 이동 방향을 확인하여 안전한 경로를 찾음
bool findSafeMove(int& targetRow, int& targetCol, Item board[MAP_ROW][MAP_COL]) {
    int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // 동, 남, 서, 북
    for (int i = 0; i < 4; i++) {
        int newRow = player_me.row + directions[i][0];
        int newCol = player_me.col + directions[i][1];
        if (newRow >= 0 && newRow < 5 && newCol >= 0 && newCol < 5 && !isTrap(newRow, newCol, board)) {
            targetRow = newRow;
            targetCol = newCol;
            return true;
        }
    }
    return false;
}

// 안전한 경로로 이동
void safeDrive(int targetRow, int targetCol, Item board[MAP_ROW][MAP_COL]) {
    drive(targetRow, targetCol);
}

void algorithm(Item board[MAP_ROW][MAP_COL]) {
    int targetRow = -1, targetCol = -1;
    bool moveToItem = false;

    // 먼저 한 칸 이내에 아이템이 있는 곳을 찾음
    int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // 동, 남, 서, 북
    for (int i = 0; i < 4; i++) {
        int newRow = player_me.row + directions[i][0];
        int newCol = player_me.col + directions[i][1];
        if (newRow >= 0 && newRow < 5 && newCol >= 0 && newCol < 5 && hasItem(newRow, newCol, board)) {
            targetRow = newRow;
            targetCol = newCol;
            moveToItem = true;
            break;
        }
    }

    // 한 칸 이내에 아이템이 없으면 트랩이 없는 안전한 곳을 찾음
    if (!moveToItem && !findSafeMove(targetRow, targetCol, board)) {
        // 이동할 수 있는 모든 곳에 트랩이 있는 경우 정지
        next_action = -1;
        return;
    }

    // 안전한 경로로 이동
    safeDrive(targetRow, targetCol, board);
}

int main() {
    while (true) {
        DGIST dgist;//DGIST 구조를 받아왔다.
        Node served_map[MAP_ROW][MAP_COL];
        string QR_data = ""; //QR 인식 데이터. 현재 위치.
        //두 플레이어의 데이터를 받아서 저장
        //두 플레이어가 다른 위치
        if (dgist.player[0].row == QR_data[0] && dgist.player[0].col == QR_data[1]) {
            player_you = dgist.player[1];
            player_me = dgist.player[0];
        }
        else {
            player_you = dgist.player[0];
            player_me = dgist.player[1];
        }
        // 우리가 (0,0)에서 시작하는 경우
        if (player_me.col == 0 && player_me.row == 0) {
            start_location = 0;
        }
        // 우리가 (4,4)에서 시작하는 경우
        else {
            start_location = 1;
        }

        served_map = dgist.map;

        if (start_location == 0) {
            //계획대로 폭탄을 전부 놓음.
            if (served_map[0][1].score == -8 && served_map[1][1].score == -8 && served_map[2][1].score == -8 && served_map[3][1].score == -8) {
                break;
            }
            //계획은 어긋낫지만 폭탄이 없음
            else if (player_me.bomb == 0) {
                break;
            }
            //폭탄 설치하러 가기
            else {
                if (served_map[0][1].score != -8) {
                    drive(0, 1);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
                else if (served_map[1][1].score != -8) {
                    drive(1, 1);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
                else if (served_map[2][1].score != -8) {
                    drive(2, 1);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
                else if (served_map[3][1].score != -8) {
                    drive(3, 1);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
            }
        }
        else {
            //계획대로 폭탄을 전부 놓음.
            if (served_map[4][3].score == -8 && served_map[3][3].score == -8 && served_map[2][3].score == -8 && served_map[1][3].score == -8) {
                break;
            }
            //계획은 어긋낫지만 폭탄이 없음
            else if (player_me.bomb == 0) {
                break;
            }
            //폭탄 설치하러 가기
            else {
                if (served_map[3][3].score != -8) {
                    drive(3, 3);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
                else if (served_map[2][3].score != -8) {
                    drive(2, 3);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
                else if (served_map[1][3].score != -8) {
                    drive(1, 3);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
                else if (served_map[0][3].score != -8) {
                    drive(0, 3);
                    currentAction = setBomb;
                    player_me.bomb--;
                }
            }
        }
    }
    while (true) {
        DGIST dgist;//DGIST 구조를 받아왔다.
        Node served_map[MAP_ROW][MAP_COL];
        string QR_data = ""; //QR 인식 데이터. 현재 위치.
        //두 플레이어의 데이터를 받아서 저장

        //두 플레이어가 다른 위치
        if (dgist.player[0].row == QR_data[0] && dgist.player[0].col == QR_data[1]) {
            player_you = dgist.player[1];
            player_me = dgist.player[0];
        }
        else {
            player_you = dgist.player[0];
            player_me = dgist.player[1];
        }

        served_map = dgist.map;

        //안전한 곳에서 주행 시작
        algorithm(served_map);
}

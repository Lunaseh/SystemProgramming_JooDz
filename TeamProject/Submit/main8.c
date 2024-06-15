#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <limits.h> // INT_MAX를 사용하기 위해 추가
#include <stdbool.h>
#include "server.h"
#include "modified_linetracing.h"

#ifdef __cplusplus
extern "C" {
    #endif
    
    extern int apple;
    
    #ifdef __cplusplus
}
#endif

#define PIN_L1 2
#define PIN_L2 3
#define PIN_R1 0
#define PIN_R2 7

#define DEVICE_ADDR 0x16

int fd;
int my_score = 0;
int direction = 0;
int next_action;
int b = 0;
int start_location; // 시작 지점이 (0,0)이면 0, (4,4)이면 1
client_info player_you;
client_info player_me;
enum Action currentAction;

extern void detect_qr_code(int* row, int* col); // QR 코드 인식 함수 선언
int write_i2c_block_data(int reg, unsigned char* data, int length) {
    unsigned char buf[length + 1];
    buf[0] = reg;
    for (int i = 0; i < length; i++) {
        buf[i + 1] = data[i];
    }
    if (write(fd, buf, length + 1) != length + 1) {
        printf("Failed to write to the i2c bus\n");
        return -1;
    }
    return 0;
}

int ctrl_car(int l_dir, int l_speed, int r_dir, int r_speed) {
    int reg = 0x01;
    unsigned char data[4] = { l_dir, l_speed, r_dir, r_speed };
    return write_i2c_block_data(reg, data, 4);
}

int car_run(int speed1, int speed2) {
    int dir1 = (speed1 < 0) ? 0 : 1;
    int dir2 = (speed2 < 0) ? 0 : 1;

    return ctrl_car(dir1, abs(speed1), dir2, abs(speed2));
}

int car_stop() {
    int reg = 0x02;
    unsigned char stop_data[1] = { 0x00 };
    return write_i2c_block_data(reg, stop_data, 1);
}

int tracking_function() {
    int L1 = digitalRead(PIN_L1);
    int L2 = digitalRead(PIN_L2);
    int R1 = digitalRead(PIN_R1);
    int R2 = digitalRead(PIN_R2);

    printf("Sensor Readings: L1=%d, L2=%d, R1=%d, R2=%d\n", L1, L2, R1, R2);
    
    
    if (L1 == LOW && L2 == LOW && R1 == LOW && R2 == LOW) {
        car_run(-40, 70);
        delay(250);
        if (R2 == LOW) {
            int random = 0;
            random = rand() % 2;
            printf("%d\n",random);
            if (random == 0) {
                car_run(70, -40);
                delay(50);
            }
                
        }
    }
    else if (L1 == LOW && L2 == LOW && R1 == HIGH && R2 == LOW) {
        car_run(-40, 70);
        delay(50);  // 더 긴 지연 시간으로 설정
    }
    else if (L1 == LOW && L2 == HIGH && R1 == LOW && R2 == LOW) {
        car_run(-40, 70);
        delay(50);  // 더 긴 지연 시간으로 설정
    }

    else if (L1 == LOW && (L2 == LOW || R1 == LOW) && R2 == HIGH) {
        car_run(-40, 70);
        delay(250);  // 더 긴 지연 시간으로 설정
        if (R2 == LOW) {
            int random = 0;
            random = rand() % 2;
            printf("%d\n",random);
            if (random == 0) {
                car_run(70, -40);
                delay(50);
            }
                
        }
    }
    
    else if (L1 == HIGH && (L2 == LOW || R1 == LOW) && R2 == LOW) {
        car_run(70, -40);
        delay(250);  // 더 긴 지연 시간으로 설정
        if (L1 == LOW) {
            int random = 0;
            random = rand() % 2;
            printf("%d\n",random);
            if (random == 0) {
                car_run(-40, 70);
                delay(50);
            }
                
        }
    }
    
    else if (L1 == HIGH && L2 == LOW && R1 == HIGH && R2 == HIGH) {
        car_run(-30, 60);
        delay(50);  // 더 긴 지연 시간으로 설정
    }
    else if (L1 == HIGH && L2 == HIGH && R1 == LOW && R2 == HIGH) {
        car_run(60, -30);
        delay(50);  // 더 긴 지연 시간으로 설정
    }
    
    else if (L1 == LOW && L2 == HIGH && R1 == HIGH && R2 == HIGH) {
        car_run(-40, 70);
        delay(50);  // 더 긴 지연 시간으로 설정
    }
    else if (L1 == HIGH  && L2 == HIGH && R1 == HIGH && R2 == LOW) {
        car_run(70, -40);
        delay(50);  // 더 긴 지연 시간으로 설정
    }
    else if (L1 == HIGH && L2 == LOW && R1 == LOW && R2 == HIGH) {
        car_run(55, 55);
    }
    else if (L1 == HIGH  && L2 == HIGH && R1 == HIGH && R2 == HIGH) {
        if (b==0) {
            delay(50);  // 더 긴 지연 시간으로 설정
        }
        else {
            car_run(-40,-40);
            delay(100);
        }
    }
    else {
        delay(50);
    }
    return 0;
}

void error_handling(const char* message) {
    perror(message);
    exit(1);
}

void drive(int row, int col) {
    int dir[4] = { 0, 0, 0, 0 };
    // 동남서북으로 떨어진 거리를 계산
    if (row - player_me.row >= 0) {
        dir[0] = row - player_me.row;
    }
    else {
        dir[2] = (row - player_me.row) * -1;
    }

    if (col - player_me.col >= 0) {
        dir[1] = col - player_me.col;
    }
    else {
        dir[3] = (col - player_me.col) * -1;
    }

    // 현재 방향이 맞으면 계속 직진
    for (int i = 0; i < 4; i++) {
        if (dir[i] > 0 && direction == i) {
            next_action = 0;
            return;
        }
    }

    // 현재 방향과 목표에 따른 방향 전환
    // 남
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
    // 북
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
    // 동
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
    // 서
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
bool isTrap(int row, int col, Node board[MAP_ROW][MAP_COL]) {
    return board[row][col].item.status == trap;
}

// 아이템 체크를 위한 함수
bool hasItem(int row, int col, Node board[MAP_ROW][MAP_COL]) {
    return board[row][col].item.status == item;
}

// 현재 위치에서 한 칸 이내의 모든 가능한 이동 방향을 확인하여 안전한 경로를 찾음
bool findSafeMove(int* targetRow, int* targetCol, Node board[MAP_ROW][MAP_COL]) {
    int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // 동, 남, 서, 북
    for (int i = 0; i < 4; i++) {
        int newRow = player_me.row + directions[i][0];
        int newCol = player_me.col + directions[i][1];
        if (newRow >= 0 && newRow < MAP_ROW && newCol >= 0 && newCol < MAP_COL && !isTrap(newRow, newCol, board)) {
            *targetRow = newRow;
            *targetCol = newCol;
            return true;
        }
    }
    return false;
}

// 안전한 경로로 이동
void safeDrive(int targetRow, int targetCol) {
    drive(targetRow, targetCol);
}

void algorithm(Node board[MAP_ROW][MAP_COL]) {
    int targetRow = -1, targetCol = -1;
    bool moveToItem = false;

    // 먼저 한 칸 이내에 아이템이 있는 곳을 찾음
    int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // 동, 남, 서, 북
    for (int i = 0; i < 4; i++) {
        int newRow = player_me.row + directions[i][0];
        int newCol = player_me.col + directions[i][1];
        if (newRow >= 0 && newRow < MAP_ROW && newCol >= 0 && newCol < MAP_COL && hasItem(newRow, newCol, board)) {
            targetRow = newRow;
            targetCol = newCol;
            moveToItem = true;
            break;
        }
    }

    // 한 칸 이내에 아이템이 없으면 트랩이 없는 안전한 곳을 찾음
    if (!moveToItem && !findSafeMove(&targetRow, &targetCol, board)) {
        // 이동할 수 있는 모든 곳에 트랩이 있는 경우 정지
        next_action = -1;
        return;
    }

    // 안전한 경로로 이동
    safeDrive(targetRow, targetCol);
}

void decideNextMove(DGIST* dgist, ClientAction* action, int playerId) {
    client_info* player = &dgist->players[playerId];
    int closestItemRow = -1, closestItemCol = -1;
    int minDistance = INT_MAX;

    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            if (dgist->map[i][j].item.status == item) {
                int distance = abs(player->row - i) + abs(player->col - j);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestItemRow = i;
                    closestItemCol = j;
                }
            }
        }
    }

    if (closestItemRow != -1 && closestItemCol != -1) {
        if (player->row < closestItemRow)
            action->row = player->row + 1;
        else if (player->row > closestItemRow)
            action->row = player->row - 1;
        else
            action->row = player->row;

        if (player->col < closestItemCol)
            action->col = player->col + 1;
        else if (player->col > closestItemCol)
            action->col = player->col - 1;
        else
            action->col = player->col;

        action->action = move;
    }
}

int main(int argc, char* argv[]) {
    
    int row, col;
    ClientAction action;
    DGIST dgist;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP address> <Port>\n", argv[0]);
        return 1;
    }

    const char* server_ip = argv[1];
    int port = atoi(argv[2]);
    int player_id = 0;

    printf("1\n");
    // 서버 연결 초기화
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }
    printf("2\n");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(port);
    printf("before connect \n");
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect");
        close(sock);
        exit(1);
    }
    
    if (wiringPiSetup() == -1) {
        printf("WiringPi Setup Failure\n");
        return 1;
    }
    else printf("WiringPi Setup Successed\n");

    fd = wiringPiI2CSetup(DEVICE_ADDR);
    if (fd == -1) {
        printf("I2C Setup Failure\n");
        return 1;
    }
    else printf("I2C Setup Successed\n");

    pinMode(PIN_L1, INPUT);
    pinMode(PIN_L2, INPUT);
    pinMode(PIN_R1, INPUT);
    pinMode(PIN_R2, INPUT);
    
    printf("3\n");
    
        
    int a = 0;
    int ret = 0;
    while (1) {
        ret = tracking_function();
        delay(10);  // 루프의 지연 시간을 늘림
        a++;
        b = 0;
        if (a==200) {
            car_stop();
            printf("hi\n");
            detect_qr_code(&row, &col);
            printf("hi2\n");
            action.row = row;
            action.col = col;
            printf("4\n");
            // 폭탄 설치 알고리즘 (여기에 실제 조건을 추가해야 합니다)
            int random1 = 0;
            random1 = rand() % 2;
            printf("%d\n",random1);
            if (apple == 1) {
                action.action = move;
            }
            else {
                action.action = move;
            }

            a = 0;
            b = 1;

            if (send(sock, &action, sizeof(ClientAction), 0) < 0) {
                error_handling("send");
            }
        }
        printf("5\n");
        
    }
    printf("9\n");
    close(sock);
    return 0;
}

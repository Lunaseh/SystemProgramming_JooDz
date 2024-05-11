#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep 함수 사용을 위해 포함
 
// Yahboom 전용 라이브러리에 해당하는 함수의 원형을 선언
// 이 함수들은 실제로 구현되어야 합니다.
void Car_Spin_Right(int, int);
void Car_Spin_Left(int, int);
void Car_Run(int, int);
void Car_Stop();
 
// GPIO 핀 번호를 WiringPi 핀 번호로 매핑
// 실제 핀 번호에 맞게 조정해야 할 수 있음
#define Tracking_Right1 0   // X1A  오른쪽 첫 번째 센서
#define Tracking_Right2 2  // X2A  오른쪽 두 번째 센서
#define Tracking_Left1 3   // X1B 왼쪽 첫 번째 센서
#define Tracking_Left2 4  // X2B 왼쪽 두 번째 센서
 
void tracking_function() {
    int Tracking_Left1Value = digitalRead(Tracking_Left1);
    int Tracking_Left2Value = digitalRead(Tracking_Left2);
    int Tracking_Right1Value = digitalRead(Tracking_Right1);
    int Tracking_Right2Value = digitalRead(Tracking_Right2);
 
    if ((Tracking_Left1Value == LOW || Tracking_Left2Value == LOW) && Tracking_Right2Value == LOW) {
        Car_Spin_Right(70, 30);
        usleep(200000); // 0.2초 대기
    } else if (Tracking_Left1Value == LOW && (Tracking_Right1Value == LOW || Tracking_Right2Value == LOW)) {
        Car_Spin_Left(30, 70);
        usleep(200000); // 0.2초 대기
    } else if (Tracking_Left1Value == LOW) {
        Car_Spin_Left(70, 70);
        usleep(50000); // 0.05초 대기
    } else if (Tracking_Right2Value == LOW) {
        Car_Spin_Right(70, 70);
        usleep(50000); // 0.05초 대기
    } else if (Tracking_Left2Value == LOW && Tracking_Right1Value == HIGH) {
        Car_Spin_Left(60, 60);
        usleep(20000); // 0.02초 대기
    } else if (Tracking_Left2Value == HIGH && Tracking_Right1Value == LOW) {
        Car_Spin_Right(60, 60);
        usleep(20000); // 0.02초 대기
    } else if (Tracking_Left2Value == LOW && Tracking_Right1Value == LOW) {
        Car_Run(70, 70);
    }
}
 
int main(void) {
    if (wiringPiSetup() == -1) {
        printf("setup wiringPi failed !\n");
        return 1; // 초기화 실패 시 프로그램 종료
    }
 
    pinMode(Tracking_Left1, INPUT);
    pinMode(Tracking_Left2, INPUT);
    pinMode(Tracking_Right1, INPUT);
    pinMode(Tracking_Right2, INPUT);
 
    while (1) {
        tracking_function();
    }
 
    return 0;
}

상황에 맞는 메뉴 있음

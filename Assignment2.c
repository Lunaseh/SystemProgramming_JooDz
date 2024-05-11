#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define Tracking_Right1 0   // WiringPi 핀 번호 사용
#define Tracking_Right2 2
#define Tracking_Left1 3
#define Tracking_Left2 4

void setup() {
    wiringPiSetup(); // WiringPi 초기화
    pinMode(Tracking_Left1, INPUT);
    pinMode(Tracking_Left2, INPUT);
    pinMode(Tracking_Right1, INPUT);
    pinMode(Tracking_Right2, INPUT);
}

void tracking_function() {
    int Tracking_Left1Value = digitalRead(Tracking_Left1);
    int Tracking_Left2Value = digitalRead(Tracking_Left2);
    int Tracking_Right1Value = digitalRead(Tracking_Right1);
    int Tracking_Right2Value = digitalRead(Tracking_Right2);

    // 여기에 파이썬 코드에서의 로직을 C언어로 변환하여 추가
    // 예를 들어:
    if ((Tracking_Left1Value == LOW || Tracking_Left2Value == LOW) && Tracking_Right2Value == LOW) {
        // car.Car_Spin_Right(70, 30)와 동등한 동작을 구현
        printf("Right Turn\n");
        delay(200);
    }
    // 나머지 조건들도 유사하게 추가

    // 주의: 실제 모터 제어 함수는 여러분의 모터 드라이버 라이브러리에 따라 달라질 수 있습니다.
}

int main(void) {
    setup();
    
    while(1) {
        tracking_function();
    }
    
    return 0;
}

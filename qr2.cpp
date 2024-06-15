#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <string>

extern "C" {
    int apple;
}

extern "C" void detect_qr_code(int* row, int* col) {
    using namespace cv;
    using namespace std;
    
    printf("hi3\n");
    VideoCapture cap(0); // Open the default camera
    printf("hi4\n");
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // Set pixel format
    printf("hi5\n");

    if (!cap.isOpened()) {
        cerr << "ERROR: Unable to open the camera" << endl;
        return;
    }

    QRCodeDetector qrDecoder;
    Mat frame, bbox, rectifiedImage;
    string data;
    
    printf("hi6\n");

    while (true) {
        printf("hello1\n");
        cap >> frame;
        printf("hello2\n");
        if (frame.empty()) {
            cerr << "ERROR: Couldn't grab a frame" << endl;
            break;
        }
        
        printf("hi7\n");

        bool detect = qrDecoder.detect(frame, bbox);
        if (detect) {
            printf("decoded\n");
            data = qrDecoder.decode(frame, bbox, rectifiedImage);
            if (data.length() > 0) {
                cout << "Decoded Data: " << data << endl;
                *row = data[0] - '0';
                *col = data[1] - '0';
                printf("hi8\n");
                apple = 1;
                break;
            }
        }
        else {
            cout << "QR Code not detected" << endl;
            break;
    
        }

        int key = waitKey(1);
        if (key == 'q') {
            break;
        }
    }

}

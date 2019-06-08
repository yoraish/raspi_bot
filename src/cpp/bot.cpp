#include <iostream>
#include <fstream>
#include <ctime>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <thread>

using namespace std;
using namespace cv;

int camera_feeder()
{
    // function that runs and updates a window on the desktop
    // set up camera
    // initializing camera feed
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_BUFFERSIZE, 1); // internal buffer will now store only 3 frames
    cap.set(CV_CAP_PROP_FRAME_WIDTH,320); // experimenting with this and the next lines
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);

    if (!cap.isOpened())
    {
        cerr << "ERROR: Unable to open the camera" << endl;
        return 0;
    }

    Mat frame;
    cout << "Start grabbing, press a key on Live window to terminate" << endl;

    while (true)
    {

        // capture image
        cap >> frame;
        if (frame.empty())
        {
            cerr << "ERROR: Unable to grab from the camera" << endl;
            break;
        }

        // flip frame
        Mat hflipped;
        // horizonatally
        flip(frame, hflipped, 1);
        // vertically
        Mat final_frame;
        flip(hflipped, final_frame, 0);

        imshow("Live", final_frame);
        int key = cv::waitKey(1); // changed from 30
    }

    // unreachable code, but leaving it here for reference
    cout << "Closing the camera" << endl;
    cap.release();
    destroyAllWindows();
    cout << "bye!" << endl;
}

int handle_command()
{
    // function that handles keyboard commands
    // initializing serial
    int fd;

    if ((fd = serialOpen("/dev/ttyACM0", 9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    // loop, wait for key. When got key, serially send it to Arduino
    while (true)
    {
        // ask for command
        cout << "write command >>>";
        string command;
        cin >> command;
        command += "\n";
        // convert command to const char pointer
        const char *c = command.c_str();

        // send serially to Arduino
        serialPuts(fd, c);
        cout << "wrote command=" << command << endl;
        // let the message bounce for a moment
        usleep(50000); // sleep for 5 ms

        // get the response
        string data = "";
        while (serialDataAvail(fd) > 0)
        {
            data += serialGetchar(fd);
        }
        cout << "got=[" << data << "]" << endl;
    }
}



int main(int argc, char **argv)
{
    bool use_cam = false;
    if (argc > 1 ){
        // then something was specified about showing an image
        // argv[0] = 0 means no camera
        // argv[1] = 1 or non existing means camera
        cout << "this is " << argv[1] << endl;
        if (*argv[1] == '0'){
            use_cam = false;
        }
    }

    if (use_cam == true){
        thread camera_thread(camera_feeder);
        camera_thread.join();
    }

    thread command_thread(handle_command);
    command_thread.join();
    return -1;
}

// // initializing camera feed
// VideoCapture cap(0);
// cap.set(CV_CAP_PROP_BUFFERSIZE, 1); // internal buffer will now store only 3 frames

// if (!cap.isOpened())
// {
//     cerr << "ERROR: Unable to open the camera" << endl;
//     return 0;
// }

// Mat frame;
// cout << "Start grabbing, press a key on Live window to terminate" << endl;

// // initializing serial
// int fd;

// if ((fd = serialOpen("/dev/ttyACM0", 9600)) < 0)
// {
//     fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
//     return 1;
// }

// // Loop, getting and printing characters
// char a = 'b';
// while (true)
// {

//     // capture image
//     cap >> frame;
//     if (frame.empty())
//     {
//         cerr << "ERROR: Unable to grab from the camera" << endl;
//         break;
//     }

//     // flip frame
//     Mat hflipped;
//     // horizonatally
//     flip(frame, hflipped, 1);
//     // vertically
//     Mat final_frame;
//     flip(hflipped, final_frame, 0);

//     imshow("Live", final_frame);
//     int key = cv::waitKey(30);
//     key = (key == 255) ? -1 : key; //#Solve bug in 3.2.0
//     if (key >= 0)
//     {
//         char command_char = char(key);
//         string command(1, command_char);
//         command += "\n";
//         const char *c = command.c_str();

//         serialPuts(fd, c);
//         cout << "wrote command=" << command << endl;
//         // let the message bounce for a moment
//         usleep(50000); // sleep for 5 ms

//         // get the response
//         string data = "";
//         while (serialDataAvail(fd) > 0)
//         {
//             data += serialGetchar(fd);
//         }
//         cout << "got=[" << data << "]" << endl;

//         cout << "loop" << endl;
//     }

//     // // ask for command
//     // cout << "write command >>>";
//     // string command;
//     // cin >> command;
//     // command += "\n";
//     // convert command to const char pointer
//     // const char *c = command.c_str();

//     // send serially to Arduino
//     // serialPuts(fd, c);
//     // cout << "wrote command=" << command << endl;
//     // // let the message bounce for a moment
//     // usleep(50000); // sleep for 5 ms

//     // // get the response
//     // string data = "";
//     // while (serialDataAvail(fd) > 0)
//     // {
//     //     data += serialGetchar(fd);
//     // }
//     // cout << "got=[" << data << "]" << endl;

//     // fflush(stdout);
//     // serialFlush(fd);
// }

// cout << "Closing the camera" << endl;
// cap.release();
// destroyAllWindows();
// cout << "bye!" << endl;
// return 0;

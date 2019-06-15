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



class RaspiBot
{
private:
    /* data */
public:
    // the public variables and methods
    // only constructors and default values
    RaspiBot(bool imgproc);
    bool use_preview = true;
    int fd;
    ~RaspiBot();
    void send_command(string cmd);
    int handle_command();
    int camera_feeder();
    vector<int> get_object_x(Mat img);
    void object_follow();
    int target_radius = 40;
    const int frame_width = 80;
    const int frame_height = 60;
    const int object_leftmost = frame_width/4;
    const int object_rightmost = frame_width*3/4;

};
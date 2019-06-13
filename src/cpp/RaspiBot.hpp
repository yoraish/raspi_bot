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
    pair<int,int> get_object_x(Mat img);
    void object_follow();
};
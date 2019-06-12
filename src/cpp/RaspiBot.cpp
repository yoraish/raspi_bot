#include "RaspiBot.hpp"


/////////////////////////////////
/*
* TODO
* create debug console
*/



RaspiBot::RaspiBot(bool preview = true)
{
    // initialization function
    // initialize public variables
    this->use_preview = preview;

    // initializes the Arduino
    // starts connection, and sends a short movement
    // this is mostly a sanity check that the arduino is connected
    //start serial
    fd = serialOpen("/dev/ttyACM0", 9600);
    if (fd < 0)
    {
        fprintf(stderr, "* Unable to open serial device: %s\n", strerror(errno));
    }
    else
    {
        usleep(2000000); // sleep for 5 ms

        cout << "ok we have arduino" << endl;
        send_command("rit");
        usleep(50000); // sleep for 5 ms
        send_command("lft");
        usleep(50000); // sleep for 5 ms
        send_command("stp");

        cout << "* Arduino connection initialized" << endl;
    }

    // spin the keyboard command thread
    cout << "* Spinning the command thread. Detaching from main thread." << endl;
    thread command_thread(&RaspiBot::handle_command, this);
    command_thread.detach();

    // spin the camera preview thread (on screen live window), if asked for
    if (use_preview == true)
    {
        cout << "* Spinning the preview thread, in main thread" << endl;
        thread preview_thread(&RaspiBot::camera_feeder, this);
        preview_thread.join();
    }

    // keep Spinning
    while(true){}

}

RaspiBot::~RaspiBot()
{
    cout << "* Closing Raspi Bot" << endl;
}

void RaspiBot::send_command(string cmd)
{
    // function that sends commands to the Arduino
    // cmd in "fwd", "bwd", "rit", "lft", "stp"
    // fd is the channel we are communicating on
    // int fd = serialOpen("/dev/ttyACM0", 9600);
    string str_cmd;

    if (cmd == "fwd")
    {
        str_cmd = "w\n";
    }
    else if (cmd == "bwd")
    {
        str_cmd = "s\n";
    }
    else if (cmd == "rit")
    {
        str_cmd = "d\n";
    }
    else if (cmd == "lft")
    {
        str_cmd = "a\n";
    }
    else
    {
        str_cmd = "e\n"; //stop
    }

    //send it
    const char *cmd_ptr = str_cmd.c_str();
    serialPuts(fd, cmd_ptr);

    // get the response
    usleep(50000); // sleep for 5 ms

    // get the response
    string back_data = "";
    while (serialDataAvail(fd) > 0)
    {
        back_data += serialGetchar(fd);
    }
}

int RaspiBot::camera_feeder()
{
    // function that runs and updates a window on the desktop
    // set up camera
    // initializing camera feed
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_BUFFERSIZE, 1);    // internal buffer will now store only 3 frames
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320); // experimenting with this and the next lines
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

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

int RaspiBot::handle_command()
{
    // function that handles keyboard commands
    // sends the commands to the Arduino, which moves the robot correctly
    // initializing serial
    int channel = serialOpen("/dev/ttyACM0", 9600);

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
        serialPuts(channel, c);
        cout << "wrote command=" << command << endl;
        // let the message bounce for a moment
        usleep(50000); // sleep for 5 ms

        // get the response
        string data = "";
        while (serialDataAvail(channel) > 0)
        {
            data += serialGetchar(channel);
        }
        // cout << "got=[" << data << "]" << endl; // TODO move into debug console
    }
}

int main(int argc, char **argv)
{

    // check if want to use cam or not
    bool use_cam = true;
    if (argc > 1)
    {
        // then something was specified about showing an image
        // argv[0] = 0 means no camera
        // argv[1] = 1 or non existing means camera
        cout << "* Use preview = " << argv[1] << endl;
        if (*argv[1] == '0')
        {
            use_cam = false;
        }
    }

    // create a new class instance
    RaspiBot bot(use_cam);

    return -1;
}

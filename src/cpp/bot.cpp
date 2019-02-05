#include <iostream>
#include <fstream>
#include <ctime>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>


// #include <opencv2/opencv.hpp>
// #include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;



int main ()
{
  int fd ;

  if ((fd = serialOpen ("/dev/ttyACM0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

// Loop, getting and printing characters
char a = 'b';
  while (true)
  {
    // ask for command
    cout << "write command >>>";
    string command;
    cin >> command;
    command += "\n";
    // convert command to const char pointer
    const char * c = command.c_str();

    // send serially to Arduino
    serialPuts(fd, c);
    cout << "wrote command="<< command <<endl;
    // let the message bounce for a moment
    usleep(50000);  // sleep for 5 ms
    
    // get the response
    string data = "";
    while (serialDataAvail(fd) > 0){
        data += serialGetchar(fd);
    }
    cout << "got=[" << data << "]" << endl;

    
    fflush (stdout) ;
    serialFlush(fd);


  }
}
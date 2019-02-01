#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
// #include <opencv2/opencv.hpp>
// #include <opencv2/highgui.hpp>
using namespace std;

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
    cout << "write command >>>";
    string command;
    cin >> command;
    command += "\n";
    const char * c = command.c_str();
    serialPuts(fd, c);


    cout << "wrote command="<< command <<endl;
    // let the message bounce for a moment
    usleep(50000);  
    
    string data = "";

    while (serialDataAvail(fd) > 0){
        data += serialGetchar(fd);
    }



    cout << "got=[" << data << "]" << endl;

    
    fflush (stdout) ;
    serialFlush(fd);


  }
}
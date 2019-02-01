#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
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

    // usleep(500000);    // char msg = 'a';
    // cout << "waiting " <<endl;
    
    string data = "";

    while (serialDataAvail(fd) > 0){
        data += serialGetchar(fd);
    }



    cout << "got=[" << data << "]" << endl;

    
    // cout <<endl <<  "flushing " <<endl;

    fflush (stdout) ;
    serialFlush(fd);
    // usleep(2000000);    // char msg = 'a';
    // serialPuts(fd, "b\n");
    // usleep(2000000);    // char msg = 'a';


  }
}
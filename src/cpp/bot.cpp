#include <iostream>
#include <fstream>
#include <ctime>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
#include <raspicam/raspicam.h>

// #include <opencv2/opencv.hpp>
// #include <opencv2/highgui.hpp>
using namespace std;


raspicam::RaspiCam setup_camera(){
    // returns a camera object
    raspicam::RaspiCam camera;
    if ( !camera.open()) {cerr<<"Error opening camera"<<endl;return camera;}
	//wait a while until camera stabilizes
	cout<<"Sleeping for 3 secs"<<endl;
	usleep(3000000);
    // flip to show correct side up
    camera.setHorizontalFlip(false);
    camera.setVerticalFlip(false);
    return camera;


}
int capture(raspicam::RaspiCam * camera, string path = ""){
    // function that captures one image and stores it in the path provided
    // if no path provided, then default to im0.png
    if (path == ""){
        path = "im0.png";
    }

    // capture image
    camera->grab();
    cout<<"1"<< endl;
    // allocate memory
    unsigned char *data=new unsigned char[  camera->getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
	//extract the image in rgb format


	camera->retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//get camera image
    // save
    cout << "11" << endl;
	std::ofstream outFile ( path,std::ios::binary );
    cout << "2"<< endl;
	outFile<<"P6\n"<<camera->getWidth() <<" "<<camera->getHeight() <<" 255\n";
    cout << "3"<< endl;
	outFile.write ( ( char* ) data, camera->getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
    cout << "4"<< endl;
    // free memory
    delete data;
    cout << "5"<< endl;
    return 1;
}

int main(){
    raspicam::RaspiCam camera = setup_camera();

    capture(&camera, "yo.png");
    cout << "out of function" << endl;

    cout <<"hflip=" << camera.isHorizontallyFlipped()<<  endl;
    camera.setHorizontalFlip(true);
    cout <<"hflip=" << camera.isHorizontallyFlipped()<<  endl;
    capture(&camera, "yoyo.png");
    cout <<"hflip=" << camera.isHorizontallyFlipped()<<  endl;

    // camera->setVerticalFlip(true);




}


// int main ()
// {
//   int fd ;

//   if ((fd = serialOpen ("/dev/ttyACM0", 9600)) < 0)
//   {
//     fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
//     return 1 ;
//   }

// // Loop, getting and printing characters
// char a = 'b';
//   while (true)
//   {
//     // ask for command
//     cout << "write command >>>";
//     string command;
//     cin >> command;
//     command += "\n";
//     // convert command to const char pointer
//     const char * c = command.c_str();

//     // send serially to Arduino
//     serialPuts(fd, c);
//     cout << "wrote command="<< command <<endl;
//     // let the message bounce for a moment
//     usleep(50000);  // sleep for 5 ms
    
//     // get the response
//     string data = "";
//     while (serialDataAvail(fd) > 0){
//         data += serialGetchar(fd);
//     }
//     cout << "got=[" << data << "]" << endl;

    
//     fflush (stdout) ;
//     serialFlush(fd);


//   }
// }

#include "stdafx.h"
#include <iostream>
//#include "FrameServer.h"//TEST
#include <opencv2\opencv.hpp>


using namespace cv;
using namespace std;
 /** Function Headers */
 void detectAndDisplay( Mat frame );

CascadeClassifier face_cascade;
CascadeClassifier left_eye_cascade;
CascadeClassifier right_eye_cascade;

String face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
String left_eye_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_lefteye_2splits.xml";
String right_eye_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_righteye_2splits.xml"; 
string window_name = "Capture - Face detection";

int main(){
	VideoCapture cam;
	cam.open(0);//("http://192.168.7.190/axis-cgi/mjpg/video.cgi?resolution=4cif&.mjpeg");
		
	if(!cam.isOpened()){
		cerr<<"Could not open camera."<<endl; return -1;
	}

	//FrameServer MyServer(cam,30);
	
	 //-- 1. Load the cascades
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
	if( !left_eye_cascade.load( left_eye_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
	if( !right_eye_cascade.load( right_eye_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	
	Mat frame,rframe;

	while(1){

		cam>>frame;//=MyServer.GetNextFrame();
		//resize(frame,rframe,Size(640,480));
		detectAndDisplay( frame );
	
		if(waitKey(1) > 0 )break;
	}
	waitKey();
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> left_eyes, right_eyes;

    //-- In each face, detect eyes
  //  left_eye_cascade.detectMultiScale( faceROI, left_eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
  //  for( size_t j = 0; j <left_eyes.size() ; j++ )//eyes.size()
  //   {
		//if(j>0) continue;
  //     Point center( faces[i].x + left_eyes[j].x + left_eyes[j].width*0.5, faces[i].y + left_eyes[j].y + left_eyes[j].height*0.5 );
  //     int radius = cvRound( (left_eyes[j].width + left_eyes[j].height)*0.25 );
  //     circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
  //   }

	
	left_eye_cascade.detectMultiScale( faceROI, right_eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    for( size_t j = 0; j <right_eyes.size() ; j++ )//eyes.size()
     {
		if(j>0) continue;
       Point center( faces[i].x + right_eyes[j].x + right_eyes[j].width*0.5, faces[i].y + right_eyes[j].y + right_eyes[j].height*0.5 );
       int radius = cvRound( (right_eyes[j].width + right_eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
     }
  }
  //-- Show what you got
  imshow( window_name, frame );
 }
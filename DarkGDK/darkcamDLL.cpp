/*
 DarkCam 1.4 : A computer vision library for DarkGDK          
 by Paul Chin  (March 3, 2010)								    
  Last Updated: March 10, 2010									 
 

   Requires:  
      cv200.dll
	  cxcore200.dll
      highgui200.dll  
      haarcascade_frontalface_alt.xml
	  A_gest.xml

																 
  * Free to modify and use for any purpose *					 
*/

#include "cv.h"
#include "highgui.h"
#include <windows.h>

#define DllExport extern "C" __declspec (dllexport)
//BASIC
#define QUIT  0
#define RUN 1
#define PAUSE 2
#define SNAPSHOT 3

int status=PAUSE; 


//HEAD TRACKING
#define BASIC 1
#define HEAD_TRACKING 2


//-------- Basic (just video, run, pause, snapshot)  ------------
CvMemStorage			*storage; 
DWORD WINAPI BasicFunction(LPVOID param)
{   
	CvCapture *capture;
	IplImage  *frame;
	int       key = 0;
	storage = cvCreateMemStorage( 0 );
	capture = cvCaptureFromCAM( 0 );

	assert(storage && capture );

	cvNamedWindow("video",0);
    
	while(true ) {
		frame = cvQueryFrame( capture );

		if( !frame ) {
			//fprintf( stderr, "Cannot query frame!\n" );
			break;
		}
		
		cvFlip( frame, frame, 1 ); //flip y axis
		
		if(status==RUN) cvShowImage( "video",frame);
		if(status==SNAPSHOT) {
			cvSaveImage("camshot.jpg",frame);
			status=PAUSE;
		}

		key = cvWaitKey( 10 ); if(key==27) break; //ESC
		if(status==QUIT) break;	
	} //end while

	cvReleaseCapture( &capture );
	cvDestroyWindow( "video" );
	
	cvReleaseMemStorage( &storage );

	return 0;
}

DllExport void _pause(){
	status=PAUSE;
}
DllExport void _run(){//to resume after a pause
	status=RUN;
}
DllExport void _snapshot(){
	status=SNAPSHOT;
}
DllExport void _release(){
	status=QUIT;
}
//end Basic


//-------- Head Tracking --------------
CvHaarClassifierCascade *cascade;
//CvMemStorage			*storage;
CvRect *r;
void detectFaces( IplImage *img )
{
	int i;

	CvSeq *faces = cvHaarDetectObjects(
			img,
			cascade,
			storage,
			1.2,
			2,
			0 /*CV_HAAR_DO_CANNY_PRUNNING*/,
			cvSize( 80, 80 ) );

	for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
		r = ( CvRect* )cvGetSeqElem( faces, i );
		cvRectangle( img,
					 cvPoint( r->x, r->y ),
					 cvPoint( r->x + r->width, r->y + r->height ),
					 CV_RGB( 255, 0, 0 ), 2, 8, 0 );

		//Output x y topleft corner position
		//printf("x = %d  y = %d\n",r->x,r->y);
	}
	if(status==RUN)	cvShowImage( "video", img );
	if(status==SNAPSHOT) {
		cvSaveImage("camshot.jpg",img);
		status=PAUSE;
	}	
}

DWORD WINAPI HeadTrackingFunction(LPVOID param){
	CvCapture *capture;
	IplImage  *frame;
	int       key = 0;
	char      *filename = "haarcascade_frontalface_alt.xml";

	cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
	storage = cvCreateMemStorage( 0 );
	capture = cvCaptureFromCAM( 0 );

	assert( cascade && storage && capture );

	cvNamedWindow( "video", 0 );
    
	while( key != 27 ) {
		
		frame = cvQueryFrame( capture );

		if( !frame ) {
			//fprintf( stderr, "Cannot query frame!\n" );
			break;
		}
		frame->origin = 0;
		cvFlip( frame, frame, 1 ); //flip y axis
		detectFaces( frame );

		key = cvWaitKey( 10 );
		if(status==QUIT) break;
	}

	cvReleaseCapture( &capture );
	cvDestroyWindow( "video" );
	cvReleaseHaarClassifierCascade( &cascade );
	cvReleaseMemStorage( &storage );

	return 0;
}
DllExport int _getX(){
	if(r!=NULL)	return r->x;
	else return 0;
}
DllExport int _getY(){
	if(r!=NULL)	return r->y;
	else return 0;
}
DllExport int _getWidth(){
	if(r!=NULL)	return r->width;
	else return 0;
}
DllExport int _getHeight(){
	if(r!=NULL)	return r->height;
	else return 0;
}
//end Head Tracking


//-------- Main controller --------------
DllExport void _init(int _type){ //create a webcam
	
	DWORD Tid1;
	if(_type==BASIC)
	HANDLE hThreadOne=CreateThread(NULL,0,BasicFunction, NULL, 0, &Tid1);
	else if(_type==HEAD_TRACKING)
	HANDLE hThreadOne=CreateThread(NULL,0,HeadTrackingFunction, NULL, 0, &Tid1);

}

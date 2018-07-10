#include <iostream>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/**                                                                                                 
 * Takes in a RGB image and outputs                                                                 
 * a Lab space image.                                                                               
 **/
Mat RGB2LAB(Mat const &imgRGB){
  Mat imgLab;
  cvtColor(imgRGB, imgLab, CV_BGR2Lab);
  return imgLab;
}

Mat aLAB(Mat const &imgLab){

  Mat aLab;
  cvtColor(imgLab, aLab, CV_Lab2RGB);
  
  Size s = aLab.size();
  int steps = imgLab.step;
  int channels = imgLab.channels();

  /* Create Black and White image of a space */
  for(int y = 0; y < s.height; y++){
    for(int x = 0; x < s.width; x++){
      (*aLab.ptr<Point3_<uchar> >(y,x)).x = imgLab.data[steps*y + channels*x + 1];
      (*aLab.ptr<Point3_<uchar> >(y,x)).y = imgLab.data[steps*y + channels*x + 1];
      (*aLab.ptr<Point3_<uchar> >(y,x)).z = imgLab.data[steps*y + channels*x + 1];
    }
  }
  return aLab;
}

Mat bLAB(Mat &imgLab){
  
  Mat bLab;
  cvtColor(imgLab, bLab, CV_Lab2RGB);
  
  Size s = bLab.size();
  int steps = imgLab.step;
  int channels = imgLab.channels();
  
  /* Create Black and White image of b space */
  for(int y = 0; y < s.height; y++){
    for(int x = 0; x < s.width; x++){
      (*bLab.ptr<Point3_<uchar> >(y,x)).x = imgLab.data[steps*y + channels*x + 2];
      (*bLab.ptr<Point3_<uchar> >(y,x)).y = imgLab.data[steps*y + channels*x + 2];
      (*bLab.ptr<Point3_<uchar> >(y,x)).z = imgLab.data[steps*y + channels*x + 2];
    }
  }
  return bLab;
}


/**
 * Finds the gradient via the sobel method,
 **/
Mat gradientEdges(Mat &img){
  
  Mat gradImg;
  Mat gx, gy;
  Mat abs_gx, abs_gy;
  
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  
  GaussianBlur(img, img, Size(3,3), 0, 0, BORDER_DEFAULT);
  
  // gradient in the x direction
  Sobel(img, gx, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs(gx, abs_gx);
  
  // Gradient in the y direction
  Sobel( img, gy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs(gy, abs_gy);
  
  // Finds the combined Gradient
  addWeighted(abs_gx*1.5, 0.5, abs_gy*1.5, 0.5, 0, gradImg);
  
  return gradImg;
  
}

int main(int argc, const char* argv[]){
  
  /*if(argc < 2){
    std::cout << "Not enough input arguments" << std::endl;
    exit(1);
  }*/
  
  VideoCapture cap;
  cap.open(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH,480);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,360);
  if (!cap.isOpened())
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}


  Mat imgLab;
while(1){
 // Mat imgLab =  RGB2LAB(imread(argv[1], CV_LOAD_IMAGE_COLOR));
  bool bSuccess1 = cap.read(imgLab); // read a new frame from video
   		
               // video.write(imgLab);
		if (!bSuccess1) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			return 0;
		}
  char filename[120];
  
  Mat a = aLAB(imgLab);
  Mat b = bLAB(imgLab);

  /* Write labs */
  //imwrite("rgb_2_lab.jpg", imgLab);
  //imwrite("aLab.jpg", a);
  //imwrite("bLab.jpg", b);
  
  /* Write gradients */
  Mat g = gradientEdges(a);
  //imwrite("aLabGradient.jpg", g);
  g = gradientEdges(b);
  imwrite("/home/pi/Desktop/project/2.edge_detection/edge.png", g);
  system("sudo cp /home/pi/Desktop/project/2.edge_detection/edge.png /var/www/html/edge.png");
  }
  return 0;
}

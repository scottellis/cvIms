/*
  Experimental image processing with OpenCV
*/

#ifdef WIN32
#include <Windows.h>
#else
#define MAX_PATH 260
#endif

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define CV_NO_BACKWARD_COMPATIBILITY
#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage* binarize_image();
void find_contours(IplImage *binImg);

//char project_dir[] = "C:\\Users\\scott\\My Documents\\FluidData\\ShapeGenBW\\Circles_3\\";
char project_dir[MAX_PATH] = "/home/scott/FluidData/ShapeGenBW/Fixed_Ellipse_1/";

CvMemStorage *mem;

int main(int argc, char **argv)
{
	IplImage *binImg;

	if (argc > 1)
		strncpy(project_dir, argv[1], MAX_PATH - 1);

	mem = cvCreateMemStorage();
	if (!mem) {
		printf("Error creating mem storage\n");
		return 1;
	}

	cvNamedWindow("raw", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("raw", 10, 10);

	cvNamedWindow("masked", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("masked", 532, 10);

	cvNamedWindow("binarized", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("binarized", 10, 440);

	cvNamedWindow("contours", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("contours", 532, 440);

	binImg = binarize_image();

	if (binImg) {
		find_contours(binImg);
		cvReleaseImage(&binImg);
	}

	cvReleaseMemStorage(&mem);

	return 0;
}

void find_contours(IplImage *binImg)
{
	IplImage *contourImg;
	CvSeq *seq;
	CvContourScanner scanner;

	contourImg = cvCreateImage(cvGetSize(binImg), binImg->depth, 1);

	if (!contourImg) {
		cvReleaseMemStorage(&mem);
		return;
	}
	
	/*
	num_contours = cvFindContours(binImg, mem, &contours, sizeof(CvContour), 
					CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	*/
	scanner = cvStartFindContours(binImg, mem, sizeof(CvContour), 
					CV_RETR_EXTERNAL, 
					CV_CHAIN_APPROX_SIMPLE);

	seq = cvFindNextContour(scanner);

	while (seq) {
		cvZero(contourImg);

		cvDrawContours(contourImg, seq, 
				cvScalarAll(255), cvScalarAll(127), 
				2, 1, 8);

		cvShowImage("contours", contourImg);
		cvWaitKey(0);

		seq = cvFindNextContour(scanner);
	}

	cvReleaseImage(&contourImg);
}

IplImage* binarize_image()
{
	char buff[MAX_PATH];
	IplImage *rawImg = NULL;
	IplImage *calImg = NULL;
	IplImage *maskedImg = NULL;
	IplImage *binImg = NULL;

	memset(buff, 0, sizeof(buff));
	strcpy(buff, project_dir);
	strcat(buff, "cal_image_000001.tif");
	calImg = cvLoadImage(buff, CV_LOAD_IMAGE_GRAYSCALE);

	strcpy(buff, project_dir);
	strcat(buff, "rawfile_000000.tif");
	rawImg = cvLoadImage(buff, CV_LOAD_IMAGE_GRAYSCALE);

	maskedImg = cvCreateImage(cvGetSize(rawImg), rawImg->depth, 1);
	binImg = cvCreateImage(cvGetSize(rawImg), rawImg->depth, 1);

	if (calImg && rawImg && maskedImg && binImg) {
		//cvSub(calImg, rawImg, binImg, NULL);
		cvAbsDiff(calImg, rawImg, maskedImg);
		cvThreshold(maskedImg, binImg, 18.0, 255.0, CV_THRESH_BINARY);
		cvShowImage("raw", rawImg);
		cvShowImage("masked", maskedImg);
		cvShowImage("binarized", binImg);		
	}

	if (rawImg)
		cvReleaseImage(&rawImg);

	if (calImg)
		cvReleaseImage(&calImg);

	if (maskedImg)
		cvReleaseImage(&maskedImg);

	return binImg;
}


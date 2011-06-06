/*
  Experimental image processing with OpenCV
*/

#ifdef WIN32
#include <Windows.h>
#endif

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define CV_NO_BACKWARD_COMPATIBILITY
#include <opencv\cv.h>
#include <opencv\highgui.h>

IplImage* binarize_image();
void find_contours(IplImage *binImg);

const char project_dir[] = "C:\\Users\\scott\\My Documents\\FluidData\\ShapeGenBW\\Circles_3\\";


int main(int argc, char **argv)
{
	IplImage *binImg;

	cvNamedWindow("raw", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("raw", 10, 10);

	cvNamedWindow("masked", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("masked", 10, 430);

	cvNamedWindow("binary", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("binary", 532, 10);

	cvNamedWindow("contours", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("contours", 532, 430);

	binImg = binarize_image();

	if (binImg) {
		find_contours(binImg);
		cvReleaseImage(&binImg);
	}

	return 0;
}

void find_contours(IplImage *binImg)
{
	IplImage *contourImg;
	CvMemStorage *mem;
	CvSeq *seq;
	CvContourScanner scanner;

	mem = cvCreateMemStorage();

	if (!mem)
		return;

	contourImg = cvCreateImage(cvGetSize(binImg), binImg->depth, 1);

	if (!contourImg) {
		cvReleaseMemStorage(&mem);
		return;
	}
	
	/*
	num_contours = cvFindContours(binImg, mem, &contours, sizeof(CvContour), 
								CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	*/
	scanner = cvStartFindContours(binImg, mem, sizeof(CvContour), CV_RETR_EXTERNAL, 
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
	cvReleaseMemStorage(&mem);
}

IplImage* binarize_image()
{
	char buff[MAX_PATH];
	IplImage *rawImg = NULL;
	IplImage *calImg = NULL;
	IplImage *maskedImg = NULL;
	IplImage *binImg = NULL;

	strncpy_s(buff, sizeof(buff), project_dir, _TRUNCATE);
	strcat_s(buff, sizeof(buff), "cal_image_000001.tif");
	calImg = cvLoadImage(buff, CV_LOAD_IMAGE_GRAYSCALE);

	strncpy_s(buff, sizeof(buff), project_dir, _TRUNCATE);
	strcat_s(buff, sizeof(buff), "rawfile_000000.tif");
	rawImg = cvLoadImage(buff, CV_LOAD_IMAGE_GRAYSCALE);

	maskedImg = cvCreateImage(cvGetSize(rawImg), rawImg->depth, 1);
	binImg = cvCreateImage(cvGetSize(rawImg), rawImg->depth, 1);

	if (calImg && rawImg && maskedImg, binImg) {
		//cvSub(calImg, rawImg, binImg, NULL);
		cvAbsDiff(calImg, rawImg, maskedImg);
		cvThreshold(maskedImg, binImg, 18.0, 255.0, CV_THRESH_BINARY);
		cvShowImage("raw", rawImg);
		cvShowImage("masked", maskedImg);
		cvShowImage("binary", binImg);		
	}

	if (rawImg)
		cvReleaseImage(&rawImg);

	if (calImg)
		cvReleaseImage(&calImg);

	if (maskedImg)
		cvReleaseImage(&maskedImg);

	return binImg;
}
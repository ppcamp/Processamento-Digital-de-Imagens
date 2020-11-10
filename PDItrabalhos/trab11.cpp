#include <cstdio>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>

using namespace cv;

#define MAX 256
#define MIN -1

/**
 * @brief Apply erosion into a image using the structuring element (SE)
 * @param image A Mat object monochromatic (grayscale)
 * @param SE Filter mask
 * @return Mat object that represent the eroded image.
*/
Mat erosion( Mat image, int SE[ ][7] )
{
	Mat output( image.rows, image.cols, image.type( ), Scalar( 0, 0, 0 ) );

	// Auxiliar matrix to be base of (solve border problem). @see Gonzales, pg 652
	Mat aux( image.rows + 6, image.cols + 6, image.type( ), Scalar( MIN, MIN, MIN ) );

	// Put image in center with boders in black
	for ( int y = 3; y < aux.rows - 3; y++ )
		for ( int x = 3; x < aux.cols - 3; x++ )
			aux.at<uchar>( y, x ) = image.at<uchar>( y - 3, x - 3 );

	// Apply Structuring element (non linear filter)
	int value, temp;
	for ( int row = 3; row < aux.rows - 3; row++ )
	{
		for ( int col = 3; col < aux.cols - 3; col++ )
		{
			value = MIN;
			// Iterate over SE
			for ( int SEi = 0; SEi < 3; SEi++ )
			{
				for ( int SEj = 0; SEj < 3; SEj++ )
				{
					// Subtract mask from image (Img - Mask)
					temp = aux.at<uchar>( row + SEi - 3, col + SEj - 3 ) - SE[SEi][SEj];
					// Set max color value
					temp = ( temp < 0 ) ? 0 : ( temp > 255 ) ? 255 : temp;
					if ( temp > value ) value = temp;
				}
			}
			output.at<uchar>( row - 3, col - 3 ) = ( uchar ) ( ( int ) value );
		}
	}
	return output;
}

/**
 * @brief Apply dilation into a image using the structuring element (SE)
 * @param image A Mat object monochromatic (grayscale)
 * @param SE Filter mask
 * @return Mat object that represent the eroded image.
*/
Mat dilation( Mat image, int SE[ ][7] )
{
	Mat output( image.rows, image.cols, image.type( ), Scalar( MAX, MAX, MAX ) );

	// Auxiliar matrix to be base of (solve border problem). @see Gonzales, pg 652
	Mat aux( image.rows + 6, image.cols + 6, image.type( ), Scalar( MAX, MAX, MAX ) );

	// Put image in center with boders in black
	for ( int y = 3; y < aux.rows - 3; y++ )
		for ( int x = 3; x < aux.cols - 3; x++ )
			aux.at<uchar>( y, x ) = image.at<uchar>( y - 3, x - 3 );

	// Apply Structuring element (non linear filter)
	int value, temp;
	for ( int row = 3; row < aux.rows - 3; row++ )
	{
		for ( int col = 3; col < aux.cols - 3; col++ )
		{
			value = MAX;
			// Iterate over SE
			for ( int SEi = 0; SEi < 3; SEi++ )
			{
				for ( int SEj = 0; SEj < 3; SEj++ )
				{
					// Subtract mask from image (Img - Mask)
					temp = aux.at<uchar>( row + SEi - 3, col + SEj - 3 ) + SE[SEi][SEj];
					// Set max color value
					temp = ( temp < 0 ) ? 0 : ( temp > 255 ) ? 255 : temp;
					if ( temp < value ) value = temp;
				}
			}
			output.at<uchar>( row - 3, col - 3 ) = ( uchar ) ( ( int ) value );
		}
	}
	return output;
}




int main( )
{
	// Load image
	uchar imgData[10][10] = {
		{ 252, 46, 115, 18, 73, 203, 60, 229, 112, 183 },
		{ 109, 31, 20, 53, 225, 58, 54, 28, 170, 94 },
		{ 99, 73, 116, 115, 183, 146, 177, 88, 14, 141 },
		{ 79, 176, 132, 54, 144, 148, 231, 157, 244, 187 },
		{ 207, 28, 4, 194, 111, 122, 172, 61, 211, 71 },
		{ 185, 199, 124, 123, 40, 195, 134, 112, 17, 194 },
		{ 26, 3, 168, 251, 12, 85, 98, 205, 174, 34 },
		{ 234, 222, 166, 121, 99, 167, 33, 35, 43, 183 },
		{ 237, 102, 254, 45, 206, 234, 49, 144, 1, 70 },
		{ 17, 231, 44, 224, 67, 195, 148, 68, 127, 42 }
	};
	//Mat image( 10, 10, CV_8UC1, &imgData );
	Mat image = imread( "Assets/lena.jpg", IMREAD_GRAYSCALE );

	// Load structuring element
	int SE[7][7] = {
		{0, 0,	0,	1,	0,	0,	0 },
		{0, 0,	1,	10, 1,	0,	0 },
		{0, 1,	10, 20, 10, 1,	0 },
		{1, 10, 20, 40, 20, 10, 1 },
		{0, 1,	10, 20, 10, 1,	0 },
		{0, 0,	1,	10, 1,	0,	0 },
		{0, 0,	0,	1,	0,	0,	0 }
	};

	// Shows original image
	namedWindow( "Original Image", WINDOW_NORMAL );
	imshow( "Original Image", image );

	// Get erosion signal
	Mat erodedImage = erosion( image, SE );
	/*for ( int i = 0; i < erodedImage.rows; i++ )
	{
		for ( int j = 0; j < erodedImage.cols; j++ )
			printf( "%d\t", ( int ) erodedImage.at<uchar>( i, j ) );
		printf( "\n" );
	}*/
	namedWindow( "Eroded Image", WINDOW_NORMAL );
	imshow( "Eroded Image", erodedImage );

	// Get dilation signal
	Mat dilatedImage = dilation( image, SE );
	namedWindow( "Dilated Image", WINDOW_NORMAL );
	imshow( "Dilated Image", dilatedImage );

	// Get morph gradient
	Mat morgphologicalGradient = dilatedImage - erodedImage;
	namedWindow( "morgphologicalGradient Image", WINDOW_NORMAL );
	imshow( "morgphologicalGradient Image", morgphologicalGradient );

	// Get morphological opening
	Mat morgphologicalOpening = dilation( erodedImage, SE );
	namedWindow( "morgphologicalOpening Image", WINDOW_NORMAL );
	imshow( "morgphologicalOpening Image", morgphologicalOpening );

	// Get morphological closing
	Mat morgphologicalClosing = erosion( dilatedImage, SE );
	namedWindow( "morgphologicalClosing Image", WINDOW_NORMAL );
	imshow( "morgphologicalClosing Image", morgphologicalClosing );

	// Wait image
	waitKey( );
}
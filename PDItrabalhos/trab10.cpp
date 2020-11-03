#include <cstdio> // Console
#include <opencv2/core.hpp> // Mat obj
#include <opencv2/highgui.hpp> // Image show
#include <opencv2/imgcodecs.hpp> // Image read
#include <opencv2/imgproc/imgproc.hpp> // line

using namespace cv;

Mat applyFilter(Mat image, int filter[][3], float c = 1)
{
    Mat output(image.rows, image.cols, image.type(), Scalar(0, 0, 0));
    // Auxiliar matrix to be base of (solve border problem)
    Mat aux(image.rows + 2, image.cols + 2, image.type(), Scalar(0, 0, 0));

    // Put image in center with boders in black
    for (int y = 1; y < aux.rows - 1; y++) {
        for (int x = 1; x < aux.cols - 1; x++) {
            aux.at<uchar>(y, x) = image.at<uchar>(y - 1, x - 1);
        }
    }

    // Apply filter
    for (int y = 1; y < aux.rows - 1; y++) {
        for (int x = 1; x < aux.cols - 1; x++) {
            int newValue = 0;
            for (int fi = 0; fi < 3; fi++)
                for (int fj = 0; fj < 3; fj++)
                    newValue += filter[fi][fj] * aux.at<uchar>(y + fi - 1, x + fj - 1);
            output.at<uchar>(y - 1, x - 1) = (uchar)((float)newValue * c);
        }
    }
    return output;
}

Mat euclidian(Mat a, Mat b)
{ // a and b have the same size
    Mat output(a.rows, a.cols, a.type(), Scalar(0, 0, 0));
    for (int i = 0; i < a.rows; i++)
        for (int j = 0; j < a.cols; j++)
            output.at<uchar>(i, j) = sqrt(pow(a.at<uchar>(i, j), 2) + pow(b.at<uchar>(i, j), 2));
    return output;
}

int main()
{
    uchar data[10][10] = {
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
    // Load image
    Mat image(10, 10, CV_8UC1, &data);

    // Show image before apply changes
    // namedWindow("Original Image", WINDOW_NORMAL);
    // imshow("Original Image", image);

    int lowPass[3][3] = {
        { 1, 1, 1 },
        { 1, 1, 1 },
        { 1, 1, 1 }
    };
    int gaussian[3][3] = {
        { 1, 2, 1 },
        { 2, 4, 2 },
        { 1, 2, 1 }
    };
    int laplacian[3][3] = {
        { 0, -1, 0 },
        { -1, 4, -1 },
        { 0, -1, 0 }
    };
    int sobelX[3][3] = {
        { 1, 2, 1 },
        { 0, 0, 0 },
        { 1, -2, -1 }
    };
    int sobelY[3][3] = {
        { 1, 0, -1 },
        { 2, 0, -2 },
        { 1, 0, -1 }
    };

    float c_lowPass = 1.0 / 9.0,
          c_gaussian = 1.0 / 16.0,
          c_laplacian = 1.0 / 16.0,
          c_sobel = 1.0 / 4.0;

    Mat out1 = applyFilter(image, sobelX, c_sobel);
    Mat out2 = applyFilter(image, sobelY, c_sobel);
    Mat output = euclidian(out1, out2);

    for (int y = 0; y < output.rows; y++) {
        for (int x = 0; x < output.cols; x++) {
            printf("%3d ", output.at<uchar>(y, x));
        }
        printf("\n");
    }

    namedWindow("Output Image", WINDOW_NORMAL);
    imshow("Output Image", output);
    waitKey();

    return 0;
}

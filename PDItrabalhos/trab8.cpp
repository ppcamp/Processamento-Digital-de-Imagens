#include <cstdio> // Console
#include <opencv2/core.hpp> // Mat obj
#include <opencv2/highgui.hpp> // Image show
#include <opencv2/imgcodecs.hpp> // Image read

using namespace cv;

#define showImg(img)                \
    imshow("Playboy da Lena", img); \
    waitKey(0);

Mat imageToGray(Mat image)
{
    /**
     * Converte um objeto do tipo imagem com os canais BGR para
	 * um objeto sem saturação (msm cor em todos os canais).
	 * Sendo a cor dada pela m�dia.
	 *
	 * @param <Mat> image Objeto que será usado de base
	 * @return <Mat> Gray image
	 */

    // Cria a matriz de cinza com cor padrão preto
    Mat grayImage(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    uint16_t imcolor;

    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            imcolor = 0;
            Vec3b c = image.at<Vec3b>(Point(col, row));
            for (int color = 0; color < image.channels(); color++)
                imcolor += c[color];
            Vec3b& color = grayImage.at<Vec3b>(Point(col, row));
            color[0] = color[1] = color[2] = imcolor / 3;
        }
    }

    return grayImage;
}

int main()
{
    // Load image
    Mat image = imread("Assets/lena.jpg", IMREAD_COLOR);
    // Show image before apply changes
    showImg(image);

    // Obt�m w,h, e a qtn de canais
    auto imrows = image.rows, imcols = image.cols, imch = image.channels();
    printf("Size of mat object: %d\tMatrix dim(Pixels):%dx%d\n", imch, imrows, imcols);

    // Transforma em cinza
    auto grayImage = imageToGray(image);

    // D�vida: Existe alguma forma de acessar sem usar o Vec3b?
    showImg(grayImage);
}

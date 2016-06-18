#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int /*argc*/, char** /*argv*/) {
    
    Mat gambar, gambarTambahLaplace, gambarHasil;
    
    gambar = (Mat_<uchar>(4,4) << 0,1,2,3,4,5,6,7,8,9,0,11,12,13,14,15);
    
    // menggunakan fungsi laplace dari library opencv
    Laplacian(gambar, gambarTambahLaplace, CV_8UC1);
    cout << gambarTambahLaplace << endl;
    
    Mat kernel = (Mat_<float>(3,3) <<
                  0,  1, 0,
                  1, -4, 1,
                  0,  1, 0);
    int ukuran = 3;
    
    // hasilnya diperkecil menyesuaikan variabel ukuran (ukuran output)
    Mat frame, frame32;
    Rect roi;
    gambarTambahLaplace = Mat::zeros(gambar.size(), CV_32F);
    for(int y=0; y<gambar.rows-ukuran/2-1; y++) {
        for(int x=0; x<gambar.cols-ukuran/2-1; x++) {
            roi = Rect(x,y, ukuran, ukuran);
            frame = gambar(roi);
            frame.convertTo(frame, CV_32F);
            frame = frame.mul(kernel);
            float v = sum(frame)[0];
            gambarTambahLaplace.at<float>(y,x) = v;
        }
    }
    gambarTambahLaplace.convertTo(gambarTambahLaplace, CV_8U);
    cout << gambarTambahLaplace << endl;
    
    // menggunakan library dari opencv, cv::filter2D
    filter2D(gambar, gambarTambahLaplace, -1, kernel);
    cout << gambarTambahLaplace << endl;
    
    
    /*Nilai 1 untuk gambar asli, nilai 0 untuk merubah menjadi grayscale
    untuk hasil yang lebih baik, gunakan grayscale tapi disini untuk melihat perbedaan, kita gunakan yang asli*/
    gambar = imread("/Users/farhan/Documents/OpenCV/OpenCV-Image-Deblur/OpenCV-Image-Deblur/data/002.bmp", 1);
    
    kernel = (Mat_<float>(3,3) <<
              1,  1, 1,
              1, -8, 1,
              1,  1, 1);
    
    /*Setelah itu, lakukan filter untuk gambar. Diharapkan hasil dari laplace memiliki nilai negatif, lalu setelah filtering nilai negatif tersebut akan hilang sehingga dapat memiliki nilai warna 8 bit antara 0 sampai 255*/
    filter2D(gambar, gambarTambahLaplace, CV_32F, kernel);
    gambar.convertTo(gambar, CV_32F);
    gambarHasil = gambar - gambarTambahLaplace;
    //konversi gambar
    gambarHasil.convertTo(gambarHasil, CV_8U);
    gambarTambahLaplace.convertTo(gambarTambahLaplace, CV_8U);
    //Membuat tampilan output
    namedWindow("Hasil Menghilangkan Blur", CV_WINDOW_AUTOSIZE);
    imshow( "Hasil Menghilangkan Blur", gambarHasil );
    
    while( true ) {
        char c = (char)waitKey(10);
        if( c == 27 ) { break; }
    }
    
    return 0;
}
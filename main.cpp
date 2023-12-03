

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>


#include <iostream>
using namespace cv;
using namespace std;



class Img { // resim okuma ve işlemleri sağlanan sınıf  
private:
    int cols;// resim genişliği 
    int rows;// resim yüksekliği 
    cv::Mat img;// giriş resim 
    int** sourseImgIndexes;
    int** segmentationImgIndexes;
    int** targetImgIndexes;
    cv::Mat* segmentetionImg;
    cv::Mat* targetImg;
  
public:
    Img(string imageSours) {//kurucu 
        this->img = cv::imread(imageSours); //resim okuma 
        this->rows = this->img.rows;// satır saısı 
        this->cols = this->img.cols;// sütün sayısı
       
        this->CreateImgIndexes(this->sourseImgIndexes);// 
        this->CreateImgIndexes(this->segmentationImgIndexes);
        this->CreateImgIndexes(this->targetImgIndexes);
        this->readsourseImgIndexes();
        this->CalculateSegmantation();
        this->CalculateReSegmantation();
        this->segmentetionImg = new cv::Mat(this->rows, this->cols, CV_8UC3);
        this->targetImg = new cv::Mat(this->rows, this->cols, CV_8UC3);
        this->setSegmentetionImg();
        this->setTargetImg();



    }


    void setSegmentetionImg() { //resim oluşturma  
      

        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->segmentetionImg->at<cv::Vec3b>(i, j) = cv::Vec3b(this->segmentationImgIndexes[i][j]*255, this->segmentationImgIndexes[i][j]*255, this->segmentationImgIndexes[i][j] * 255);
            }
        }
    }

    void setTargetImg() { //resim  oluşturma 


        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->targetImg->at<cv::Vec3b>(i, j) = cv::Vec3b(this->targetImgIndexes[i][j] * 255, this->targetImgIndexes[i][j] * 255, this->targetImgIndexes[i][j] * 255);
            }
        }
    }

    void showSegmentetionImg(string title = "histo_title") {// resmin yeni halı gösterme fon.
        cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
        cv::imshow(title, *this->segmentetionImg);
        cv::waitKey(0);

    }

    void showTargetImg(string title = "histo_title") {// resmin yeni halı gösterme fon.
        cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
        cv::imshow(title, *this->targetImg);
        cv::waitKey(0);

    }

    void CreateImgIndexes(int **&img) {
        img = new int* [this->rows];
        for (int i = 0; i < this->rows; i++) {
            img[i] = new int[this->cols];
            for (int j = 0; j <this->cols; j++) {
                img[i][j] = 0;
            }
        }
    }
   
    void readsourseImgIndexes() {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                cv::Vec3b index = this->img.at<cv::Vec3b>(i, j);
                this->sourseImgIndexes[i][j] = index[0] == 0 ? 0 : 1;
            }
        }
    }

    int segmantation(int i,int j){ // birlik matris ile kesişleme yapılır 
        int value = this->sourseImgIndexes[i][j] == 1 
            && this->sourseImgIndexes[i][j - 1] == 1 
            && this->sourseImgIndexes[i][j + 1] == 1 
            && this->sourseImgIndexes[i - 1][j] == 1 
            && this->sourseImgIndexes[i + 1][j] == 1 
            ? 1 : 0;
        return value;
    }

    int reSegmantation(int i, int j) { // birlik matris ile kesişleme tersi yapılır 
        int value = this->segmentationImgIndexes[i][j] == 1 
            || this->segmentationImgIndexes[i][j - 1] == 1 
            || this->segmentationImgIndexes[i][j + 1] == 1
            || this->segmentationImgIndexes[i - 1][j] == 1
            || this->segmentationImgIndexes[i + 1][j] == 1
            ? 1 : 0;
        return value;
    }

    void CalculateSegmantation() {
        for (int i = 1; i < this->rows - 1; i++) {
            for (int j = 1; j < this->cols - 1; j++) {
                this->segmentationImgIndexes[i][j] = segmantation(i, j);
            }
        }
       
    }

    void CalculateReSegmantation() {
        for (int i = 1; i < this->rows - 1; i++) {
            for (int j = 1; j < this->cols - 1; j++) {
                this->targetImgIndexes[i][j] = reSegmantation(i, j);
            }
        }

    }


    void showImg(string title = "title") { // resim güsterme fon.
        cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
        cv::imshow(title, this->img);
        cv::waitKey(0);

    }
 
    cv::Mat getImg() { // resim alma 
        return this->img;
    }
   
    int getRows() { return this->rows; }// satır sayı alma 
    int getCols() { return this->cols; }// sütün sayı alam 

    
    ~Img() { // yıkıcı 
      
    }
};






int main()
{


    Img sourcImg("./parmakizi.tif");// giriş resim okuma 
    sourcImg.showImg("input img"); // giriş resim gösterme 
    
    sourcImg.showSegmentetionImg("segmantation");// ardışlama yaptıktan sonra resim  gösterme
    sourcImg.showTargetImg("Target");// sonuç resim 









    return 0;
}
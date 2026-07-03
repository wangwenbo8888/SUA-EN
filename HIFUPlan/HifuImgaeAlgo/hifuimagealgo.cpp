#include <math.h>

#include "hifuimagealgo.h"
#include <time.h>

#include "drawscene.h"

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

#include "image_algo.h"

#include "geometry_match.h"

#include "realtimescene.h"
#include <QMessageBox>

#ifdef DEBUG
#include <QDebug>
#endif

Q_LOGGING_CATEGORY(HIFUIMAGEALGO,"HIFUIMAGEALGO")


static double RESOLUTION = 2.07692;
HifuImageAlgo::~HifuImageAlgo()
{

}

using namespace cv;


double HifuImageAlgo::deepthmeasure(QImage image)
{
//    image.save("deepimage.png");
//    QImage deepimage;
//    deepimage.load("deepimage.png");
    QImage testimage = image.copy(100,50,340,360);
    QQueue<QPoint> point_num;
    QColor imagergb;
    for(int i = 0;i<testimage.height();i++)
    {
        for(int j = 0;j<testimage.width();j++)
        {
            imagergb = testimage.pixel(j,i);
            if(imagergb.red()!=imagergb.blue()||imagergb.red()!=imagergb.green())
            {
                point_num<<QPoint(j,i);
            }
        }
    }
    qDebug()<<"大小 ："<<point_num.size();
    int num_point = 0;
    for(int k = 0;k<point_num.size();k++)
    {
        if(point_num[point_num.size()-k-1].y()<point_num[point_num.size()-1].y())
        {
            num_point = k;
//                qDebug()<<"哪个点："<<num_point;
            break;
        }
    }
    qDebug()<<"哪个点："<<num_point;
    QPoint center_point;
    if(point_num.isEmpty())
    {
        if(!ispointnull)
        {
            QMessageBox::warning(0,"Notice","M-mode ultrasound is not open. Detection failed.");
            ispointnull = !ispointnull;
        }

        return 0;
    }
    else
    {
        if(num_point%2)
        {
            center_point = (point_num[point_num.size()-1-num_point/2]);
        }
        else
        {
            center_point = (point_num[point_num.size()-1-(num_point+1)/2]);
        }
        ispointnull = false;
    }
    qDebug()<<"中心点为:"<<center_point;
    Mat test_image(testimage.height(),testimage.width(),CV_8UC4,testimage.bits(),testimage.bytesPerLine());
    Mat gray_image;
    cvtColor(test_image,gray_image,COLOR_RGB2GRAY);
    Mat erode_mat;
//        cv::Mat otsu_mat1;

    Mat otsu_mat;
    Mat erode_kernel = getStructuringElement(MORPH_ERODE,Size(5,5));
    threshold(gray_image,otsu_mat,0,255,THRESH_OTSU);
//        cv::threshold(otsu_mat1,otsu_mat,0,255,THRESH_OTSU);
    erode(otsu_mat,erode_mat,erode_kernel);
    QImage erode_image((const unsigned char *)(erode_mat.data),erode_mat.cols,erode_mat.rows,erode_mat.step,QImage::Format_Indexed8);
    int deep;
    for(int i=0;i<erode_image.height();i++)
    {
        QColor erode_pix = erode_image.pixel(center_point.rx(),i);
//        double rgb = (erode_pixel.red()+erode_pixel.blue()+erode_pixel.green())/3;
        double rgb = erode_pix.value();
//            double r = erode_pix.red();
//            double g = erode_pix.green();
//            double b = erode_pix.blue();
//        qDebug()<<"rgb:"<<r<<" "<<g<<" "<<b<<" "<<rgb<<""<<center_point.rx()<<" "<<center_point.ry();
//        qDebug()<<"dkda!"<<erode_pix[i];
        if(rgb>0)
        {
            deep = (center_point.y()-i)/RESOLUTION;
//                ui->label_deepthmeasure->setText("深度:"+QString::number(deep));
            qDebug()<<"深度:"+QString::number(deep);
            break;
        }
        if(i>center_point.y())
        {
            deep = -1;
            break;
        }
    }
    return deep;

}

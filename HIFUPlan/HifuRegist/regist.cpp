#include <math.h>

#include "regist.h"
#include <time.h>

#include "drawscene.h"

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

#include "image_algo.h"

#include "geometry_match.h"

#ifdef DEBUG
#include <QDebug>
#endif

Q_LOGGING_CATEGORY(HIFUREGIST,"HIFUREGIST")

Regist::Regist(DrawScene* scene,QImage* image,int radius)
    : m_iRadius(radius)
{
    if (scene!=NULL)
    {
        m_pScene = scene;
    }
    else
    {
        m_pScene = NULL;
    }

    if (image!=NULL)
    {
        m_pImage = image;
    }
}

Regist::Regist(DrawScene* scene,QImage* contour)
{
    if (scene!=NULL)
    {
        m_pScene = scene;
    }
    else
    {
        m_pScene = NULL;
    }

    if (contour!=NULL)
    {
        m_pContour = contour;
    }
}

Regist::~Regist()
{

}

using namespace cv;
bool Regist::match(int usAngle,float& x,float& y,float& rotatedAngle,float& score)
{
    if (m_pScene==NULL)
    {
        qCDebug(HIFUREGIST())<<HIFUREGIST().categoryName()
                             <<"match scene pointer null !"
                             << TRACE_CMH();
        return false;
    }

    if(m_pImage==NULL)
    {
        qCDebug(HIFUREGIST())<<HIFUREGIST().categoryName()
                             <<"match image pointer null !"
                             << TRACE_CMH();
        return false;
    }

    //当前算法超声图像搜索区域直径为120mm
    // int radius = m_pImage->width();
    int radius = m_iRadius*RESOLUTION;
    int a = X_OFFSET - radius;
    int b = FOCAL_PLANE_140MM - radius;
    int w = 2*radius;

    const QImage src = m_pScene->getPixmap().toImage().copy(a,b,w,radius*0.8);
    cv::Mat clipedImage = QImageToMat(src);

    // imshow("cliped image",clipedImage);

    // 模糊去掉小的细节
    medianBlur(clipedImage, clipedImage, 9);

    cvtColor(clipedImage, clipedImage, CV_BGR2GRAY);

    Mat edge;
    // 提取边缘
    Canny(clipedImage, edge, 30, 150, 3);
    int innerRadius = radius*0.6;

    // 去掉非轮廓边缘区域
    circle(edge,Point(radius,radius),innerRadius,cvScalar(0),-1);

    circle(edge,Point(0,0),innerRadius,cvScalar(0),-1);
    circle(edge,Point(edge.cols,0),innerRadius,cvScalar(0),-1);

    // imshow("cliped edge",edge);

    CvMemStorage* storage1 = cvCreateMemStorage();
    CvSeq* first_contour1 = NULL;

    IplImage image = IplImage(edge);
    int Nc = cvFindContours(&image,
        storage1,
        &first_contour1,
        sizeof(CvContour),
        CV_RETR_EXTERNAL);

    if (Nc==0)
    {
        qCDebug(HIFUREGIST())<<HIFUREGIST().categoryName()
                             <<"get contour zero !"
                             << TRACE_CMH();

        cvReleaseMemStorage(&storage1);
        return false;
    }

    // qDebug()<<"find contour size is "<<QString::number(Nc);

    CvSeq* max_cont = NULL;
    int maxArea = std::numeric_limits<int>::min();
    for (CvSeq* c = first_contour1; c != NULL; c = c->h_next)
    {
        int tempArea = ((CvContour*)c)->rect.height * ((CvContour*)c)->rect.width;
        if (tempArea >= maxArea)
        {
            maxArea = tempArea;
            max_cont = c;
        }
    }

    if (max_cont==NULL)
    {
        qCDebug(HIFUREGIST())<<HIFUREGIST().categoryName()
                             <<"get contour empty !"
                             << TRACE_CMH();

        cvReleaseMemStorage(&storage1);
        return false;
    }

    if(max_cont->total< 5)
    {
        qCDebug(HIFUREGIST())<<HIFUREGIST().categoryName()
                             <<"get contour too small !"
                             << TRACE_CMH();

        cvReleaseMemStorage(&storage1);
        return false;
    }

    cvZero(&image);
    cvDrawContours(&image, max_cont, cvScalarAll(255), cvScalarAll(255),100);
    // cvShowImage("contour", &image);

    CvPoint leftdown(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    CvPoint rightdown(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    CvPoint temp;
    for (int i = 0; i < max_cont->total;++i)
    {
        temp = *(CvPoint*)cvGetSeqElem(max_cont, i);
        if (temp.x < leftdown.x)
        {
            leftdown = temp;
        }
        else if (temp.x == leftdown.x && temp.y < leftdown.y)
        {
            leftdown = temp;
        }

        if (temp.x > rightdown.x)
        {
            rightdown = temp;
        }
        else if (temp.x == rightdown.x && temp.y < rightdown.y)
        {
            rightdown = temp;
        }
    }

    cvLine(&image, leftdown,rightdown, cvScalarAll(255), 1, LINE_AA);

    Nc = cvFindContours(&image,storage1,&first_contour1,sizeof(CvContour),CV_RETR_EXTERNAL);
    if (Nc==0)
    {
        qCDebug(HIFUREGIST())<<HIFUREGIST().categoryName()
                             <<"get contour for close edge failed !"<<Nc
                             << TRACE_CMH();

        cvReleaseMemStorage(&storage1);
        return false;
    }

    cvDrawContours(&image, first_contour1, cvScalarAll(255), cvScalarAll(255),0,-1);

    GeoMatch ge_match;
    ge_match.CreateGeoMatchModel(&image, 10, 100);
    // ge_match.DrawContours(&image, cvScalarAll(255), 1);
    CvPoint point;
    // 获取配准轮廓起点在原图像中位置
    ge_match.GetStart(point);
    // 载入核磁图像得到的整体肌瘤轮廓
    imagePreprocess(*m_pImage, edge);

    IplImage imageDest = IplImage(edge);

    CvPoint result;
    // 获取配准轮廓起点在肌瘤背景中对应点
    score = ge_match.FindGeoMatchModel(&imageDest, 0.4, 0.8, &result);
    qCDebug(HIFUREGIST())<<HIFUREGIST().categoryName()
                         <<"score is "<<score << usAngle-90
                         <<TRACE_CMH();

    CvPoint begin;
    ge_match.GetBegin(result,begin);

#if 0
    edge = QImageToMat(*m_pContour);
    // medianBlur(edge, edge,31);  // 边界平滑
    cvtColor(edge, edge, CV_BGR2GRAY);

    Canny(edge, edge, 50, 150, 3);
    image = IplImage(edge);
    Nc = cvFindContours(&image,storage1,&first_contour1,sizeof(CvContour),CV_RETR_EXTERNAL);

    // 找到边界点最多的轮廓
    std::vector<std::vector<QPointF> > points;
    for (CvSeq* c = first_contour1; c != NULL; c = c->h_next)
    {
        std::vector<QPointF> temp;
        for (int i = 0; i < c->total; ++i)
        {
            CvPoint* p = (CvPoint*)cvGetSeqElem(c,i);
            temp.push_back(QPointF(a+point.y+p->x-begin.x,b+point.x+p->y-begin.y));
        }
        points.push_back(temp);
    }
#endif
    x = a+point.y-begin.x;
    y = b+point.x-begin.y;

    cvReleaseMemStorage(&storage1);
    return true;
//    m_pScene->setPoints(points);
//    return m_pScene->addOutline(points);
}

void DataEdgeSmooth(cv::Mat& src, cv::Mat& dst, int uthreshold, int vthreshold, int type)
{
    src.copyTo(dst);
    int height = dst.rows;
    int width = dst.cols;
    int k;  //用于循环计数传递到外部
    for (int i = 0; i < height - 1; i++)
    {
        uchar* p = dst.ptr<uchar>(i);
        for (int j = 0; j < width - 1; j++)
        {
            if (type == 0)
            {
                //行消除
                if (p[j] == 255 && p[j + 1] == 0)
                {
                    if (j + uthreshold >= width)
                    {
                        for (int k = j + 1; k < width; k++)
                            p[k] = 255;
                    }
                    else
                    {
                        for (k = j + 2; k <= j + uthreshold; k++)
                        {
                            if (p[k] == 255) break;
                        }
                        if (p[k] == 255)
                        {
                            for (int h = j + 1; h < k; h++)
                                p[h] = 255;
                        }
                    }
                }
                //列消除
                if (p[j] == 255 && p[j + width] == 0)
                {
                    if (i + vthreshold >= height)
                    {
                        for (k = j + width; k < j + (height - i)*width; k += width)
                            p[k] = 255;
                    }
                    else
                    {
                        for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
                        {
                            if (p[k] == 255) break;
                        }
                        if (p[k] == 255)
                        {
                            for (int h = j + width; h < k; h += width)
                                p[h] = 255;
                        }
                    }
                }
            }
            else  //type = 1
            {
                //行消除
                if (p[j] == 0 && p[j + 1] == 255)
                {
                    if (j + uthreshold >= width)
                    {
                        for (int k = j + 1; k < width; k++)
                            p[k] = 0;
                    }
                    else
                    {
                        for (k = j + 2; k <= j + uthreshold; k++)
                        {
                            if (p[k] == 0) break;
                        }
                        if (p[k] == 0)
                        {
                            for (int h = j + 1; h < k; h++)
                                p[h] = 0;
                        }
                    }
                }
                //列消除
                if (p[j] == 0 && p[j + width] == 255)
                {
                    if (i + vthreshold >= height)
                    {
                        for (k = j + width; k < j + (height - i)*width; k += width)
                            p[k] = 0;
                    }
                    else
                    {
                        for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
                        {
                            if (p[k] == 0) break;
                        }
                        if (p[k] == 0)
                        {
                            for (int h = j + width; h < k; h += width)
                                p[h] = 0;
                        }
                    }
                }
            }
        }
    }

    cv::blur(dst, src, cv::Size(3,3));
}

void dataPostProcess(cv::Mat& image)
{
    using namespace cv;
    cv::Mat srcBw(image);

    Size m_Size = srcBw.size();
    Mat Temp=Mat::zeros(m_Size.height+2,m_Size.width+2,srcBw.type());//延展图像
    srcBw.copyTo(Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));

    cv::floodFill(Temp, Point(0, 0), Scalar(255));

    Mat cutImg;//裁剪延展的图像
    Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);

    image = srcBw | (~cutImg);

    DataEdgeSmooth(image,srcBw,4,4,1);
}


bool Regist::move(const QPointF& point)
{

//    qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                          << "step==============1"
//                          << TRACE_CMH();

    if (m_pScene==NULL)
    {
        return false;
    }

//    if(m_pContour==NULL)
//    {
//        return false;
//    }

    if(m_pContour==NULL){
//        qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                              << "m_pContour======NULL"
//                              << TRACE_CMH();
        return false;
    }

    Mat edge = QImageToMat(*m_pContour);

//    m_pContour->save("E:\\testimage.png");


//    qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                          << "m_pContourF======"<<m_pContour->format()
//                          << TRACE_CMH();


//    qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                          << "edge======"<<edge.rows
//                          << TRACE_CMH();

    cvtColor(edge, edge, CV_BGR2GRAY);

//    qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                          << "step==============4"
//                          << TRACE_CMH();

    // 平滑太耗时
    // dataPostProcess(edge);

    Canny(edge, edge, 50, 150, 3);
    IplImage image = IplImage(edge);
    CvMemStorage* storage1 = cvCreateMemStorage();
    CvSeq* first_contour1 = NULL;
    int Nc = cvFindContours(&image,storage1,&first_contour1,sizeof(CvContour),CV_RETR_EXTERNAL);

//    qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                          << "step==============5"
//                          << TRACE_CMH();

    std::vector<std::vector<QPointF> > points;
    for (CvSeq* c = first_contour1; c != NULL; c = c->h_next)
    {
        std::vector<QPointF> temp;
        for (int i = 0; i < c->total; ++i)
        {
            CvPoint* p = (CvPoint*)cvGetSeqElem(c,i);

            temp.push_back(QPointF(p->x,p->y));
        }
        points.push_back(temp);
    }

//    qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                          << "step==============6"
//                          << TRACE_CMH();

    cvReleaseMemStorage(&storage1);

    m_pScene->setCenterAndOutline(points,point);
    //m_pScene->setPoints(points);

//    qCDebug(HIFUREGIST()) << HIFUREGIST().categoryName()
//                          << "step==============7"
//                          << TRACE_CMH();
    return m_pScene->addOutline();


}

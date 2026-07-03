
#include "image_algo.h"

#include <time.h>

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

#include "geometry_match.h"

using namespace std;

 void DrawBox(const CvBox2D& box, IplImage* img)
  {
          CvPoint2D32f point[4];
          int i;
          for (i = 0; i<4; i++)
              {
                  point[i].x = 0;
                  point[i].y = 0;
              }
         cvBoxPoints(box, point);
         CvPoint pt[4];
         for (i = 0; i<4; i++)
            {
                 pt[i].x = (int)point[i].x;
                 pt[i].y = (int)point[i].y;
             }
         cvLine(img, pt[0], pt[1], CV_RGB(255, 0, 0), 2, 8, 0);
         cvLine(img, pt[1], pt[2], CV_RGB(255, 0, 0), 2, 8, 0);
         cvLine(img, pt[2], pt[3], CV_RGB(255, 0, 0), 2, 8, 0);
         cvLine(img, pt[3], pt[0], CV_RGB(255, 0, 0), 2, 8, 0);
  }

static int gesContourCompFunc(const void* _a, const void* _b, void* userdata)
{
    int retval;
    double s1, s2;
    CvContour* a = (CvContour*)_a;
    CvContour* b = (CvContour*)_b;

    s1 = fabs(cvContourArea(a));
    s2 = fabs(cvContourArea(b));

    if (s1 < s2)
    {
        retval = 1;
    }
    else if (s1 == s2)
    {
        retval = 0;
    }
    else
    {
        retval = -1;
    }

    return retval;
}

void match(const std::string& src)
{
    if (src.empty())
    {
        return;
    }

    Mat edge;
    getEdge(src.c_str(), edge);
    // imwrite("canny.jpg", dst);
    // imshow("part_edge", edge);

    CvMemStorage* storage1 = cvCreateMemStorage();
    CvSeq* first_contour1 = NULL;

    IplImage image = IplImage(edge);
    int Nc = cvFindContours(&image,
        storage1,
        &first_contour1,
        sizeof(CvContour),
        CV_RETR_EXTERNAL);

    // getEdge("F:\\us\\model.png", edge);
    // imshow("whole_edge", edge);

    CvMemStorage* all_sto;
    CvSeq* all_cont;
    CvSeq* cur_cont;

    all_sto = cvCreateMemStorage(0);
    all_cont = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvSeq), all_sto);
    for (CvSeq* c = first_contour1; c != NULL; c = c->h_next)
    {
        if (((CvContour*)c)->rect.height * ((CvContour*)c)->rect.width >= 3000)
        {
            cvSeqPush(all_cont, c);
        }
    }

    cur_cont = (CvSeq*)cvGetSeqElem(all_cont, 0);
    cvZero(&image);
    cvDrawContours(&image, cur_cont, cvScalarAll(255), cvScalarAll(255),
        100);
    cvShowImage("contour", &image);

    CvPoint leftdown(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    CvPoint rightdown(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    CvPoint temp;
    for (int i = 0; i < cur_cont->total;++i)
    {
        temp = *(CvPoint*)cvGetSeqElem(cur_cont, i);
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

    Nc = cvFindContours(&image,
        storage1,
        &first_contour1,
        sizeof(CvContour),
        CV_RETR_EXTERNAL);
    cvDrawContours(&image, first_contour1, cvScalarAll(255), cvScalarAll(255),
        0,-1);

    Mat anti = cv::cvarrToMat(&image);

    cv::Size dst_sz = anti.size();
    cv::Point2f center(130, 126);
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, 14, 1.0);
    cv::warpAffine(anti, anti, rot_mat, dst_sz, 1, 0, 0);
    imshow("sharp", anti);

    bitwise_not(anti, anti);
    image = IplImage(anti);
    //cvShowImage("close", &image);

    GeoMatch ge_match;

    ge_match.CreateGeoMatchModel(&image, 10, 100);
    ge_match.DrawContours(&image, cvScalarAll(255), 1);
    CvPoint point;
    ge_match.GetStart(point);

    IplImage* searchImage = cvLoadImage("F:\\us\\model.png", -1);
    imagePreprocess("F:\\us\\model.png", edge);
    imshow("whole", edge);

    double max = std::numeric_limits<double>::min();
    double angle;
    for (int i = 0; i < 30;++i)
    {
        Mat temp;
        dst_sz = edge.size();
        cv::Point2f center(86, 48);
        cv::Mat rot_mat = cv::getRotationMatrix2D(center, -15.0+i, 1.0);
        cv::warpAffine(edge, temp, rot_mat, dst_sz, 1, 0, 0);

        IplImage imageDest = IplImage(temp);

        CvPoint result;
        double score = ge_match.FindGeoMatchModel(&imageDest, 0.4, 0.8, &result);
        if (max < score)
        {
            max = score;
            angle = -15.0 + i;
        }
    }

    dst_sz = edge.size();
    // cv::Point2f center(130, 126);
    rot_mat = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(edge, edge, rot_mat, dst_sz, 1, 0, cvScalarAll(255));

    IplImage imageDest = IplImage(edge);

    CvPoint result;
    double score = ge_match.FindGeoMatchModel(&imageDest, 0.4, 0.8, &result);

    ge_match.DrawContours(&imageDest, result, cvScalarAll(255), 1);

    cvNamedWindow("Template", CV_WINDOW_AUTOSIZE);
    cvShowImage("Template", &imageDest);

    cvNamedWindow("Search Image", CV_WINDOW_AUTOSIZE);
    cvShowImage("Search Image", &imageDest);
    // wait for both windows to be closed before releasing images

    // getEdgeFromIp1Image(&imageDest, cur_cont);

}

cv::Mat QImageToMat(const QImage& image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
//    case QImage::Format_Grayscale8:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
//        break;
    }
    return mat;
}

void imagePreprocess(const QImage& qimage, Mat& clipedImage)
{
    Mat src = QImageToMat(qimage);

    medianBlur(src, clipedImage,31);

    cvtColor(clipedImage, clipedImage, CV_BGR2GRAY);
}

void imagePreprocess(const std::string& file, Mat& clipedImage)
{
    Mat src = imread(file.c_str());
    Mat dst;

    if (src.cols == 800)
    {
        clipedImage = src.rowRange(180, 450).colRange(255, 500);
    }
    else
    {
        clipedImage = src;
    }

    IplImage image = IplImage(clipedImage);
    IplImage* temp = cvCreateImage(
        cvGetSize(&image),
        IPL_DEPTH_8U,
        3
    );

    medianBlur(clipedImage, clipedImage, 15);

    cvtColor(clipedImage, clipedImage, CV_BGR2GRAY); 
}

void canny(const std::string& file)
{
    if (file.empty())
    {
        return;
    }

    Mat edge; 
    getEdge(file.c_str(),edge);
    // imwrite("canny.jpg", dst);
    imshow("part_edge", edge);

    CvMemStorage* storage1 = cvCreateMemStorage();
    CvSeq* first_contour1 = NULL;

    IplImage image = IplImage(edge);
    int Nc = cvFindContours(&image,
            storage1,
            &first_contour1,
            sizeof(CvContour),
            CV_RETR_EXTERNAL);

    cvZero(&image);

    CvSeq* first_contour2 = NULL;

    getEdge("F:\\us\\model.png",edge);
    imshow("whole_edge", edge);

    image = IplImage(edge);
    int count = cvFindContours(&image,
        storage1,
        &first_contour2,
        sizeof(CvContour),
        CV_RETR_EXTERNAL
    );

    CvMemStorage* all_sto;
    CvSeq* all_cont;
    CvSeq* cur_cont;

    all_sto = cvCreateMemStorage(0);
    all_cont = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvSeq), all_sto);

    first_contour1->total;
    for (CvSeq* c = first_contour1; c != NULL; c = c->h_next)
    {
        if (((CvContour*)c)->rect.height * ((CvContour*)c)->rect.width >=3000)
        {
            cvSeqPush(all_cont, c);
        }
    }

    cur_cont = (CvSeq*)cvGetSeqElem(all_cont, 0);

    if (cur_cont==NULL)
    {
        return;
    }

    CvRect arcRect = cvBoundingRect(cur_cont,1);
    CvRect rect = cvBoundingRect(first_contour2);

    CvPoint2D32f tempCenter;
    tempCenter.x = rect.x + 0.5*rect.width;
    tempCenter.y = rect.y + 0.5*rect.height;

    CvPoint2D32f center;
    center.y = arcRect.y + 0.5* rect.height;
    float deltay = arcRect.y - rect.y;
    float deltax = arcRect.x - rect.x;

    float step = (arcRect.width - rect.width) / 80.0;

    double map[6];
    CvMat map_matrix = cvMat(2, 3, CV_64FC1, map);

    IplImage* pSrcImg = NULL;
    IplImage* pRatationedImg = NULL;

    CvMemStorage* storage = cvCreateMemStorage();
    CvSeq* first_contour = NULL;

    Mat temp;
    pSrcImg = cvLoadImage("F:\\us\\model.png", 1);
    pRatationedImg = cvCreateImage(cvGetSize(pSrcImg), pSrcImg->depth, pSrcImg->nChannels);

    double min = std::numeric_limits<double>::max();
    double x;
    double y;
    double angle = 0;

    cv::Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1);
    t_mat.at<float>(0, 0) = 1;
    t_mat.at<float>(1, 1) = 1;
    Mat src = cvarrToMat(pSrcImg);
    cv::Size dst_sz = src.size();
    for (int j = 0; j < 80; ++j)
    {
        float tempX = deltax + step*j;
          t_mat.at<float>(0, 2) = deltax+step*j;
          t_mat.at<float>(1, 2) = deltay;  

          cv::warpAffine(src, temp, t_mat, dst_sz,1,0,cvScalarAll(255));
          //imshow("image", src);
          //imshow("moved",temp);

          medianBlur(temp, temp, 15);

          cvtColor(temp, temp, CV_BGR2GRAY); 

          Canny(temp, edge, 50, 150, 3);

          IplImage image = IplImage(edge);
          int count = cvFindContours(&image,
                                     storage,
                                     &first_contour2,
                                     sizeof(CvContour),
                                     CV_RETR_EXTERNAL);

            double d = cvMatchShapes(cur_cont, first_contour2, CV_CONTOURS_MATCH_I2, 0);
            if (d < min)
            {
                min = d;
                x = deltax + step*j;
                y = deltay;
            }
    }

    t_mat.at<float>(0, 2) = x;  
    t_mat.at<float>(1, 2) = y;
    Mat moved;
    cv::warpAffine(src, moved, t_mat, dst_sz, 1, 0, cvScalarAll(255));
    /*clock_t time1 = clock() - time;*/


    for (int j = 0; j < 10; ++j)
    {
        cv::Point2f center(tempCenter.x+x, tempCenter.y+y);  
        cv::Mat rot_mat = cv::getRotationMatrix2D(center, -15 + 3.0*j, 1.0);
 
        cv::warpAffine(moved, temp, rot_mat, dst_sz,1,0, cvScalarAll(255));
        // imshow("rotated",temp);

        medianBlur(temp, temp, 15);

        cvtColor(temp, temp, CV_BGR2GRAY); 

        Canny(temp, edge, 50, 150, 3);

        IplImage image = IplImage(edge);
        int count = cvFindContours(&image,
            storage,
            &first_contour2,
            sizeof(CvContour),
            CV_RETR_EXTERNAL
        );

        double d = cvMatchShapes(cur_cont, first_contour2, CV_CONTOURS_MATCH_I1, 0);
        if (d < min)
        {
            min = d;
            angle = -15 + 3.0*j;
        }
    }
 
    cv::Point2f newcenter(tempCenter.x + x, tempCenter.y + y);
 
    cv::Mat rot_mat = cv::getRotationMatrix2D(newcenter, angle, 1.0);


    cv::warpAffine(moved, temp, rot_mat, dst_sz, 1, 0, cvScalarAll(255));

    medianBlur(temp, temp, 15);

    cvtColor(temp, temp, CV_BGR2GRAY);

    Mat newedge;
    Canny(temp, newedge, 50, 150, 3);

    image = IplImage(newedge);
    count = cvFindContours(&image,
        storage,
        &first_contour2,
        sizeof(CvContour),
        CV_RETR_EXTERNAL
    );

    // cvSeqPush(all_cont, first_contour2);

    cvZero(&image);
    for (int i = 0; i < all_cont->total; ++i)
    {
        cur_cont = (CvSeq*)cvGetSeqElem(all_cont, i);
        double d = cvMatchShapes(cur_cont, first_contour2, CV_CONTOURS_MATCH_I1, 0);

        cvDrawContours(&image, cur_cont, cvScalarAll(255), cvScalarAll(255),
            100);

    }

    cvShowImage("contour", &image);
    // imshow("dst", dst);
    //waitKey();
}

void getEdge(const std::string& file,Mat& edge)
{
    Mat src = imread(file.c_str());
    Mat dst;

    Mat clipedImage;
    if (src.cols == 800)
    {
        clipedImage = src.rowRange(180, 450).colRange(255, 500);
    }
    else
    {
        clipedImage = src;
    }

    IplImage image = IplImage(clipedImage);
    IplImage* temp = cvCreateImage(
        cvGetSize(&image),
        IPL_DEPTH_8U,
        3
    );

    medianBlur(clipedImage, clipedImage, 15);
    Mat afterFilter = cv::cvarrToMat(temp);

    cvtColor(clipedImage, clipedImage, CV_BGR2GRAY);

    // Mat edge;
    Canny(clipedImage, edge, 50, 150, 3);
    // imshow("edge", edge);
}

void getEdgeFromIp1Image(IplImage* pImage, CvSeq* seq)
{
    if (pImage == NULL)
    {
        return;
    }

    Mat temp = cv::cvarrToMat(pImage);


    medianBlur(temp, temp, 15);
    // cvSmooth(&image, temp, CV_BLUR_NO_SCALE, 2, 2);

    //Mat afterFilter = cv::cvarrToMat(temp);
    // imshow("afterfilter", clipedImage);

    cvtColor(temp, temp, CV_BGR2GRAY);

    Mat edge;
    // Mat edge;
    Canny(temp, edge, 50, 150, 3);

    CvMemStorage* storage = cvCreateMemStorage();
    CvSeq* first_contour = NULL;

    IplImage image = IplImage(edge);
    cvZero(&image);
    // findContours(image, first_contour2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int count = cvFindContours(&image,
        storage,
        &seq,
        sizeof(CvContour),
        CV_RETR_EXTERNAL
    );
}

void getImageEdge(const IplImage* pImage, Mat& mat)
{
    Mat src = cv::cvarrToMat(pImage);
    Mat dst;

    Mat clipedImage;
    if (src.cols == 800)
    {
        clipedImage = src.rowRange(180, 450).colRange(255, 500);
    }
    else
    {
        clipedImage = src;
    }

    IplImage image = IplImage(clipedImage);
    IplImage* temp = cvCreateImage(
        cvGetSize(&image),
        IPL_DEPTH_8U,
        3
    );

    medianBlur(clipedImage, clipedImage, 15);
    Mat afterFilter = cv::cvarrToMat(temp);

    cvtColor(clipedImage, clipedImage, CV_BGR2GRAY);

    Canny(clipedImage, mat, 50, 150, 3);
}

void imageRotate(const std::string& file)
{
#if 0
    IplImage* pSrcImg = NULL;
    IplImage* pRatationedImg = NULL;

    Mat edge;
    getEdge("F:\\us\\model.png", edge);
    imshow("raw_edge", edge);

    IplImage image = IplImage(edge);

    CvMemStorage* storage1 = cvCreateMemStorage();
    CvSeq* first_contour = NULL;

    // findContours(image, first_contour2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int count = cvFindContours(&image,
        storage1,
        &first_contour,
        sizeof(CvContour),
        CV_RETR_EXTERNAL
    );

    CvBox2D rect = cvMinAreaRect2(first_contour);

    pSrcImg = cvLoadImage("F:\\us\\model.png", 1);
    pRatationedImg = cvCreateImage(cvGetSize(pSrcImg), pSrcImg->depth, pSrcImg->nChannels);

  
    CvPoint2D32f center = rect.center; 
    center.x = rect.center.x - 50;
    center.y = rect.center.y - 50;
    double map[6];

    CvMat map_matrix = cvMat(2, 3, CV_64FC1, map);
    cv2DRotationMatrix(center, 3.0, 1.0, &map_matrix);
    cvWarpAffine(pSrcImg, pRatationedImg, &map_matrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(255));

    cvShowImage("raw_image", pSrcImg);
    cvShowImage("rotated_image", pRatationedImg);
#endif

    cv::Mat src = cv::imread("F:\\us\\model.png");
    cv::Mat dst;

    cv::Size dst_sz = src.size();

    //cv::Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1);

    //t_mat.at<float>(0, 0) = 1;
    //t_mat.at<float>(0, 2) = 20; 
    //t_mat.at<float>(1, 1) = 1;
    //t_mat.at<float>(1, 2) = 10;  
    cv::Point2f center(130,126);
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, 15, 1.0);  
    cv::warpAffine(src, dst, rot_mat, dst_sz, 1 , 0, 0);

    cv::imshow("image", src);
    cv::imshow("result", dst);

    cv::waitKey(0);
}

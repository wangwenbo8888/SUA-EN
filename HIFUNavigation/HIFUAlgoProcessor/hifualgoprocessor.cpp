#include "hifualgoprocessor.h"
#include <QDir>
#include <qdebug.h>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

HIFUAlgoProcessor::HIFUAlgoProcessor()
{
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


void HIFUAlgoProcessor::SeededGrow3dProcess(short *data, int width, int heigt, int depth, int seedx, int seedy, int seedz, int thresholdmin, int thresholdmax,uchar* outimage)
{
    SeededGrow3d SGProcess(NULL);
    SGProcess.max=thresholdmax;
    SGProcess.min=thresholdmin;

    BitMap3d bmp(data,width,heigt,depth);
    Int16Triple seed(seedx,seedy,seedz);
    SGProcess.ExcuteSpanFill(&bmp,seed,outimage);

}

void HIFUAlgoProcessor::DataPostProcess(int width, int height, int depth,uchar* mask)
{
    using namespace cv;
    for (int i = 0; i < depth;++i)
    {
        int index = width*height*i;
        cv::Mat srcBw(height, width, CV_8UC1, &mask[index]);

        Size m_Size = srcBw.size();
        Mat Temp=Mat::zeros(m_Size.height+2,m_Size.width+2,srcBw.type());//延展图像
        srcBw.copyTo(Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));

        cv::floodFill(Temp, Point(0, 0), Scalar(255));

        Mat cutImg;//裁剪延展的图像
        Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);

        Mat dstBw;
        dstBw = srcBw | (~cutImg);

        DataEdgeSmooth(dstBw,srcBw,4,4,1);

        uchar* pDest = &mask[index];
        uchar* pSource = dstBw.data;
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                if (*pSource==255)
                {
                    *pDest = 1;
                }
                else
                {
                    *pDest=*pSource;
                }

                ++pSource;
                ++pDest;
            }
        }
    }
}


void HIFUAlgoProcessor::SurfaceRenderingProcess(short* data, int width, int heigt, int depth, double spacingX,double spacingY,double spacingZ)
{
    BitMap3d bmp(data, width, heigt, depth);
    SMC_AdSMCProcessor smc(&bmp, spacingX,spacingY,spacingZ,NULL);
    Mesh m;
    smc.GenerateSurface(&m);
    // m.HCLaplacin(4, 0.4, 0.0);
    m.Taubin(4,0.8,0.0);
    //        vector<Point3d> inters;
    //        VectorOfPlaneCrossMesh(&m, SMC::Point3d(0, 0, 0), SMC::Point3d(0, 0, 1), inters);
    //        for (int i = 0; i < inters.size(); i++)
    //        {
    //            SMC::Point3d p = inters[i];
    //            //int index = p.X + p.Z * 320;
    //            //edgedata[index] = 255;
    //            pair<int, int> value;
    //            value.first = p.X;
    //            value.second = p.Z;
    //            vdata.push_back(value);
    //        }
    QDir dir;
    QString path=dir.currentPath();
    path.append("/Tumor3d.stl");
    qDebug()<<"tumor path is "<<path;
    m.WriteStl(path.toStdString().c_str());
}

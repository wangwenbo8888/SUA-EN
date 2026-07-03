#include "imagecapture.h"
#include <QDebug>
ImageCapture* myCapture;
long        tnum;
const int IMAGE_LEFT = 0;
const int IMAGE_TOP = 0;
const int IMAGE_RIGHT = 1024;
const int IMAGE_BOTTOM = 768;

ImageCapture::ImageCapture(QObject *parent) : QThread(parent),
    m_enableVideoRecord(false),m_isCapture(false)
{
    myCapture = this;
}

ImageCapture::~ImageCapture()
{
}

void ImageCapture::run()
{
}

void ImageCapture::startCapture()
{
    stopCapture();
    if (initCapture())
    {
        okSetSeqCallback(m_hBoard, beginCapture, process, endCapture);
        setCapture(true);
        okCaptureTo(m_hBoard, BUFFER, 0, -1);
    }
}

void ImageCapture::stopCapture()
{
//  TODO
//  determine whether the capture is doing now, then stop capturing if so
    if (isCapture())
    {
        okStopCapture(m_hBoard);
        okCloseBoard(m_hBoard);
        setCapture(false);
    }
}

void ImageCapture::updateImage(QImage image)
{
    emit imageUpdate(image);
}

void ImageCapture::saveImage(QString fileName)
{
    if (isCapture())
    {
        long frmIndex = okGetCaptureStatus(m_hBoard, 0);
        short sImageWidth, sImageHeight;
        long lImageStride;
        LPBYTE lpCurrImage = (LPBYTE)okGetTargetInfo(m_hBoard,BUFFER,frmIndex,
                                                     &sImageWidth,&sImageHeight,
                                                     &lImageStride);
        QImage image = QImage(lpCurrImage,sImageWidth,sImageHeight,QImage::Format_RGB888);

        image.save(fileName,"jpg",100);
    }else
    {
        //  TODO: emit a signal
    }
}

void ImageCapture::startSaveVideo(char *fileName)
{
    setEnableVideoRecord(true);
    setVideoName(fileName);
    startCapture();
}

void ImageCapture::stopSaveVideo()
{
    stopCapture();
    setEnableVideoRecord(false);
}

bool ImageCapture::initCapture()
{
    MLONG lIndex = -1;
    m_hBoard = okOpenBoard(&lIndex);
    if (m_hBoard)
    {
        okSetCaptureParam(m_hBoard, CAPTURE_BUFRGBFORMAT, FORM_RGB888);
        okSetCaptureParam(m_hBoard, CAPTURE_BUFBLOCKSIZE, 0);
        return true;
    }else
    {
        emit captureFail();
        return false;
    }
}

BOOL CALLBACK beginCapture(HANDLE hBoard)
{
    RECT rect;
    rect.left = IMAGE_LEFT;
    rect.right = IMAGE_RIGHT;
    rect.bottom = IMAGE_BOTTOM;
    rect.top = IMAGE_TOP;
//    RECT rect;
//    SetRect(*rect,0,0,1024,768);

    MLONG frmNum = okSetTargetRect(hBoard, BUFFER, &rect);

    if (myCapture->enableVideoRecord())
    {
        char avif[] = "../Monitoring.avi,MPG4,beg";
        char* aviFileName = avif;
        okSaveImageFile(hBoard,aviFileName,0,BUFFER,0,1);
    }

    return 1;
}

BOOL CALLBACK process(HANDLE hBoard, long frmIndex)
{

    qDebug()<<"进入采集了============================>";
    short sImageWidth, sImageHeight;
    long lImageStride;
    LPBYTE lpCurrImage = (LPBYTE)okGetTargetInfo(hBoard,BUFFER,frmIndex,
                                                 &sImageWidth,&sImageHeight,
                                                 &lImageStride);
    QImage image = QImage(lpCurrImage,sImageWidth,sImageHeight,QImage::Format_RGB888);
    myCapture->updateImage(image);

//    if (myCapture->enableVideoRecord())
//    {

        qDebug()<<"开始采集了============================>";
        //old
//        char* aviFileName = "../Monitoring.avi,MPG4,con";
//        okSaveImageFile(hBoard,aviFileName,0,BUFFER,0,1);
        //new
        char  a[80];
        sprintf(a,"image.avi,H264,con");
        okSaveImageFile(hBoard,a,tnum,BUFFER,frmIndex ,1);
        tnum++;
  //
//    }

    return 1;
}

BOOL CALLBACK endCapture(HANDLE hBoard)
{
    myCapture->exit();
    okSetSeqCallback(hBoard,NULL,NULL,NULL);
    okStopCapture(hBoard);

    if (myCapture->enableVideoRecord())
    {
        char avif[] = "../Monitoring.avi,MPG4,end";
        char* aviFileName = avif;
        okSaveImageFile(hBoard,aviFileName,0,BUFFER,0,1);
    }

    return 1;
}

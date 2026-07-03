#include "imagegrabber.h"
#include <QDebug>

#include <iostream>
#include <QFile>

#include <limits>

bool savevideo=false;
char* filebeg;
char* filecon;
char* fileend;
QByteArray ba1;
QByteArray ba2;
QByteArray ba3;
long snum=0;
long count = 0;

ImageGrabber* grabber;
QRETURN on_process_signal_removed( PVOID pDevice, ULONG nVideoInput, ULONG nAudioInput, PVOID pUserData )
{
    // ChannelWindow* m_pChannelWindow = (ChannelWindow*)pUserData;

    qDebug("signal removed \n");

    // m_pChannelWindow->str_info.sprintf("info...(signal removed)");

    return QCAP_RT_OK;
}

QRETURN on_process_no_signal_detected( PVOID pDevice, ULONG nVideoInput, ULONG nAudioInput, PVOID pUserData )
{
    // ChannelWindow* m_pChannelWindow = (ChannelWindow*)pUserData;

    ImageGrabber* grabber = (ImageGrabber*)pUserData;
    grabber->noSignal();
    qDebug("no signal Detected  \n");

    // m_pChannelWindow->str_info.sprintf("info...(no signal detected)");

    return QCAP_RT_OK;
}

QRETURN on_process_format_changed( PVOID pDevice, ULONG nVideoInput, ULONG nAudioInput,
                                   ULONG nVideoWidth, ULONG nVideoHeight, BOOL bVideoIsInterleaved,
                                   double dVideoFrameRate, ULONG nAudioChannels, ULONG nAudioBitsPerSample,
                                   ULONG nAudioSampleFrequency, PVOID pUserData )
{
    // ChannelWindow* m_pChannelWindow = (ChannelWindow*)pUserData;

    qDebug("Image grabber format changed Detected  \n");

    ImageGrabber* grabber = (ImageGrabber*)pUserData;

    grabber->m_nVideoWidth = nVideoWidth;

    grabber->m_nVideoHeight = nVideoHeight;

    grabber->m_dVideoFrameRate = dVideoFrameRate;

    grabber->m_nAudioChannels = nAudioChannels;

    grabber->m_nAudioBitsPerSample = nAudioBitsPerSample;

    grabber->m_nAudioSampleFrequency = nAudioSampleFrequency;

    grabber->tmpBuffer = (BYTE*)malloc(nVideoWidth * nVideoHeight * 3);

    //CHAR strVideoInput[ MAX_PATH ] = { 0 };

    //CHAR strAudioInput[ MAX_PATH ] = { 0 };

    //CHAR strFrameType[ MAX_PATH ] = { 0 };

    grabber->onSignal();

    return QCAP_RT_OK;
}

#define BigtoLittle16(A) ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))

QRETURN on_video_preview_callback(PVOID pDevice, double dSampleTime, BYTE* pFrameBuffer,
                                   ULONG nFrameBufferLen, PVOID pUserData)
{
    // qDebug(QString("Image grabber enter preview callback %1 !").arg(nFrameBufferLen).toStdString().c_str());
    // qDebug()<< "Image grabber enter preview callback " << QString::number(nFrameBufferLen) ;

    count++;
    if (count%2==0)
    {
        // 采集减半
        return  QCAP_RT_OK;
    }

    if (nFrameBufferLen==0 || pFrameBuffer==nullptr)
    {
        return QCAP_RT_FAIL;
    }

    ImageGrabber* grabber = (ImageGrabber*)pUserData;

//    QFile file("D:\\file.bin");
//    if (file.open(QIODevice::WriteOnly))
//    {
//        file.write((const char*)pFrameBuffer,nFrameBufferLen);
//    }

//    file.close();

    if (grabber->recordFlag)
    {

        qDebug()<<"Enter record !";

        for (int i = 0; i < grabber->m_nVideoHeight / 2; ++i)
        {
            memcpy(grabber->tmpBuffer + (grabber->m_nVideoHeight - i - 1) * grabber->m_nVideoWidth * 3, pFrameBuffer + i * grabber->m_nVideoWidth * 3,
               grabber->m_nVideoWidth * 3);

            memcpy(grabber->tmpBuffer + i * grabber->m_nVideoWidth * 3, pFrameBuffer + (grabber->m_nVideoHeight - i - 1) * grabber->m_nVideoWidth * 3,
               grabber->m_nVideoWidth * 3);
        }

        QCAP_SET_VIDEO_SHARE_RECORD_UNCOMPRESSION_BUFFER_EX(0, QCAP_COLORSPACE_TYPE_BGR24, grabber->m_nVideoWidth, grabber->m_nVideoHeight,
                                                        grabber->tmpBuffer, nFrameBufferLen, REC_X, REC_Y, REC_WIDTH, REC_HEIGHT);
    }

    uint64_t size = IMAGE_WIDTH*IMAGE_HIGHT;
    // qDebug()<<"buff size is "<< nFrameBufferLen;
    // qDebug()<<"dest image size is "<<size;

    QImage dest(IMAGE_WIDTH, IMAGE_HIGHT, QImage::Format_RGB888);

//    yuv420p_to_rgb(pFrameBuffer,IMAGE_WIDTH,IMAGE_HIGHT,dest);

//    grabber->refresh(dest.copy(REC_X, REC_Y, REC_WIDTH, REC_HEIGHT));

//    return QCAP_RT_OK;
    /*
    uint16_t min = (std::numeric_limits<uint16_t>::max)();
    uint16_t max = (std::numeric_limits<uint16_t>::min)();

    uint16_t* pSource = (uint16_t*)pFrameBuffer;
    for (int i = 0; i < size; ++i)
    {
         uint16_t data = BigtoLittle16(*pSource);
        if (data < min)
        {
            min = data;
        }

        if (data > max)
        {
            max = data;
        }

        pSource++;
    }

    */

    //qDebug()<< "Image grabber min is " << QString::number(min);
    //qDebug()<< "Image grabber max is " << QString::number(max);

    uint8_t* pSource = (uint8_t*)pFrameBuffer;
    //QImage dest(IMAGE_WIDTH, IMAGE_HIGHT, QImage::Format_RGB888);
    uint8_t* pDest = (uint8_t*)dest.bits();
    for (int i = 0; i < size; i++)
    {
        //uint8_t data = pSource);

        // uint8_t value = static_cast<uint8_t>(floor(255*(data - min) / (max - min)));
        uint8_t b = *pSource++;
        uint8_t g = *pSource++;
        //uint8_t r = 255;
        uint8_t r = *pSource++;

        *(pDest++) = r;  // B
        *(pDest++) = g;  // G
        *(pDest++) = b;  // R

        //qDebug()<< "Image grabber Source is " << QString::number(data);
        //qDebug()<< "Image grabber value is " << QString::number(value);

        // pSource++;
    }

    // 沿水平方向镜像
    QImage fliped = dest.mirrored(false,true);

    // fliped.save("D:\\us.jpg");

    //qDebug("Image grabber after change to qimage !");

    grabber->refresh(fliped.copy(REC_X, REC_Y, REC_WIDTH, REC_HEIGHT));

    //qDebug("Image grabber after refresh !");

    return QCAP_RT_OK;
}

QRETURN on_process_preview_audio_buffer( PVOID pDevice, double dSampleTime, BYTE* pFrameBuffer,
                                         ULONG nFrameBufferLen, PVOID pUserData)
{
    ImageGrabber* m_pMainDialog = (ImageGrabber*)pUserData;

    if (m_pMainDialog->recordFlag)
    {
           QCAP_SET_AUDIO_SHARE_RECORD_UNCOMPRESSION_BUFFER(0, pFrameBuffer, nFrameBufferLen);
    }

    return QCAP_RT_OK;
}

ImageGrabber::ImageGrabber(QObject *parent) : QObject(parent)
{
    grabber = this;
    m_isOpen = false;
    m_isWorking = false;
    m_gray = NULL;
//    m_clipable = false;

    recordFlag = FALSE;
    Initialize();
}

ImageGrabber::~ImageGrabber()
{
    stop();

    if (recordFlag)
    {
        recordFlag = false;

        QCAP_STOP_SHARE_RECORD(0);
    }

    if (m_gray!=NULL)
    {
        delete []m_gray;
        m_gray = NULL;
    }

    if (tmpBuffer != nullptr)
    {

        free(tmpBuffer);
    }
}

bool ImageGrabber::open()
{
    /*
    if (isOpen()) return true;

    MLONG index = -1;
    int extsign=0;
    m_board = okOpenBoard(&index);
    extsign=okGetSignalParam(m_board,SIGNAL_VIDEOEXIST);
    //if (m_board)
    if (extsign){
        okSetCaptureParam(m_board, CAPTURE_BUFRGBFORMAT, FORM_RGB888);
        okSetCaptureParam(m_board, CAPTURE_BUFBLOCKSIZE, 0);
        emit statusUpdated(IMAGE_GRABBER_AVAILABLE);
        m_isOpen = true;
    }else emit statusUpdated(IMAGE_GRABBER_NONAVAILABLE);

    return isOpen();
    */
    emit statusUpdated(IMAGE_GRABBER_AVAILABLE);

    return true;
}

void ImageGrabber::work()
{
    /*
    if (isWorking())
        return;

    okSetSeqCallback(m_board, beginCapture, process, endCapture);
    okCaptureTo(m_board, BUFFER, 0, -1);
    m_isWorking = true;
    */
}

void ImageGrabber::stop()
{
    /*
    if (isWorking()) {
        okStopCapture(m_board);
        okCloseBoard(m_board);
        m_isWorking = false;
    }
    emit finished();
    */

    Uninitialize();
}

void ImageGrabber::startSaveVideo(QString fileName)
{
    if (recordFlag) {

        recordFlag = false;

        QCAP_STOP_RECORD(m_hVideoDevice, 0);
    }

    ULONG Encoder;

       QRESULT rt = QCAP_QUERY_ENCODER_TYPE_CAP(0, QCAP_ENCODER_TYPE_NVIDIA_NVENC, QCAP_ENCODER_FORMAT_H264);

       if (rt != QCAP_RS_ERROR_NON_SUPPORT)
           Encoder = QCAP_ENCODER_TYPE_NVIDIA_NVENC;
       else {
           rt = QCAP_QUERY_ENCODER_TYPE_CAP(0, QCAP_ENCODER_TYPE_INTEL_MEDIA_SDK, QCAP_ENCODER_FORMAT_H264);

           if (rt != QCAP_RS_ERROR_NON_SUPPORT)
               Encoder = QCAP_ENCODER_TYPE_INTEL_MEDIA_SDK;
           else
               Encoder = QCAP_ENCODER_TYPE_SOFTWARE;
       }

       // QCAP_SET_VIDEO_SHARE_RECORD_UNCOMPRESSION_BUFFER_EX(0,QCAP_COLORSPACE_TYPE_YUY2,);

       // 截取视频帧率减半
       QCAP_SET_VIDEO_SHARE_RECORD_PROPERTY(0, QCAP_ENCODER_TYPE_INTEL_MEDIA_SDK, QCAP_ENCODER_FORMAT_H264, QCAP_COLORSPACE_TYPE_YUY2,
                                            IMAGE_WIDTH, IMAGE_HIGHT, m_dVideoFrameRate*0.5, QCAP_RECORD_MODE_CBR, 8000, 6 * 1024 * 1024, 30, 0, 0, 0, 0, 0);

       QCAP_SET_AUDIO_SHARE_RECORD_PROPERTY(0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_PCM, m_nAudioChannels, m_nAudioBitsPerSample, m_nAudioSampleFrequency);

       QByteArray record2_path_ByteArray = fileName.toLocal8Bit();

       CHAR* psz = record2_path_ByteArray.data();

       QString file(psz);
       rt = QCAP_START_SHARE_RECORD(0, psz, QCAP_RECORD_FLAG_FULL | QCAP_RECORD_FLAG_VIDEO_ONLY);
       if (rt==QCAP_RS_SUCCESSFUL)
       {
           qDebug()<<QString("start share record success %1 !").arg(file);
       }
       else
       {
           qDebug()<<QString("start share record failed !").arg(file);
       }

       recordFlag = true;

       return;

    QRESULT return_type = QCAP_RS_ERROR_GENERAL;

    // QString record2_path;

//    if( m_bSupportGPU2 == TRUE )
//    {
//        QCAP_SET_VIDEO_RECORD_PROPERTY(m_hVideoDevice, 1, QCAP_ENCODER_TYPE_INTEL_MEDIA_SDK,
//                                       QCAP_ENCODER_FORMAT_H264, QCAP_RECORD_MODE_CBR, 8000, 4 * 1024 * 1024, 30, 0, 0,
//                                       QCAP_DOWNSCALE_MODE_OFF);
//    }
//    else
    {
        //QRESULT res = QCAP_SET_VIDEO_RECORD_PROPERTY(m_hVideoDevice, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_RAW_NATIVE,
        //                               QCAP_RECORD_MODE_CBR, 8000, 8 * 1024 * 1024, 30, 0, 0, QCAP_DOWNSCALE_MODE_OFF);

        QRESULT res = QCAP_SET_VIDEO_RECORD_PROPERTY_EX(m_hVideoDevice, 0,0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_H264,
                                          QCAP_RECORD_PROFILE_BASELINE,QCAP_RECORD_LEVEL_41,QCAP_RECORD_ENTROPY_CAVLC,1,
                                          QCAP_RECORD_MODE_CBR, 8000, 12 * 1024 * 1024, 30, 0, FALSE, 0,
                                          0,40,TRUE,FALSE,FALSE,0,0,0,0,0,0,0,
                                          REC_X,REC_Y,REC_WIDTH,REC_HEIGHT,REC_WIDTH,REC_HEIGHT,0,0);
        if(res == QCAP_RS_SUCCESSFUL)
        {
            qDebug("Set video para success !");
        }
        else
        {
            qDebug("Set video para failed !");
        }

         // QCAP_SET_VIDEO_RECORD_PROPERTY(m_hVideoDevice, 0, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_H264,
         // QCAP_RECORD_MODE_CBR, 8000, 8 * 1024 * 1024, 30, 0, 0, QCAP_DOWNSCALE_MODE_OFF );
    }


//    if( m_bMP4_12 )
//    {
//        record2_path = path + QString("/record_2.mp4");

//        QCAP_SET_AUDIO_RECORD_PROPERTY(m_hVideoDevice, 1, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_AAC );
//    }
//    else
//    {
//        // record2_path = path + QString("/record_2.avi");

//        QCAP_SET_AUDIO_RECORD_PROPERTY(m_hVideoDevice, 1, QCAP_ENCODER_TYPE_SOFTWARE, QCAP_ENCODER_FORMAT_PCM );
//    }

    // QByteArray record2_path_ByteArray = record2_path.toLocal8Bit();

//    QByteArray record2_path_ByteArray = fileName.toLocal8Bit();

//    CHAR* psz = record2_path_ByteArray.data();

//    return_type = QCAP_START_RECORD(m_hVideoDevice, 0, psz);

    if(return_type == QCAP_RS_SUCCESSFUL)
    {
        qDebug(QString("start record 1_1 success，path %1 !").arg(QString(psz)).toStdString().c_str());
    }
    else
    {
        qDebug(QString("start record 1_1 failed,path %1 !").arg(QString(psz)).toStdString().c_str());
    }
}

void ImageGrabber::stopSaveVideo()
{
    //  m_enableVideoRecord=false;
    //  savevideo=m_enableVideoRecord;

    recordFlag = FALSE;
    if (QCAP_RS_SUCCESSFUL==QCAP_STOP_SHARE_RECORD(0))
    {
        qDebug("Stop record 1_1 success !");
    }
    else
    {
        qDebug("Stop record 1_1 failed !");
    }
}

void ImageGrabber::refresh(QImage image)
{
    emit imageRefreshed(image);
}

void ImageGrabber::onSignal()
{
    emit on_signal();
}

void ImageGrabber::noSignal()
{
    emit no_signal();
}

void ImageGrabber::Initialize()
{
    // CREATE CAPTURE DEVICE
    QCAP_CREATE("SC0710 PCI", 0, nullptr, &m_hVideoDevice, TRUE);

    // QCAP_SET_VIDEO_DEFAULT_OUTPUT_FORMAT(m_hVideoDevice,QCAP_COLORSPACE_TYEP_BGR24,0,0,0,0);

    QCAP_REGISTER_NO_SIGNAL_DETECTED_CALLBACK(m_hVideoDevice, on_process_no_signal_detected, this);

    QCAP_REGISTER_SIGNAL_REMOVED_CALLBACK(m_hVideoDevice, on_process_signal_removed, this);

    QCAP_REGISTER_FORMAT_CHANGED_CALLBACK(m_hVideoDevice, on_process_format_changed, this);

    QCAP_REGISTER_VIDEO_PREVIEW_CALLBACK(m_hVideoDevice, on_video_preview_callback, this);

    QCAP_REGISTER_AUDIO_PREVIEW_CALLBACK(m_hVideoDevice, on_process_preview_audio_buffer, this);

    QCAP_SET_VIDEO_INPUT(m_hVideoDevice, QCAP_INPUT_TYPE_AUTO);

    QCAP_SET_AUDIO_INPUT(m_hVideoDevice, QCAP_INPUT_TYPE_EMBEDDED_AUDIO);

//    QCAP_SET_AUDIO_VOLUME(m_hVideoDevice, 100);

//    QCAP_SET_VIDEO_DEINTERLACE(m_hVideoDevice, 1);

    QCAP_SET_DEVICE_CUSTOM_PROPERTY(m_hVideoDevice, 380, 1);

    QCAP_SET_VIDEO_DEFAULT_OUTPUT_FORMAT(m_hVideoDevice, QCAP_COLORSPACE_TYPE_BGR24, 0, 0, 0, 0.0);

    QCAP_RUN(m_hVideoDevice);

    if(m_hVideoDevice == 0)
    {
        qDebug("create fail  \n");
        m_isOpen = false;
    }
    else
    {
        m_isOpen = true;
    }
}

void ImageGrabber::Uninitialize()
{
    if(m_hVideoDevice)
    {
        QCAP_STOP(m_hVideoDevice);

        QCAP_DESTROY(m_hVideoDevice);

        m_hVideoDevice = NULL;
    }

    CoUninitialize();
}

BOOL CALLBACK beginCapture(HANDLE board)
{
    /*
    RECT rect;
    rect.left = IMAGE_LEFT;
    rect.right = IMAGE_RIGHT;
    rect.bottom = IMAGE_BOTTOM;
    rect.top = IMAGE_TOP;
//    SetRect(&rect, IMAGE_LEFT, IMAGE_TOP, IMAGE_RIGHT, IMAGE_BOTTOM);
    okSetTargetRect(board, BUFFER, &rect);

    if (savevideo)
    {
        okSaveImageFile(board, filebeg, 0, BUFFER, 0, 1);
        snum=1;
    }
    */
    return true;
}

BOOL CALLBACK process(HANDLE board, long frameIndex)
{
    /*
    if (savevideo)
    {

        if(snum==0)
        {
            okSaveImageFile(board, filebeg, snum, BUFFER, 0, 1);
        }
        else{
            okSaveImageFile(board, filecon, snum, BUFFER, frameIndex, 1);
        }
        snum++;
    }
    else if(snum!=0){
        okSaveImageFile(board, fileend, 0, BUFFER, 0, 1);
        snum=0;
    }

    short width, height;
    long stride;
    LPBYTE currentImage = (LPBYTE)okGetTargetInfo(board,BUFFER,frameIndex,
                                                 &width, &height, &stride);
#ifdef DEBUG
    printf("width = %d, height = %d, stride = %ld\n", width, height, stride);
#endif
    // 通道转换
    BYTE* pTemp = currentImage;
//    if (grabber!=NULL&&grabber->m_gray==NULL)
//    {
//        grabber->m_gray = new BYTE[height*width];
//    }

    // BYTE* pGray = grabber->m_gray;
    BYTE r;
    // Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B;
    for (int i = 0;i<height;++i)
    {
        for (int j = 0; j < width; ++j)
        {
           r = *pTemp;
           *pTemp = *(pTemp+2);
           *(pTemp+2) = r;
           //*pGray = 0.29900*(*(pTemp+2))+0.58700*(*pTemp+1)+0.11400*(*pTemp);
           pTemp = pTemp+3;
           //++pGray;
        }
    }

    QImage image = QImage(currentImage, width, height, QImage::Format_RGB888);
    //QImage image = QImage(grabber->m_gray, width, height, QImage::Format_Indexed8);
    grabber->refresh(image.copy(REC_X, REC_Y, REC_WIDTH, REC_HEIGHT));

//    delete []gray;
*/
    return true;
}

BOOL CALLBACK endCapture(HANDLE board)
{
    /*
    okSetSeqCallback(board, NULL, NULL, NULL);
    okStopCapture(board);

    if (savevideo&&snum!=0)
    {
        okSaveImageFile(board, fileend, 0, BUFFER, 0, 1);
    }
    */

    return true;
}

// 彩色图像转换成灰度图像
QImage toGray(QImage image)
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
        break;

    case QImage::Format_RGB888:
        break;

    }

    return ret;
}

QImage convertYV12ToQImage(const uchar *data, int width, int height)
{
    QImage image(width, height, QImage::Format_RGB32);

    // 获取Y、U、V平面的起始位置
    const uchar *yPlane = data;
    const uchar *uPlane = data + width * height;
    const uchar *vPlane = data + width * height + (width * height) / 4;

    // 遍历每个像素
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // 获取Y、U、V值
            uchar yValue = yPlane[y * width + x];
            uchar uValue = uPlane[(y / 2) * (width / 2) + (x / 2)];
            uchar vValue = vPlane[(y / 2) * (width / 2) + (x / 2)];

            // 转换为RGB值
            int d = (1.164 * (yValue - 16)) + (1.596 * (vValue - 128));
            int r = qBound(0, d, 255);

            int e = (1.164 * (yValue - 16)) - (0.813 * (uValue - 128)) - (0.391 * (vValue - 128));
            int g = qBound(0, e, 255);

            int f = (1.164 * (yValue - 16)) + (2.018 * (uValue - 128));
            int b = qBound(0, f, 255);

            // 设置像素颜色
            image.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return image;
}

QImage convertYUY2ToQImage(const uchar *data, int width, int height)
{
    QImage image(width, height, QImage::Format_RGB32);

    // 遍历每个像素
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int index = y * width * 2 + x * 2;

            // 获取Y、U、V值
            uchar y1 = data[index];
            uchar u = data[index + 1];
            uchar y2 = data[index + 2];
            uchar v = data[index + 3];

            // 转换为RGB值
            int d1 = (1.164 * (y1 - 16)) + (1.596 * (v - 128));
            int r1 = qBound(0, d1, 255);
            int e1 = (1.164 * (y1 - 16)) - (0.813 * (u - 128)) - (0.391 * (v - 128));
            int g1 = qBound(0, e1, 255);
            int f1 = (1.164 * (y1 - 16)) + (2.018 * (u - 128));
            int b1 = qBound(0, f1 , 255);

            int d2 = (1.164 * (y2 - 16)) + (1.596 * (v - 128));
            int r2 = qBound(0, d2 , 255);
            int e2 = (1.164 * (y2 - 16)) - (0.813 * (u - 128)) - (0.391 * (v - 128));
            int g2 = qBound(0, e2, 255);
            int f2 = (1.164 * (y2 - 16)) + (2.018 * (u - 128));
            int b2 = qBound(0, f2, 255);

            // 设置像素颜色
            if (x % 2 == 0) // 第一个像素
            {
                image.setPixelColor(x, y, QColor(r1, g1, b1));
            }
            else // 第二个像素
            {
                image.setPixelColor(x, y, QColor(r2, g2, b2));
            }
        }
    }

    return image;
}


void yuv420p_to_rgb(const unsigned char* yuvData, int width, int height, QImage& rgbImage)
{
    // 创建一个与输入图像大小相同的RGB图像

    int frameSize = width * height;
    const unsigned char* yPlane = yuvData;
    const unsigned char* uPlane = yuvData + frameSize;
    const unsigned char* vPlane = yuvData + frameSize + (frameSize / 4);

     uint8_t* pDest = (uint8_t*)rgbImage.bits();
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            int y = yPlane[j * width + i];
            int u = uPlane[(j / 2) * (width / 2) + (i / 2)];
            int v = vPlane[(j / 2) * (width / 2) + (i / 2)];

            // 将YUV值转换到正确的范围
            y = y < 16 ? 16 : y;
            u = u < 16 ? 16 : u;
            v = v < 16 ? 16 : v;

            // YUV to RGB conversion
            int r = static_cast<int>(y + 1.402 * (v - 128));
            int g = static_cast<int>(y - 0.344136 * (u - 128) - 0.714136 * (v - 128));
            int b = static_cast<int>(y + 1.772 * (u - 128));

            // 防止溢出

            r = min(max(r, 0), 255);
            g = min(max(g, 0), 255);
            b = min(max(b, 0), 255);

            pDest[3*(j*width+i)] = static_cast<uchar>(r);
            pDest[3*(j*width+i)+1] = static_cast<uchar>(g);
            pDest[3*(j*width+i)+2] = static_cast<uchar>(b);

//            rgbImage.at<cv::Vec3b>(j, i)[2] = static_cast<uchar>(r); // R
//            rgbImage.at<cv::Vec3b>(j, i)[1] = static_cast<uchar>(g); // G
//            rgbImage.at<cv::Vec3b>(j, i)[0] = static_cast<uchar>(b); // B
        }
    }
}

#ifndef IMAGEGRABBER_H
#define IMAGEGRABBER_H

#include <QObject>
#include <QImage>

#include "variable.h"
#include "windows.h"
#include "QCAP.H"
#include "imagegrabber_global.h"

const int IMAGE_LEFT = 0;
const int IMAGE_TOP = 0;
const int IMAGE_WIDTH = 1024;
const int IMAGE_HIGHT = 768;
const int REC_X = 265;
const int REC_Y = 100;
const int REC_WIDTH = 576;
const int REC_HEIGHT = 576;

QImage convertYV12ToQImage(const uchar *data, int width, int height);

QImage convertYUY2ToQImage(const uchar *data, int width, int height);

void yuv420p_to_rgb(const unsigned char* yuvData, int width, int height, QImage& rgbImage);

class IMAGEGRABBERSHARED_EXPORT ImageGrabber : public QObject
{
    Q_OBJECT

public:
    ImageGrabber(QObject *parent=0);
    ~ImageGrabber();

    bool open();
    void refresh(QImage image);

    void startSaveVideo(QString fileName);
    void stopSaveVideo();

    BYTE* m_gray;

signals:
    void imageRefreshed(QImage);
    void finished();
    void statusUpdated(DataFlag);

public slots:
//    void setClip(char* fileName);
    void work();
    void stop();

public:
    double m_dVideoFrameRate;
    ULONG m_nVideoWidth;

    ULONG m_nVideoHeight;

    ULONG m_nAudioChannels;

    ULONG m_nAudioBitsPerSample;

    ULONG m_nAudioSampleFrequency;

    BYTE* tmpBuffer;

    BOOL recordFlag;

private:
    void Initialize();

    void Uninitialize();

    PVOID m_hVideoDevice;

    HANDLE m_board;
    bool m_isOpen, m_isWorking;

    bool m_enableVideoRecord;
    QString m_videoName;

    inline bool isOpen() const { return m_isOpen; }
    inline bool isWorking() const { return m_isWorking; }

};

QImage toGray(QImage image);
BOOL CALLBACK beginCapture(HANDLE board);
BOOL CALLBACK process(HANDLE board, long frameIndex);
BOOL CALLBACK endCapture(HANDLE board);

#endif // IMAGEGRABBER_H

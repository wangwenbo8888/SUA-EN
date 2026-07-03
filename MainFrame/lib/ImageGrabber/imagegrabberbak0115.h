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
const int IMAGE_WIDTH = 720;
const int IMAGE_HIGHT = 576;
const int REC_X = 72;
const int REC_Y = 0;
const int REC_WIDTH = 576;
const int REC_HEIGHT = 576;

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

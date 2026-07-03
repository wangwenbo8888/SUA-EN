#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include <QThread>
#include <QImage>

#include "windows.h"
#include "Okapi32.h"
#include "imagecapture_global.h"

class IMAGECAPTURESHARED_EXPORT ImageCapture : public QThread
{
    Q_OBJECT
public:
    explicit ImageCapture(QObject *parent = 0);
    ~ImageCapture();

    void run();
//  start to capture the real-time images
    void startCapture();
//  stop capturing the images
    void stopCapture();
//  get the current image
    void updateImage(QImage image);
    void saveImage(QString fileName);
    void startSaveVideo(char* fileName);
    void stopSaveVideo();

    inline bool enableVideoRecord() const { return m_enableVideoRecord; }
    inline void setEnableVideoRecord(bool enableVideoRecord) { m_enableVideoRecord = enableVideoRecord; }

    inline bool isCapture() const { return m_isCapture; }
    inline void setCapture(bool isCapture) { m_isCapture = isCapture; }

    inline void setVideoName(char* videoName) { m_videoName = videoName; }
    inline char* getVideoName() { return m_videoName; }

signals:

    void imageUpdate(QImage image);
    void captureSucceed();
    void captureFail();

public slots:

private:
    HANDLE m_hBoard;
    bool m_enableVideoRecord;
    bool m_isCapture;
    char* m_videoName;

    bool initCapture();
};

BOOL CALLBACK beginCapture(HANDLE hBoard);
BOOL CALLBACK process(HANDLE hBoard, long frmIndex);
BOOL CALLBACK endCapture(HANDLE hBoard);

#endif // IMAGECAPTURE_H

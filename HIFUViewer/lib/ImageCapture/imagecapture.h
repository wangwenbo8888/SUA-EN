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
    void saveImage();

signals:
    void imageUpdate(QImage image);
    void captureSucceed();
    void captureFail();

public slots:

private:
    HANDLE m_hBoard;
    bool initCapture();
};

BOOL CALLBACK beginCapture(HANDLE hBoard);
BOOL CALLBACK process(HANDLE hBoard, long frmIndex);
BOOL CALLBACK endCapture(HANDLE hBoard);

#endif // IMAGECAPTURE_H

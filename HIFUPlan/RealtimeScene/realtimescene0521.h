#ifndef REALTIMESCENE_H
#define REALTIMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QStringList>

#include <QLoggingCategory>

#include "realtimescene_global.h"

#include "adjustwindow.h"
#include "adjcontrast.h"

const int ANGLE_COUNT = 181;
const float ANGLE_STEP = 1.0;
const int INVALID_ANGLE = -100;
const QString FILE_PREFIX = "Target_Image_Sequence_";
const QString FILE_SUFFIX = ".jpg";

Q_DECLARE_LOGGING_CATEGORY(REALTIMESCENE)
class REALTIMESCENESHARED_EXPORT RealtimeScene : public QGraphicsScene
{
    Q_OBJECT

public:
    RealtimeScene(QObject *parent=0);

    ~RealtimeScene();

    // Add the feature: save image into file
    inline void setFrameAcquired(bool);
    inline bool frameAcquired() const;

    void zoomOut();      // 图像放大
    void zoomIn();       // 图像缩小

    void setBeamVisible(const bool status);    // 设置显示声窗

    void showFocus();  // 显示自然焦点
    void hideFocus();  // 隐藏自然焦点

    void showRange();  // 显示辐照区域
    void hideRange();  // 隐藏辐照区域

    void resumeImage();  // 恢复原始图像大小

    void setContrast(float contrast);

    // Add the feature: save images by switching the imaging plane for the plan
    inline void setSeriesAcquired(bool);
    inline bool seriesAcquired() const;

    inline void setFrameFileName(QString);
    inline void setfoldpath(QString);
    inline QString frameFileName() const;
    inline QString foldpath() const;
    QString frameFileName(int) const;
    QString foldpath(QString) const;

    QPixmap getPixmap();
    inline void resetFrameIndex();
    // Current angle to save the image
    float angle() const;
    // Add the feature:
    float angle(int) const;

public slots:
    void refresh(QImage image);

signals:
    void seriesAcquireCompleted();

    void synPlanImage(QImage image);

    void synImage();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:

    void drawBeam();
    void drawFocus();
    void drawRange();
    void setScal(qreal scal);

    void adjustWindow(QImage& image);

    void adjContrast(QImage& image);

    QGraphicsPixmapItem* m_pixmap;
    qreal m_scal;
    bool m_frameAcquired, m_seriesAcquired;
    int m_frameIndex;

    QGraphicsPathItem *m_beam;
    QGraphicsPathItem *m_focus;   // 自然焦点
    QGraphicsPathItem *m_range;   // 辐照区域

    QPen m_beamPen;
    QPen m_focusPen;
    QPen m_rangePen;

    double m_dFocusDeep;
    QPointF m_pDelta;
    bool m_bAdjust;

    AdjustWindow* pAdjust;
    AdjContrast* m_pAdjContrast;
    bool m_bIsInitContrast;

    QString m_frameFileName;
    QString m_foldpath;
};

inline void RealtimeScene::setFrameAcquired(bool value) { m_frameAcquired = value; }
inline bool RealtimeScene::frameAcquired() const { return m_frameAcquired; }

inline void RealtimeScene::setSeriesAcquired(bool value) { m_seriesAcquired = value; }
inline bool RealtimeScene::seriesAcquired() const { return m_seriesAcquired; }

inline void RealtimeScene::resetFrameIndex() { m_frameIndex = 1; }
inline void RealtimeScene::setfoldpath(QString str) {m_foldpath = str;}
inline void RealtimeScene::setFrameFileName(QString str) { m_frameFileName = str; }
//inline void RealtimeScene::setfoldpath(QString str) {m_foldpath = str;}
inline QString RealtimeScene::frameFileName() const { return m_frameFileName; }
<<<<<<< .mine
inline QString RealtimeScene::foldpath() const {return m_foldpath;}
||||||| .r685

=======
inline QString RealtimeScene::foldpath() const {return m_foldpath;}

>>>>>>> .r694
#endif // REALTIMESCENE_H

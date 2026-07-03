#ifndef ALGOBASE
#define ALGOBASE

#include <QVector>
#include <QPair>
enum Operation{
    NoOperation=0,
    SetSeedPoint,
    SetLimitedRegion,
    Erase,
    Fill,
    ManualDraw,
};

enum FetchMethod{
    FetchThreshold,
    FetchManualDraw,
};

enum CoordinateDirection{
    ZNegative=-3,
    YNegative,
    XNegative,
    Unknown,
    XPositive,
    YPositive,
    ZPositive
};

enum PatientDirection{
    Anterior,
    Posterior,
    Left,
    Right,
    Superior,
    Inferior
};
struct DicomImageTagInfo
{
    QString sFilePath;
    int iInstanceNumber;
    double dWindowCenter;
    double dWindowWidth;
    double dImagePositionX;
    double dImagePositionY;
    double dImagePositionZ;
    double dImageOrientationRowX;
    double dImageOrientationRowY;
    double dImageOrientationRowZ;
    double dImageOrientationColX;
    double dImageOrientationColY;
    double dImageOrientationColZ;

    DicomImageTagInfo()
    {
        sFilePath=QString("");
        iInstanceNumber=-1;
        dWindowCenter=-1;
        dWindowWidth=-1;
        dImagePositionX=dImagePositionY=dImagePositionZ=0;
        dImageOrientationRowX=dImageOrientationRowY=dImageOrientationRowZ=-999.0;
        dImageOrientationColX=dImageOrientationColY=dImageOrientationColZ=-999.0;
    }
};

struct DicomSourceInfo
{
    bool isSourceRead;
    int width;
    int height;
    int depth;
    int m_iVoxelSize;
    double spacing_x;
    double spacing_y;
    double spacing_z;
    double scalar_min;
    double scalar_max;

    QString SeriesUID;
    QString PatientID;
    QString PatientName;
    QString PatientAge;
    QString PatientSex;
    QString PatientWeight;
    QString PatientBirthDate;
    QVector<DicomImageTagInfo> DicomTagInfoList;

    CoordinateDirection OrientationRow;
    CoordinateDirection OrientationCol;
    CoordinateDirection OrientationNum;

    DicomSourceInfo()
    {
    }

    void ClearParam()
    {
        isSourceRead=false;
        this->width=0;
        this->height=0;
        this->depth=0;
        this->spacing_x=0.0;
        this->spacing_y=0.0;
        this->spacing_z=0.0;
        this->scalar_min=0.0;
        this->scalar_max=0.0;
        DicomTagInfoList.clear();
    }
};

struct AlgoProcessInfo
{
    bool isCanThreshold;            //是否可以阈值分割
    bool isThreshold;               //是否阈值化过
    bool isShowThresholdMask;       //是否显示阈值化的Mask
    int threshold_min;              //当前阈值min
    int threshold_max;              //当前阈值max

    bool isShowRegionGrowDialog;    //是否显示区域增长窗体
    bool isRegionGrow;              //是否区域增长操作过
    bool isShowRegionGrowMask;      //是否显示区域增长后的Mask
    bool isSetingSeedPoint;         //是否正在设置种子点
    bool isSettingLimitedRegion1;   //是否正在设置区域点1
    bool isSettingLimitedRegion2;   //是否正在设置区域点2
    bool isErasing;                 //是否正在擦除操作
    int EraseSize;                  //擦除区域尺寸
    bool isFilling;                 //是否正在填充操作
    int FillSize;                   //填充区域大小

    bool isContourCreated;           // 是否肌瘤mask已经创建
    bool isShowContourMask;              // 是否显示肌瘤掩模

    bool isSurfaceRendering;        //是否表面重建过

    //当前种子点
    int SeedPoint_X;
    int SeedPoint_Y;
    int SeedPoint_Z;

    bool isShowLimitedRegion;       //是否显示限制区域
    bool isSettingLimitedRegion;    //是否正在标定限制区域
    int LimitPoint1_x;              //勾选区域点1
    int LimitPoint1_y;
    int LimitPoint1_z;
    int LimitPoint2_x;              //勾选区域点2
    int LimitPoint2_y;
    int LimitPoint2_z;
    int LimitRegion_z1;             //勾选区Z范围
    int LimitRegion_z2;
    double LimitRegion_center[3];
    double LimitRegion_r;


    double TumorSize[3];            //肌瘤尺寸
    double TumorVolume;             //肌瘤体积

    bool isManualDraw;              //是否人工勾选过区域
    bool isManualDrawing;           //是否正在人工勾选

    FetchMethod FecthTumorMethod;   //提取肌瘤的方式

    AlgoProcessInfo()
    {
        this->isShowThresholdMask=false;
        this->isThreshold=false;
        this->threshold_max=this->threshold_min=0;
        this->isShowRegionGrowDialog=false;
        this->isRegionGrow=false;
        this->isShowContourMask=false;
        this->isShowRegionGrowMask=false;
        this->isSettingLimitedRegion=false;
        SeedPoint_X=SeedPoint_Y=SeedPoint_Z=-1;
        isCanThreshold=false;
        isSetingSeedPoint=false;
        isErasing=false;
        isFilling=false;
        LimitPoint1_x=LimitPoint1_y=LimitPoint1_z=-1;
        LimitPoint2_x=LimitPoint2_y=LimitPoint2_z=-1;
        LimitRegion_z1=LimitRegion_z2=0;
        isSettingLimitedRegion1=isSettingLimitedRegion2=false;
        isShowLimitedRegion=false;
        TumorSize[0]=TumorSize[1]=TumorSize[2]=0;
        TumorVolume=0;
        isManualDraw=false;
        FecthTumorMethod=FetchThreshold;
        isManualDrawing=false;

        isContourCreated = false;
        isShowContourMask = false;
    }

    void ClearParam()
    {
        this->threshold_max=this->threshold_min=0;
        isCanThreshold=false;
        isThreshold=false;
        isRegionGrow=false;
        isShowContourMask=false;
        isSurfaceRendering=false;
        SeedPoint_X=SeedPoint_Y=SeedPoint_Z=-1;
        LimitPoint1_x=LimitPoint1_y=LimitPoint1_z=-1;
        LimitPoint2_x=LimitPoint2_y=LimitPoint2_z=-1;
        LimitRegion_z1=LimitRegion_z2=0;
        isErasing=false;
        isFilling=false;
        isSettingLimitedRegion1=isSettingLimitedRegion2=false;
        isShowLimitedRegion=false;
        isManualDraw=false;
        isManualDrawing=false;

        isContourCreated = false;
    }

    bool ValidThreshold()
    {
        if(SeedPoint_X==-1||SeedPoint_Y==-1||SeedPoint_Z==-1||(threshold_max==0&&threshold_min==0))
        {
            return false;
        }
        else
            return true;
    }

    bool ValidLimitRegion()
    {
        if(LimitPoint1_x==-1||LimitPoint1_y==-1||LimitPoint1_z==-1||LimitPoint2_x==-1||LimitPoint2_y==-1||LimitPoint2_z==-1||(LimitRegion_z1==0&&LimitRegion_z2==0))
            return false;
        else
            return true;
    }

    void ClearLimitRegion()
    {
        LimitPoint1_x=LimitPoint1_y=LimitPoint1_z=-1;
        LimitPoint2_x=LimitPoint2_y=LimitPoint2_z=-1;
        LimitRegion_z1=LimitRegion_z2=0;
    }
};

struct DicomPreProInfo{
    int width;
    int height;
    int depth;
    int m_iVoxelSize;
    double spacing_x;
    double spacing_y;
    double spacing_z;
    double scalar_min;
    double scalar_max;

    CoordinateDirection OrientationRow;
    CoordinateDirection OrientationCol;
    CoordinateDirection OrientationNum;

    bool bNeedRotate;
    int xyzos[3];
    bool xyzflip[3];

    int newSeed[3];

    DicomPreProInfo()
    {
    }
    void ClearParam()
    {
        this->width=0;
        this->height=0;
        this->depth=0;
        this->spacing_x=0.0;
        this->spacing_y=0.0;
        this->spacing_z=0.0;
        this->scalar_min=0.0;
        this->scalar_max=0.0;
        OrientationRow=Unknown;
        OrientationCol=Unknown;
        OrientationNum=Unknown;
        bNeedRotate=false;
    }
};

#endif // ALGOBASE


#ifndef USERMANAGER_GLOBAL
#define USERMANAGER_GLOBAL

#include <QString>

//查询数据模式（患者，医师）
enum QueryDataType
{
    TypePatient,
    TypeDoctor,
};

//患者数据行
struct PatientDataRow
{
    int id;
    QString id_str;
    QString pid;
    QString sid;
    QString name;
    QString gender;
    QString birth;
    int age;
    QString age_str;
    QString mobile;
    int depth;
    QString depth_str;
    int size_x;
    QString size_x_str;
    int size_y;
    QString size_y_str;
    int size_z;
    QString size_z_str;
    QString size_str;
    QString address;
    QString doctor;

    bool bClear;

public:
    void SetData(int ID,QString PID,QString SID,QString Name,QString Gender,QString Birth,
                   int Age,QString Mobile,int Depth,int x,int y,int z,QString Address,QString Doctor)
    {
        clear();
        this->id=ID;
        this->id_str=QString::number(this->id);
        this->pid=PID;
        this->sid=SID;
        this->name=Name;
        this->gender=Gender;
        this->birth=Birth;
        this->age=Age;
        this->age_str=QString::number(this->age);
        this->mobile=Mobile;
        this->depth=Depth;
        this->depth_str=QString::number(this->depth);
        this->size_x=x;
        this->size_x_str=QString::number(this->size_x);
        this->size_y=y;
        this->size_y_str=QString::number(this->size_y);
        this->size_z=z;
        this->size_z_str=QString::number(this->size_z);
        this->size_str=this->size_x_str+"*"+this->size_y_str+"*"+this->size_z_str+" mm3";
        this->address=Address;
        this->doctor=Doctor;
        bClear=false;
    }
    void clear()
    {
        this->id=-1;
        this->id_str="";
        this->name="";
        this->age=-1;
        this->age_str="";
        this->mobile="";
        this->depth=-1;
        this->size_x=-1;
        this->size_y=-1;
        this->size_z=-1;
        this->size_x_str=this->size_y_str=this->size_z_str=this->size_str=this->depth_str=this->address="";
        bClear=true;
    }

    bool isClear()
    {
        return bClear;
    }
};

//医师数据行
struct DoctorDataRow
{
    int id;
    QString id_str;
    QString firstName;
    QString lastName;
    QString fullName;
    QString gender;
    int age;
    QString age_str;
    QString post;
    bool isAdmin;
    QString mobile;
    QString email;

private:
    bool bClear;

public:
    void SetData(int ID,QString FName,QString LName,QString FullName,
                   QString Gender,int Age,QString Post,bool admin,QString Mobile,QString Email)
    {
        clear();
        this->id=ID;
        this->id_str=QString::number(this->id);
        this->firstName=FName;
        this->lastName=LName;
        this->fullName=FullName;
        this->gender=Gender;
        this->age=Age;
        this->age_str=QString::number(this->age);
        this->post=Post;
        this->isAdmin=admin;
        this->mobile=Mobile;
        this->email=Email;
        bClear=false;
    }
    void clear()
    {
        this->id=-1;
        this->id_str="";
        this->firstName="";
        this->lastName="";
        this->fullName="";
        this->gender="";
        this->age=-1;
        this->age_str="";
        this->post="";
        this->isAdmin=false;
        this->mobile=this->email="";
        bClear=true;
    }
    bool isClear()
    {
        return bClear;
    }
};

//医师登录信息数据行
struct DrLoginDataRow
{
    QString name;
    QString userName;
    QString pw;
    int DrID;
};

#endif // USERMANAGER_GLOBAL


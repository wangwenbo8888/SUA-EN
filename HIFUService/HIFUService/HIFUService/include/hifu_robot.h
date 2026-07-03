
// 电机控制类

#define DECODE_PROGRAMM L"MainWindow"

enum Message_Type
{
    Type_robot = 0,
    Type_image = 2
};

// enum Robot_Axis
// {
//     Axis_updown = 0,
//     Axis_leftright,
//     Axis_frontback,
//     Axis_rotate
// };

class HIFURobot
{
public:

    HIFURobot();
    ~HIFURobot();

    bool setPos(int axis, int pos);
    int getPos(int axis);

private:

};
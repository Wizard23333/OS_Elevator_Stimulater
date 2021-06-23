#ifndef ELEVATOR_UI_H
#define ELEVATOR_UI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QTime>

namespace Ui {
class elevator_UI;
}

#define ELENUM 5

enum ele_state
{
    STOPPED,
    MOVING,
    OPENING,
    CLOSING,
    BROKEN
};

const QString state_name[5]
{
    "STOPPED",
    "MOVING",
    "OPENING",
    "CLOSING",
    "BROKEN"
};

//电梯的结构体
typedef struct ele
{
    //int ele_num;//电梯号
    int current_floor;//电梯当前层数
    int direction;//电梯方向 -1 0 1 共三种状态
    bool isAlarming;//判断是否有人报警
    bool request[21];//楼层请求//外部请求也显示在上
    ele_state state;//电梯运动状态
} ele_model;


class elevator : public QWidget
{
    Q_OBJECT
public:
    explicit elevator(QWidget *parent = nullptr);
    ~elevator();

    ele_model ele_model_inside;//电梯模型
private slots:
    void on_floor_button_clicked();

    //内部run
    void run();

    //更新view
    void update_view();

    void on_alarm_btn_clicked();
private:
    void stop();
    //延时函数
    void sleep(int msec);
    //按钮
    QPushButton *btn[21];

    QPushButton *open_btn, *close_btn, *alarm_btn;
    //标签
    QLabel *lbl_direct, *lbl_floor, *lal_state;

    int make_only_one_run;

    Ui::elevator_UI *ui;
};



#endif // ELEVATOR_UI_H

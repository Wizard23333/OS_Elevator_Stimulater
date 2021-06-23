#ifndef BUILDINGOFFLOOR_H
#define BUILDINGOFFLOOR_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <queue>
#include <QTimer>

namespace Ui {
class buildingOfFloor;
}

class buildingOfFloor : public QWidget
{
    Q_OBJECT

//    friend class MainWindow;
public:
    explicit buildingOfFloor(QWidget *parent = nullptr);
    ~buildingOfFloor();

    //电梯请求信息
    bool request_up[20];
    bool request_down[21];
    std::queue<int> request_queue_up;//队列 记录按钮的请求顺序
    std::queue<int> request_queue_down;

    //外部接口 接人上下
    void get_peo_on(int floor);
    void get_peo_down(int floor);



private slots:
    void on_upButton_clicked();//当某个上行按键被按下时
    void on_downButton_clicked();
private:
    //指针数组存Button控件
    QPushButton *up_btn[20];
    QPushButton *down_btn[21];
    Ui::buildingOfFloor *ui;
};

#endif // BUILDINGOFFLOOR_H

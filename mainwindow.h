#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
//#include "windows.h"
#include <QScreen>
#include <elevator.h>
#include <buildingoffloor.h>
#include <dispatch.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<elevator*> elevators;//电梯的collection
    buildingOfFloor *building;//楼层模型
private slots:
    void update_view();
    //同步ele和building的model
    void synchronize_ele_building();
private:
    unsigned int request_tol;//楼层的电梯请求总数
    QSlider *ele_slider[5];//滑块电梯collocation
    QLabel *num_floor[5];//电梯的层数显示
    int make_only_one_synchronize;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

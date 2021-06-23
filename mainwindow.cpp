#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    make_only_one_synchronize = 0;

    request_tol = 0;

    //绑定电梯
    ele_slider[0] = ui->Elevator1;
    ele_slider[1] = ui->Elevator2;
    ele_slider[2] = ui->Elevator3;
    ele_slider[3] = ui->Elevator4;
    ele_slider[4] = ui->Elevator5;


    //电梯的楼层显示
    num_floor[0] = ui->floorOfEle_1;
    num_floor[1] = ui->floorOfEle_2;
    num_floor[2] = ui->floorOfEle_3;
    num_floor[3] = ui->floorOfEle_4;
    num_floor[4] = ui->floorOfEle_5;


    //屏幕宽度获取
    QList<QScreen *> list_screen =  QGuiApplication::screens();  //多显示器
    QRect rect = list_screen.at(0)->geometry();
    auto desktop_width = rect.width();


    //楼层UI创建
    building = new buildingOfFloor;
    building->show();
    building->move(desktop_width / 5 * 3, desktop_width / 6);

    //电梯窗口创建
    for(int i = 0; i < 5; i++)
    {
        class elevator *tempEle = new class elevator;
        //设置电梯窗口位置
        tempEle->move( 10 + i % (desktop_width / tempEle->width()) * (tempEle->width() + 5),
                       10 + ((i + 1) * tempEle->width() / desktop_width) * (tempEle->height() + 35)
                     );
        //设置电梯窗口名称：电梯1 电梯2 ...
        QString str = QString("电梯") + QString::number(i + 1);
        tempEle->setWindowTitle(str);

        tempEle->show();
        tempEle->setFixedSize(235, 278);
        elevators.push_back(tempEle);
    }

    //view更新
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update_view()));
    connect(timer, SIGNAL(timeout()), SLOT(synchronize_ele_building()));
    timer->start(10);

}
void MainWindow::update_view()
{
    for(unsigned int i = 0; i < elevators.size(); i++)
    {
        ele_slider[i]->setValue(elevators[i]->ele_model_inside.current_floor - 1);
        num_floor[i]->setText(QString::number(elevators[i]->ele_model_inside.current_floor));
        if(elevators[i]->ele_model_inside.state == BROKEN)
        {
            ele_slider[i]->setStyleSheet("background-color:red;");
        }
    }
}
void MainWindow::synchronize_ele_building()
{
    //信号量机制
    if(make_only_one_synchronize != 0)
        return;
    make_only_one_synchronize = 1;


    for(unsigned int i = 0; i < 5; i++)
    {
        //电梯CLOSING的时候清除building的乘坐电梯请求
        if(elevators[i]->ele_model_inside.state == CLOSING)
        {
            //上
            if(elevators[i]->ele_model_inside.direction == 1)
            {
                building->request_up[elevators[i]->ele_model_inside.current_floor] = false;
            }
            else if(elevators[i]->ele_model_inside.direction == -1)
            {
                building->request_down[elevators[i]->ele_model_inside.current_floor] = false;
            }
            else {

            }
        }
    }

    //实现building到elevators的请求分配
    //dispatch(elevators, building)
    //通过dispatch函数来将任务静态分派给电梯的request数组，电梯里的按钮也显示被按下，不区分电梯内和电梯外
    //其实实际上也更加人性化（实际电梯都没有这个功能）

   //调用分派函数
    //dispatch_to_elevator(elevators, building);

    if(request_tol < (building->request_queue_up.size() + building->request_queue_down.size()))
    {
        dispatch_to_elevator(elevators, building);
        request_tol = building->request_queue_up.size() + building->request_queue_down.size();
    }


    make_only_one_synchronize = 0;
    return;
}

MainWindow::~MainWindow()
{
    //删除building 和 elevators
    delete building;
    for(; elevators.empty() == false; )
    {
        class elevator *delTemp = elevators.back();
        elevators.pop_back();
        delete delTemp;
    }
    delete ui;
}


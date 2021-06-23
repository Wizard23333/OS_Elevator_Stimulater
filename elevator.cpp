#include "elevator.h"
#include "ui_elevator.h"

elevator::elevator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::elevator_UI)
{
    ui->setupUi(this);

    //电梯model初始化
    ele_model_inside = {1, 0, false, {true}, STOPPED};

    btn[1] = ui->f1;
    btn[2] = ui->f2;
    btn[3] = ui->f3;
    btn[4] = ui->f4;
    btn[5] = ui->f5;
    btn[6] = ui->f6;
    btn[7] = ui->f7;
    btn[8] = ui->f8;
    btn[9] = ui->f9;
    btn[10] = ui->f10;
    btn[11] = ui->f11;
    btn[12] = ui->f12;
    btn[13] = ui->f13;
    btn[14] = ui->f14;
    btn[15] = ui->f15;
    btn[16] = ui->f16;
    btn[17] = ui->f17;
    btn[18] = ui->f18;
    btn[19] = ui->f19;
    btn[20] = ui->f20;

    open_btn = ui->openButton;
    close_btn = ui->closeButton;
    alarm_btn = ui->alarmButton;

    lbl_floor = ui->label_floor;
    lbl_direct = ui->label_direction;

    make_only_one_run = 0;

    for(int i = 1; i < 21; i++)
    {
        connect(btn[i], SIGNAL(clicked()), SLOT(on_floor_button_clicked()));
    }
    connect(alarm_btn, SIGNAL(clicked()), SLOT(on_alarm_btn_clicked()));

    //每隔一段时间就run一次
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update_view()));
    timer->start(10);

    QTimer *timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), SLOT(run()));
    timer1->start(10);


}

void elevator::on_floor_button_clicked()
{
    auto btn_sender = qobject_cast<QPushButton*>(QObject::sender());
    QString btn_name = btn_sender->objectName();
    int btn_num = btn_name.mid(1).toInt();

    //记录被按下的按键的位置
    ele_model_inside.request[btn_num] = true;
    //qDebug() << btn_num;
    btn_sender->setDisabled(true);
    btn_sender->setStyleSheet("color: blue");
}

void elevator::on_alarm_btn_clicked()
{
    ele_model_inside.state = BROKEN;
}
void elevator::update_view()
{
    //更新状态
    ui->label_state->setText(state_name[ele_model_inside.state]);
    //当前楼层
    ui->label_floor->setText(QString::number(ele_model_inside.current_floor));
    //当前方向
    QString temp_direc;
    if(ele_model_inside.direction == -1)
        temp_direc = "下";
    else if(ele_model_inside.direction == 1)
        temp_direc = "上";
    else
        temp_direc = "-";
    ui->label_direction->setText(temp_direc);
    //开门关门
    if(ele_model_inside.state == OPENING)
    {
        open_btn->setDisabled(true);
        open_btn->setStyleSheet("color: blue");
    }
    else
    {
        open_btn->setDisabled(false);
        open_btn->setStyleSheet("color: black");
    }
    if(ele_model_inside.state == CLOSING)
    {
        close_btn->setDisabled(true);
        close_btn->setStyleSheet("color: blue");
    }
    else
    {
        close_btn->setDisabled(false);
        close_btn->setStyleSheet("color: black");
    }
    if(ele_model_inside.state == BROKEN)
    {
        this->setStyleSheet("background-color:red;");
    }

    //电梯楼层按钮
    for(int i = 1; i < 21; i++)
    {
        if(ele_model_inside.request[i] == true)
        {
            btn[i]->setDisabled(true);
            btn[i]->setStyleSheet("color: blue");
        }
        else
        {
            btn[i]->setDisabled(false);
            btn[i]->setStyleSheet("color: black");
        }
    }



}


void elevator::run()
{
    //控制只能有一个run函数在运行 类似于PV操作
    if(make_only_one_run != 0)
        return;
    make_only_one_run = 1;

    int sum_request = 0;//计算请求总量
    for(int i = 1; i <= 20; i++)
    {
        sum_request += ele_model_inside.request[i];
    }
    //请求总量为零 停止并退出运行
    if(sum_request == 0)
    {
        stop();
        make_only_one_run = 0;
        return;
    }
    //根据电梯的状态运行
    //TODO:
    switch (ele_model_inside.state) {
    case STOPPED:
    {
        //切换成运动态
        ele_model_inside.state = MOVING;
        int min_cost = 100;
        //寻找最近的一个请求楼层
        for(int i = 1; i <= 20; i++)
        {
            if(ele_model_inside.request[i] == true && abs(ele_model_inside.current_floor - i) < min_cost)
            {
                min_cost = abs(ele_model_inside.current_floor - i);
                if(i - ele_model_inside.current_floor < 0)
                    ele_model_inside.direction = -1;
                else if(i - ele_model_inside.current_floor > 0)
                    ele_model_inside.direction = 1;
                else
                    ele_model_inside.direction = 0;
            }
        }
    }
        break;
    case MOVING:
    {
        //如果当前楼层有请求
        if(ele_model_inside.request[ele_model_inside.current_floor] == true)
        {
            //转换到开门状态
            sleep(2000);
            ele_model_inside.state = OPENING;
        }
        else
        {
            //方向反转
            //计算上下请求的数目
            int flag = 0;
            if(ele_model_inside.direction < 0)
            {
                for(int i = 1; i < ele_model_inside.current_floor; i++)
                {
                    flag += ele_model_inside.request[i];
                }
            }
            else if(ele_model_inside.direction > 0)
            {
                for(int i = ele_model_inside.current_floor + 1; i <= 20; i++)
                {
                    flag += ele_model_inside.request[i];
                }
            }
            else
            {
                //定向到第一种情况中解决电梯卡住问题问题
                ele_model_inside.state = STOPPED;
            }
            if(ele_model_inside.current_floor + ele_model_inside.direction == 21 || ele_model_inside.current_floor + ele_model_inside.direction == 0 || flag == 0)
            {
                ele_model_inside.direction = 0 - ele_model_inside.direction;
            }
            sleep(1000);
            ele_model_inside.current_floor += ele_model_inside.direction;
        }
    }
        break;
    case OPENING:
    {

        sleep(3000);
        ele_model_inside.state = CLOSING;
    }
        break;
    case CLOSING:
    {
        sleep(3000);
        ele_model_inside.request[ele_model_inside.current_floor] = false;
        ele_model_inside.state = MOVING;
    }
        break;
    case BROKEN:
    {
    }
        break;
    }

    make_only_one_run = 0;
    return;
}

void elevator::stop()
{
    if(ele_model_inside.state != BROKEN)
        ele_model_inside.state = STOPPED;

}

void elevator::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < dieTime )
            {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            //这条语句能够使程序在while等待期间，去处理一下本线程的事件循环，处理事件循环最多100ms必须返回本语句，
            //如果提前处理完毕，则立即返回这条语句。这也就导致了该Delay_MSec函数的定时误差可能高达100ms。
        }
}


elevator::~elevator()
{
    delete ui;
}





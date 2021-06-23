#include "buildingoffloor.h"
#include "ui_buildingoffloor.h"

buildingOfFloor::buildingOfFloor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::buildingOfFloor)
{
    ui->setupUi(this);

    //上行按钮的槽函数连接
    up_btn[1] = ui->up1;
    up_btn[2] = ui->up2;
    up_btn[3] = ui->up3;
    up_btn[4] = ui->up4;
    up_btn[5] = ui->up5;
    up_btn[6] = ui->up6;
    up_btn[7] = ui->up7;
    up_btn[8] = ui->up8;
    up_btn[9] = ui->up9;
    up_btn[10] = ui->up10;
    up_btn[11] = ui->up11;
    up_btn[12] = ui->up12;
    up_btn[13] = ui->up13;
    up_btn[14] = ui->up14;
    up_btn[15] = ui->up15;
    up_btn[16] = ui->up16;
    up_btn[17] = ui->up17;
    up_btn[18] = ui->up18;
    up_btn[19] = ui->up19;

    down_btn[2] = ui->down2;
    down_btn[3] = ui->down3;
    down_btn[4] = ui->down4;
    down_btn[5] = ui->down5;
    down_btn[6] = ui->down6;
    down_btn[7] = ui->down7;
    down_btn[8] = ui->down8;
    down_btn[9] = ui->down9;
    down_btn[10] = ui->down10;
    down_btn[11] = ui->down11;
    down_btn[12] = ui->down12;
    down_btn[13] = ui->down13;
    down_btn[14] = ui->down14;
    down_btn[15] = ui->down15;
    down_btn[16] = ui->down16;
    down_btn[17] = ui->down17;
    down_btn[18] = ui->down18;
    down_btn[19] = ui->down19;
    down_btn[20] = ui->down20;

    for(int i = 1; i < 20; i++)
    {
        request_up[i] = false;
        connect(up_btn[i], SIGNAL(clicked()), SLOT(on_upButton_clicked()));
    }

    for(int i = 2; i < 21; i++)
    {
        request_down[i] = false;
        connect(down_btn[i], SIGNAL(clicked()), SLOT(on_downButton_clicked()));
    }

    //初始化request数组
    for(int i = 1; i < 20; i++)
    {
        request_up[i] = false;
    }
    for(int i = 2; i < 21; i++)
    {
        request_down[i] = false;
    }
}

void buildingOfFloor::on_upButton_clicked()
{
    //将sender传回的QObject转换成QPushButton类型
    auto btn_sender = qobject_cast<QPushButton*>(QObject::sender());
    //获取按钮名称
    QString btn_name = btn_sender->objectName();

    //    qDebug() << btn_name;//一定是qDebug 不是QDebug
    //获取按钮编号
    //int btn_num = QString(btn_name.at(btn_name.size() - 1)).toInt();//错误方法
    int btn_num = btn_name.mid(2).toInt();

    request_up[btn_num] = true;
    request_queue_up.push(btn_num);
}

void buildingOfFloor::on_downButton_clicked()
{
    auto btn_sender = qobject_cast<QPushButton*>(QObject::sender());
    QString btn_name = btn_sender->objectName();
    int btn_num = btn_name.mid(4).toInt();
    request_down[btn_num] = true;
    request_queue_down.push(btn_num);
}

void buildingOfFloor::get_peo_on(int floor)
{
    if(floor >= 1 && floor <= 19)
    {
        up_btn[floor]->setDisabled(false);
        up_btn[floor]->setStyleSheet("color: black");
        request_up[floor] = false;//消除请求状态
    }
    else
    {
        qDebug("error1");
    }
}

void buildingOfFloor::get_peo_down(int floor)
{
    if(floor >= 2 && floor <= 20)
    {
        down_btn[floor]->setDisabled(false);
        down_btn[floor]->setStyleSheet("color: black");
        request_down[floor] = false;
    }
    else
    {
        qDebug("error1");
    }
}

buildingOfFloor::~buildingOfFloor()
{
    delete ui;
}

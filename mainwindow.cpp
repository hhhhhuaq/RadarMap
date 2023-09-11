#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include <iostream>
#include <sstream>
#include <fstream>

//生成数据
void genrateData(QVector<double> &data)
{
    fstream f("/home/toor/Desktop/data.csv",ios::in);
    if(f.is_open() == false){std::cout<<"open failed!\n";}
    string line;
    string buf;
    int count = 0;
    while(std::getline(f,line))
    {
        stringstream ss(line);
        while(ss)
        {
            ss>>data[count++];
        }
    }
}

static const int RealDistance{10000};

static const int ItemSize{8};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //极坐标系
    _pcanvas = this->ui->MainGraphicsView->canvas();
    this->ui->MainGraphicsView->setBoundingRectVisible(true);
    _pAxisAngular = new TPolarAxisAngular(_pcanvas);
//    _pAxisAngular->setRadarRange(RadarRatiorange);
//    _pAxisAngular->setSpectrumRange(SpectrumRatiorange);
//    _pAxisAngular->setEmeRange(EmeRatiorange);
//    _pAxisAngular->setLogicRadius(RealDistance);

    _pDroneTableWidget = this->ui->tableWidgetDrone;



//    connect(this->ui->checkBox_Drone,&QCheckBox::clicked,this,[&](bool checked){
//        for(int i = 0 ; i < _NomalDrones.size() ; i++)
//        {
//            _NomalDrones[i]->setVisible(checked);
//        }
//        _pcanvas->update();
//    });
    connect(this->ui->checkBox_Radar,&QCheckBox::clicked,this,[&](bool checked){
        this->ui->MainGraphicsView->setOpacity(checked*100);
    });

//    for(int i = 0 ; i < 4 ; i++)
//    {
//        //无人机
//        TPolarGraphDroneItem *drone = new TPolarGraphDroneItem(_pAxisAngular);
//        int x = qrand()%RealDistance;
//        drone->addData(x,qrand()%360);
//        _NomalDrones.append(drone);
//        _pDroneTableWidget->addData(i,1,QString("无人机 %1").arg(i));
//        connect(this->ui->MainGraphicsView,&TPolarPlot::KeyPress,[&,drone](QKeyEvent * e){
//            TPolarPoint p = drone->LogicPoint();
//            if(e->key() == Qt::Key_Up)
//            {
//                drone->addData(p.Radius(),p.Angle()+5);
//            }
//            else if(e->key() == Qt::Key_Down)
//            {
//                drone->addData(p.Radius(),p.Angle()-5);
//            }
//            else if(e->key() == Qt::Key_Left)
//            {
//                drone->addData(p.Radius() - 20,p.Angle());
//            }
//            else if(e->key() == Qt::Key_Right)
//            {
//                drone->addData(p.Radius() + 20,p.Angle());
//            }
//            this->ui->MainGraphicsView->update();
//        });

//        _UnknowDrones.append(new TPolarGraphDroneItem(_pAxisAngular));
//        _UnknowDrones[i]->setImage(ImageUnkonwDroneFileName);
//        _UnknowDrones[i]->addData(qrand()%RealDistance,qrand()%360);
//        _pDroneTableWidget->addData(i,1,QString("未知无人机 %1").arg(i));

//        //遥控器
//        TPolarGraphRemoteControlItem *pRC = new TPolarGraphRemoteControlItem(_pAxisAngular);
//        pRC->setPos(qrand()%RealDistance,qrand()%360);
//        _RemoteControl.append(pRC);

//        //辐射源
//        TPolarGraphRadioSourceItem *pRS = new TPolarGraphRadioSourceItem(_pAxisAngular);
//        pRS->setPos(qrand()%RealDistance,qrand()%360);
//        _RadioSources.append(pRS);
//    }
    //脉冲


    connect(&_timer,&QTimer::timeout,this,[&](){
        TPolarGraphPulseItem *pPulse = new TPolarGraphPulseItem(_pAxisAngular);
        pPulse->setLogicPos(qrand()%RealDistance,qrand()%360);
        auto con = connect(pPulse,&TPolarGraphPulseItem::disapear,this,[pPulse](){
            if(pPulse != nullptr)delete pPulse;
        });
    });
    _timer.start(400);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

}

QSize MainWindow::sizeHint() const
{
    return QSize(800,600);
}











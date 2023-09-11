#include "dronetablewidget.h"
#include <QtDebug>
DroneTableWidget::DroneTableWidget(QWidget *parent)
    :QTableWidget(parent)
{
    _TableHeader<<"ID"<<"类型"<<"名称"<<"是否显示";
    this->setColumnCount(_TableHeader.count());
    this->setHorizontalHeaderLabels(_TableHeader);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->resize(200,this->height());
}


DroneTableWidget::~DroneTableWidget()
{

}

void DroneTableWidget::addData(const qint32 &ID, const qint8 &type, const QString &name)
{
    int count = this->rowCount();
    this->insertRow(count);
    this->setItem(count,0,new  QTableWidgetItem(QString::number(ID)));
    this->setItem(count,1,new  QTableWidgetItem(QString::number(type)));
    this->setItem(count,2,new  QTableWidgetItem(name));
}

#ifndef DRONETABLEWIDGET_H
#define DRONETABLEWIDGET_H

#include <QObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QPalette>

class DroneTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit DroneTableWidget(QWidget *parent = Q_NULLPTR);
    virtual ~DroneTableWidget();

    void addData(const qint32 &ID,const qint8 &type,const QString &name);
private:
    QStringList _TableHeader;
};

#endif // DRONETABLEWIDGET_H

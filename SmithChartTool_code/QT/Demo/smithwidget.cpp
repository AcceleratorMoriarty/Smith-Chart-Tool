/*************************************************************************
	* @File Name: smithwidget.c
	* @Author: Accelerator(Xu HuiYao)
   	* @Version: 1.0
	* @Mail: 2364412203@qq.com 
 ************************************************************************/

#include "smithwidget.h"
#include <QDebug>
#include <QPainter>
#include "ui_smithwidget.h"
#include <cmath>

SmithWidget::SmithWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SmithWidget)
{
    ui->setupUi(this);

    //安装事件滤波器

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SmithWidget::onTimerTimeout);
    timer->start(500); // 0.5秒触发一次定时器

    ui->widget_paint->installEventFilter(this);
}

SmithWidget::~SmithWidget()
{
    delete ui;
}

bool SmithWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->widget_paint && event->type() == QEvent::Paint) {
        SmithPaint(); //响应函数
    }
    return QWidget::eventFilter(watched, event);
}

//实现响应函数
void SmithWidget::SmithPaint()
{
    QPainter painter(ui->widget_paint);
    painter.setPen(Qt::gray);
    //绘制单位圆
    painter.drawEllipse(QPoint(ui->widget_paint->width() / 2, ui->widget_paint->height() / 2),
                        ui->widget_paint->width() / 2 - PAINT_BIAS,
                        ui->widget_paint->height() / 2 - PAINT_BIAS);
    //比例系数
    double k = (ui->widget_paint->width() / 2 - PAINT_BIAS);

    //绘制横轴
    painter.drawLine(QPoint(ui->widget_paint->width(), ui->widget_paint->height() / 2),
                     QPoint(0, ui->widget_paint->height() / 2));
    //绘制纵轴
    painter.drawLine(QPoint(ui->widget_paint->width() / 2, 0),
                     QPoint(ui->widget_paint->width() / 2, ui->widget_paint->height()));

    //归一化阻抗
    z = std::complex<double>(ui->lineEdit_z_r->text().toDouble(),
                             ui->lineEdit_z_i->text().toDouble());

    //归一化导纳
    y = 1.0 / z;
    if (y.imag() >= 0)
        ui->lineEdit_y->setText(QString::number(y.real()) + "+" + QString::number(y.imag()) + "j");
    else if (y.imag() < 0)
        ui->lineEdit_y->setText(QString::number(y.real()) + QString::number(y.imag()) + "j");

    zl = std::complex<double>(ui->lineEdit_zl_r->text().toDouble(),
                              ui->lineEdit_zl_i->text().toDouble());
    z0 = ui->lineEdit_z0->text().toDouble();

    //反射系数
    gamma = (zl - z0) / (zl + z0);

    if (gamma.imag() >= 0)
        ui->lineEdit_gamma->setText(QString::number(gamma.real()) + "+"
                                    + QString::number(gamma.imag()) + "j");
    else if (gamma.imag() < 0)
        ui->lineEdit_gamma->setText(QString::number(gamma.real()) + QString::number(gamma.imag())
                                    + "j");

    //模长
    ui->lineEdit_abs->setText(QString::number(abs(gamma)));
    //相位
    ui->lineEdit_angle->setText(QString::number(arg(gamma) * 180 / 3.1415926));

    //驻波系数
    vswr = (1 + std::abs(gamma)) / (1 - std::abs(gamma));
    ui->lineEdit_vswr->setText(QString::number(vswr));

    //电长度
    ui->lineEdit_l->setText(QString::number(0.25 - (arg(gamma) / 4 / 3.1415926)));

    //输入阻抗
    l0 = ui->lineEdit_l0->text().toDouble();
    zi = z0 * (zl + std::complex<double>(0, 1) * z0 * tan(l0 * 2 * 3.1415926))
         / (z0 + std::complex<double>(0, 1) * zl * tan(l0 * 2 * 3.1415926));

    if (zi.imag() >= 0)
        ui->lineEdit_zi->setText(QString::number(zi.real()) + "+" + QString::number(zi.imag())
                                 + "j");
    else if (zi.imag() < 0)
        ui->lineEdit_zi->setText(QString::number(zi.real()) + QString::number(zi.imag()) + "j");

    gamma_i = (zi - z0) / (zi + z0);

    //反射系数圆
    painter.setPen(QPen(Qt::red, 2));
    painter.translate(ui->widget_paint->width() / 2,
                      ui->widget_paint->height() / 2); // 将坐标原点移到窗口中心
    painter.drawEllipse(QPointF(0, 0), abs(gamma) * k, abs(gamma) * k);

    painter.setPen(QPen(Qt::darkGreen, 2));
    painter.drawLine(QPointF(0, 0), QPointF(gamma.real() * k * 500, -gamma.imag() * k * 500));

    painter.setPen(QPen(Qt::cyan, 2));
    painter.drawLine(QPointF(0, 0), QPointF(gamma_i.real() * k * 500, -gamma_i.imag() * k * 500));

    //归一化电阻圆
    painter.setPen(QPen(Qt::darkMagenta, 2));
    painter.drawEllipse(QPointF(z.real() / (1 + z.real()) * k, 0),
                        1 / (1 + z.real()) * k,
                        1 / (1 + z.real()) * k);

    //归一化电抗圆
    painter.setPen(QPen(Qt::blue, 2));
    painter.drawEllipse(QPointF(1 * k, -1 / z.imag() * k), 1 / z.imag() * k, 1 / z.imag() * k);
}

//按键计算归一化阻抗
void SmithWidget::on_pushButton_clicked()
{
    z_r = ui->lineEdit_zl_r->text().toDouble() / ui->lineEdit_z0->text().toDouble();
    ui->lineEdit_z_r->setText(QString::number(z_r));
    z_i = ui->lineEdit_zl_i->text().toDouble() / ui->lineEdit_z0->text().toDouble();
    ui->lineEdit_z_i->setText(QString::number(z_i));
}

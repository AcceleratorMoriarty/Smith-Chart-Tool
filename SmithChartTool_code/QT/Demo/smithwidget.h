/*************************************************************************
	* @File Name: smithwidget.h
	* @Author: Accelerator(Xu HuiYao)
   	* @Version: 1.0
	* @Mail: 2364412203@qq.com 
 ************************************************************************/
#ifndef SMITHWIDGET_H
#define SMITHWIDGET_H

#include <QTimer>
#include <QWidget>
#include <complex>

#define PAINT_BIAS 5 //绘图边沿距离

QT_BEGIN_NAMESPACE
namespace Ui { class SmithWidget; }
QT_END_NAMESPACE

class SmithWidget : public QWidget
{
    Q_OBJECT

public:
    SmithWidget(QWidget *parent = nullptr);
    ~SmithWidget();

private slots:
    void onTimerTimeout()
    {
        // 定时器超时时，调用update函数触发重新绘制
        update();
    }

    void on_pushButton_clicked();

private:
    Ui::SmithWidget *ui;

    QTimer *timer;

    double z_r; //归一化阻抗实部
    double z_i; //归一化阻抗虚部

    double z0;
    double vswr;
    double l0;
    std::complex<double> zl;
    std::complex<double> zi;
    std::complex<double> z;
    std::complex<double> y;
    std::complex<double> gamma;
    std::complex<double> gamma_i;

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void SmithPaint();
};
#endif // SMITHWIDGET_H

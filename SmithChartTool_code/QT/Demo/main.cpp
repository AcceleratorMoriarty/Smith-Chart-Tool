/*************************************************************************
	* @File Name: main.c
	* @Author: Accelerator(Xu HuiYao)
   	* @Version: 1.0
	* @Mail: 2364412203@qq.com 
 ************************************************************************/
#include <QApplication>
#include "smithwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmithWidget w;
    w.show();
    return a.exec();
}

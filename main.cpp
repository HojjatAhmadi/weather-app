#include "weather.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Weather w;
    w.setMaximumSize(800,600);
    w.setMinimumSize(800,600);
    w.show();
    return a.exec();
}

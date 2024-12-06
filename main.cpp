#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QThread>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建启动画面
    QPixmap pixmap(":/images/images/njzy.jpg");  // 从资源文件加载图片
    pixmap = pixmap.scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // QSplashScreen splash(pixmap);

    //进度条
    MySplash splash(pixmap);

    // 显示启动画面
    splash.show();


    MainWindow w;

    // 延迟显示主窗口（这里设置3秒）
    // QTimer::singleShot(3000, &splash, [&w, &splash](){
    //     w.show();        // 显示主窗口
    //     splash.finish(&w); // 关闭启动画面
    // });
    // return a.exec();

    //进度条模拟加载过程
    for(int i = 0; i <= 100; i++) {
        splash.setProgress(i);
        QThread::msleep(10);
        a.processEvents();
    }
    w.show();
    splash.finish(&w);

    return a.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QToolBar>

#include <QApplication>
#include <QSplashScreen>
#include <QProgressBar>
#include <QMdiSubWindow>
#include <QSpinBox>
#include <QScrollBar>
#include <QTranslator>

#include <QPointer>
#include "fire.h"
#include "ghttimer.h"
#include "ght.h"
#include "photoShop.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 添加自定义启动画面类
class MySplash : public QSplashScreen
{
    Q_OBJECT
public:
    MySplash(const QPixmap &pixmap);
    void setProgress(int value);

private:
    QProgressBar *progressBar;
};


//原有的
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_N_triggered();

    void on_action_S_triggered();

    void on_action_Dock_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    // void on_processButton_clicked();

    // void on_processButton_clicked(const QImage& inputImage = QImage());
    void on_processButton_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    void setText(const char *);
    QMdiSubWindow *currentText;  // 保存当前文本窗口的指针
    Ght *ght;

    //文字对齐
    void setTextLine(Qt::Alignment alignment);

    QColor currentTextColor;  // 添加成员变量保存当前文字颜色
    void changeTextColor(const QColor &color);  // 添加改变颜色的函数

    QFont currentFont;  // 添加成员变量保存当前字体
    void changeTextFont(const QFont &font);  // 添加改变字体的函数

    QSpinBox *spinBox;  // 改用你定义的变量名
    QSpinBox *fontSizeSpinBox;  // 添加成员变量
    void changeFontSize(int size);  // 添加改变字体大小的函数

    //声明timer界面 QPointer<newwindow1> new_win1;
    QPointer<ghtTimer> ght_timer;

    //声明fire界面
    QPointer<fire> fire;

    //声明photoshop界面
    QPointer<photoShop> photoShop;

    //图像处理
    // photoShop *photoProcessor;

    //测试opencv
    void testOpenCV();

    //中英文
    QString strFile;
    QTranslator ch;
    QTranslator en;

};
#endif // MAINWINDOW_H

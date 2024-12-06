#ifndef GHTTIMER_H
#define GHTTIMER_H

#include <QDialog>
#include <QTimer>
#include <QVector>
#include <QString>

namespace Ui {
class ghtTimer;
}

class ghtTimer : public QDialog
{
    Q_OBJECT

public:
    explicit ghtTimer(QWidget *parent = nullptr);
    ~ghtTimer();

private slots:
    void on_btnStart_clicked();

    void on_btnPause_clicked();

    void on_btn_AddImages_clicked();

    void showNextImage();

    void updateTimerInterval(double value);  //监听spinbox

    void daytimerUpdate();

private:
    Ui::ghtTimer *ui;
    QTimer *dayTimer;             //系统时间
    QTimer *timer;                //定时器
    QVector<QString> imageList;  //图片储存
    bool isPlaying;               // 播放状态
};

#endif // GHTTIMER_H

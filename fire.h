#ifndef FIRE_H
#define FIRE_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QImage>

namespace Ui {
class fire;
}

class fire : public QDialog
{
    Q_OBJECT

public:
    explicit fire(QWidget *parent = nullptr);
    ~fire();

private:
    Ui::fire *ui;
    QLabel *imageLabel;        // 显示区域
    QPushButton *openImageBtn; // 打开图片按钮
    QPushButton *openVideoBtn; // 打开视频按钮
    QPushButton *openCameraBtn;// 打开摄像头按钮
    QPushButton *startBtn;     // 开始检测按钮
    QPushButton *stopBtn;      // 停止检测按钮
    QTimer *timer;             // 用于视频播放
    bool isDetecting;          // 检测状态

private slots:
    void onOpenImage();
    void onOpenVideo();
    void onOpenCamera();
    void onStartDetection();
    void onStopDetection();
};

#endif // FIRE_H

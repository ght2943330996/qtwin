#include "fire.h"
#include "ui_fire.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

fire::fire(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::fire)
    , isDetecting(false)
{
    ui->setupUi(this);
    //关闭窗口时，自动析构释放内存
    setAttribute(Qt::WA_DeleteOnClose);

    // 设置窗口标志，添加最大化最小化按钮
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

    // 创建定时器
    timer = new QTimer(this);

    // 连接信号和槽
    connect(ui->openImageBtn, &QPushButton::clicked, this, &fire::onOpenImage);
    connect(ui->openVideoBtn, &QPushButton::clicked, this, &fire::onOpenVideo);
    connect(ui->openCameraBtn, &QPushButton::clicked, this, &fire::onOpenCamera);
    connect(ui->startBtn, &QPushButton::clicked, this, &fire::onStartDetection);
    connect(ui->stopBtn, &QPushButton::clicked, this, &fire::onStopDetection);
}

// 打开图片
void fire::onOpenImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开图片"), "",tr("图片文件 (*.png *.jpg *.jpeg *.bmp)"));

    if (!fileName.isEmpty()) {
        // TODO: 处理图片
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::warning(this, tr("错误"),tr("无法加载图片 %1").arg(fileName));
            return;
        }
        imageLabel->setPixmap(QPixmap::fromImage(
            image.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }
}

// 打开视频
void fire::onOpenVideo()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开视频"), "", tr("视频文件 (*.mp4 *.avi *.mov)"));

    if (!fileName.isEmpty()) {
        // TODO: 处理视频
        QMessageBox::information(this, tr("提示"),tr("已选择视频文件: %1").arg(fileName));
    }
}

// 打开摄像头
void fire::onOpenCamera()
{
    // TODO: 打开摄像头
    QMessageBox::information(this, tr("提示"), tr("正在打开摄像头..."));
}

// 开始检测
void fire::onStartDetection()
{
    isDetecting = true;
    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    // TODO: 开始检测逻辑
}

// 停止检测
void fire::onStopDetection()
{
    isDetecting = false;
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    // TODO: 停止检测逻辑
}

fire::~fire()
{
    delete ui;
}

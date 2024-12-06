#include "ghttimer.h"
#include "ui_ghttimer.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>

ghtTimer::ghtTimer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ghtTimer)
{
    ui->setupUi(this);
    //关闭窗口时，自动析构释放内存
    setAttribute(Qt::WA_DeleteOnClose);

    //初始化定时器
    dayTimer = new QTimer(this);
    timer = new QTimer(this);

    //系统时间
    connect(dayTimer, &QTimer::timeout, this, &ghtTimer::daytimerUpdate);
    dayTimer->start(1000); // 每秒更新一次时间
    // 初始显示一次时间，避免等待第一次定时器触发
    daytimerUpdate();

    //定时器关联显示
    connect(timer, &QTimer::timeout, this, &ghtTimer::showNextImage);

    // 初始化界面状态
    ui->spinBox->setValue(0.5);       // 默认值1秒
    ui->spinBox->setSuffix(" 秒");   // 添加后缀"秒"
    ui->spinBox->setPrefix("间隔: "); // 添加前缀"间隔: "
    ui->btnPause->setEnabled(false);

    //用于实时更新定时器间隔
    connect(ui->spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),this, &ghtTimer::updateTimerInterval);
}

ghtTimer::~ghtTimer()
{
    delete ui;
}

void ghtTimer::daytimerUpdate()
{
    //获取系统现在的时间
    QDateTime daytimer = QDateTime::currentDateTime();
    //设置系统时间显示格式
    QString str = daytimer.toString("yyyy-MM-dd hh:mm:ss dddd");
    //在标签上显示时间
    ui->lineEdit->setText(str);

}

void ghtTimer::on_btnStart_clicked()
{
    if (imageList.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先添加图片！");
        return;
    }

    int interval = ui->spinBox->value() * 1000; // 将秒转换为毫秒
    timer->start(interval);
    isPlaying = true;

    ui->btnStart->setEnabled(false);
    ui->btnPause->setEnabled(true);
}


void ghtTimer::on_btnPause_clicked()
{
    timer->stop();
    isPlaying = false;

    ui->btnStart->setEnabled(true);
    ui->btnPause->setEnabled(false);
}


void ghtTimer::on_btn_AddImages_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择图片",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp)"
        );

    if (!files.isEmpty()) {
        imageList.append(files.toVector());
        ui->label_2->setText(QString("已添加 %1 张图片").arg(imageList.size()));
    }
}


void ghtTimer::showNextImage()
{
    if (imageList.isEmpty()) return;

    // 随机选择一张图片
    int index = QRandomGenerator::global()->bounded(imageList.size());
    QString imagePath = imageList.at(index);

    // 显示图片
    QPixmap pixmap(imagePath);
    ui->label->setPixmap(pixmap.scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

//监听函数
void ghtTimer::updateTimerInterval(double value)
{
    if (timer->isActive()) {
        timer->setInterval(value * 1000);
    }
}

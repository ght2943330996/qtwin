#ifndef PHOTOSHOP_H
#define PHOTOSHOP_H

#include <QDialog>
#include <QImage>
#include <QLabel>
#include <QString>

namespace Ui {
class photoShop;
}

class photoShop : public QDialog
{
    Q_OBJECT

public:
    explicit photoShop(QWidget *parent = nullptr);
    ~photoShop();

public:
    // photoShop();
    // QImage convertToGray(const QString &imagePath);
    // QImage meanFilter(const QImage &image);  // 均值滤波
    // void addWatermark(QImage &image, const QString &text = "官海涛"); // 添加水印



    //图像处理
    QImage ImageSetSize(QImage  qimage,QLabel *qLabel);
    int imagenum;
    QStringList srcDirPathList;
    void ImageShow(int imagenum);
    //灰度化
    QImage gray(QImage image);
    //二值化
    QImage TwoSide(QImage grayimage,int value);
    //均值滤波
    QImage avg(QImage image);
    //伽马变化
    QImage Gamma(QImage image,int value);
    // QImage Gamma(QImage image, double gamma);
    //高斯滤波
    QImage guass(QImage grayimg);
    //边缘处理
    QImage calculate(QImage img,QImage img_gray,QImage img_guass);


private slots:
    void on_ChPicBtn_clicked();

    void on_BeforeBtn_clicked();

    void on_AfterBtn_clicked();

    void on_GreyBtn_clicked();

    void on_YuanTuBtn_clicked();

    void on_TwoSideSlider_valueChanged(int value);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_TXTBtn_clicked();

    void on_GamaSlider_valueChanged(int value);

    void on_GamalineEdit_textChanged(const QString &arg1);

    void on_SideBtn_clicked();

private:
    Ui::photoShop *ui;
};

#endif // PHOTOSHOP_H

#include "photoshop.h"
#include "ui_photoshop.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>


photoShop::photoShop(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::photoShop)
{
    ui->setupUi(this);
    //关闭窗口时，自动析构释放内存
    setAttribute(Qt::WA_DeleteOnClose);

    // 设置窗口标志，添加最大化最小化按钮
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);


    // 可选：设置对齐方式
    ui->piclabel->setAlignment(Qt::AlignCenter);
}

photoShop::~photoShop()
{
    delete ui;
}


// 绘制水印
// void photoShop::addWatermark(QImage &image, const QString &text)
// {
//     QPainter painter(&image);

//     // 根据图片大小调整字体大小
//     int fontSize = qMin(image.width(), image.height()) / 30;
//     fontSize = qBound(12, fontSize, 36);  // 限制字体大小在12到36之间

//     QFont font("Arial", fontSize, QFont::Bold);
//     painter.setFont(font);

//     QFontMetrics metrics(font);
//     int textWidth = metrics.horizontalAdvance(text);
//     int textHeight = metrics.height();

//     // 计算水印位置，使其位于图片底部中间
//     int x = (image.width() - textWidth) / 2;  // 水平居中
//     int y = image.height() - textHeight - fontSize;  // 底部位置，留出一定边距

//     // 绘制实际文字
//     painter.setPen(QColor(0, 0, 0, 180));
//     painter.drawText(x, y, text);
// }



//图像相关处理

//选择图片
void photoShop::on_ChPicBtn_clicked()
{
    //打开图片文件，选择图片
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"),
                                                                QDir::homePath(), tr("图像文件(*.jpg *.png *.bmp)"));
    qDebug()<<"图片路径"<<srcDirPathListS; //在控制台输出路径观察
    srcDirPathList = srcDirPathListS;//持久化图片
    if(!srcDirPathList.isEmpty())
    {
        imagenum =0;
        QImage image=QImage(srcDirPathList.at(imagenum));//初始化选中第一张图片
        qDebug()<<"image:"<<image;
        ImageShow(imagenum);
    }
    else if(srcDirPathList.isEmpty())
    {
        return;
    }
}
//图片居中显示,图片大小与label大小相适应
QImage photoShop::ImageSetSize (QImage  qimage,QLabel *qLabel)
{
    QImage image;
    QSize imageSize = qimage.size();
    QSize labelSize = qLabel->size();

    double dWidthRatio = 1.0*imageSize.width() / labelSize.width();
    double dHeightRatio = 1.0*imageSize.height() / labelSize.height();

    // 确保label保持固定大小
    ui->piclabel->setFixedSize(ui->piclabel->size());
    if (dWidthRatio>dHeightRatio)
    {
        image = qimage.scaledToWidth(labelSize.width());
    }
    else
    {
        image = qimage.scaledToHeight(labelSize.height());
    }
    return image;

}


void photoShop::on_BeforeBtn_clicked()
{
    // 切换到上一张图片
    if (imagenum == 0) {
        imagenum = srcDirPathList.count() - 1;
    }
    else {
        imagenum = imagenum - 1;  // 修正：减1而不是加1
    }
    ImageShow(imagenum);  // 使用封装好的ImageShow函数
}

void photoShop::on_AfterBtn_clicked()
{
    // 切换到下一张图片
    if (imagenum >= srcDirPathList.count() - 1) {  // 修正：检查是否到达最后一张
        imagenum = 0;
    }
    else {
        imagenum = imagenum + 1;  // 修正：加1而不是减1
    }
    ImageShow(imagenum);  // 使用封装好的ImageShow函数
}

void photoShop::ImageShow(int imagenum)
{
    ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(QImage(srcDirPathList.at(imagenum)),ui->piclabel)));

    if(imagenum-1<0)
    {
        ui->BeforeLabel->setPixmap(QPixmap::fromImage(ImageSetSize(QImage(srcDirPathList.at(srcDirPathList.count()-1)),ui->BeforeLabel)));
    }
    else
    {
        ui->BeforeLabel->setPixmap(QPixmap::fromImage(ImageSetSize(QImage(srcDirPathList.at(imagenum-1)),ui->BeforeLabel)));
    }

    if(imagenum+1 >= srcDirPathList.count())
    {
        ui->AfterLabel->setPixmap(QPixmap::fromImage(ImageSetSize(QImage(srcDirPathList.at(0)),ui->AfterLabel)));
    }
    else {
        ui->AfterLabel->setPixmap(QPixmap::fromImage(ImageSetSize(QImage(srcDirPathList.at(imagenum+1)),ui->AfterLabel)));
    }
}

//灰度化
QImage photoShop::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

    for(int y = 0; y < newImage.height(); y++)
    {
        for(int x = 0; x < newImage.width(); x++)
        {
            oldColor = QColor(image.pixel(x,y));
            int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
            newImage.setPixel(x, y, qRgb(average, average, average));
        }
    }
    return newImage;
}
//灰度化槽
void photoShop::on_GreyBtn_clicked()
{
    if(srcDirPathList.isEmpty())
    {
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
        QImage image=QImage(srcDirPathList.at(imagenum));
        QImage grayimage=gray(image);
        // 确保label保持固定大小
        ui->piclabel->setFixedSize(ui->piclabel->size());
        ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(grayimage,ui->piclabel)));
    }
}
//显示原图
void photoShop::on_YuanTuBtn_clicked()
{
    if(srcDirPathList.isEmpty())
    {
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
        QImage image=QImage(srcDirPathList.at(imagenum));
        ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(image,ui->piclabel)));
    }
}


QImage photoShop::TwoSide(QImage image, int value) {
    // 先进行灰度化
    QImage grayImage = gray(image);
    QImage TwoSideImage = grayImage.convertToFormat(QImage::Format_ARGB32);

    for(int y = 0; y < grayImage.height(); y++) {
        for(int x = 0; x < grayImage.width(); x++) {
            // 因为已经灰度化，只需要读取一个通道即可
            int gray = qRed(grayImage.pixel(x,y));
            int ts = (gray < value) ? 0 : 255;
            TwoSideImage.setPixel(x, y, qRgb(ts, ts, ts));
        }
    }
    return TwoSideImage;
}


//二值化调节条
void photoShop::on_TwoSideSlider_valueChanged(int value)
{
    if(srcDirPathList.isEmpty()){
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
        QImage image=QImage(srcDirPathList.at(imagenum));//读取当前图片
        QImage grayimage=gray(image);//灰度化
        QImage TwoSideImage=TwoSide(grayimage,value);//二值化
        ui->lineEdit->setText(QString::number(value));//改变文本框内值为二值化比对值
        ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(TwoSideImage,ui->piclabel)));//显示二值化图像
    }
}

//二值化文本框
void photoShop::on_lineEdit_textChanged(const QString &arg1)
{
    if(srcDirPathList.isEmpty()){
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
        int value=arg1.toInt();
        if (value>=0 && value<=255)
        {
            QImage image=QImage(srcDirPathList.at(imagenum));
            QImage grayimage=gray(image);
            QImage TwoSideImage=TwoSide(grayimage,value);//都是和上面一样的
            ui->TwoSideSlider->setValue(value);//当文本框内数值改变时，动态变化调节条位置
            ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(TwoSideImage,ui->piclabel)));
        }
        else
        {
            QMessageBox::information(this,tr("请输入正确数值"),
                                     tr("请输入0-255！"));
            return;
        }
    }
}

//均值滤波
void photoShop::on_TXTBtn_clicked()
{
    if(srcDirPathList.isEmpty())
    {
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
        QImage image=QImage(srcDirPathList.at(imagenum));
        QImage avgimage=avg(image);
        ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(avgimage,ui->piclabel)));
    }
}


//均值滤波优化
QImage photoShop::avg(QImage image) {
    // 先进行灰度化
    QImage grayImage = gray(image);
    QImage result = grayImage.copy();

    const int kernel[3][3] = {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };
    const int kernelSize = 3;
    const int sumKernel = 9;

    for(int x = kernelSize/2; x < grayImage.width() - kernelSize/2; x++) {
        for(int y = kernelSize/2; y < grayImage.height() - kernelSize/2; y++) {
            int sum = 0;

            // 只需要处理一个通道
            for(int i = -kernelSize/2; i <= kernelSize/2; i++) {
                for(int j = -kernelSize/2; j <= kernelSize/2; j++) {
                    sum += qRed(grayImage.pixel(x+i, y+j)) * kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }

            int avg = qBound(0, sum/sumKernel, 255);
            result.setPixel(x, y, qRgb(avg, avg, avg));
        }
    }
    return result;
}

//伽马变换
QImage photoShop::Gamma(QImage image,int value)
{
    QImage GammaImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

    for(int y = 0; y < image.height(); y++)
    {
        for(int x = 0; x < image.width(); x++)
        {
            oldColor = QColor(image.pixel(x,y));
            double red=oldColor.red();
            double green=oldColor.green();
            double blue=oldColor.blue();

            int r=qBound(0,(int)pow(red,value),255);
            int g=qBound(0,(int)pow(green,value),255);
            int b=qBound(0,(int)pow(blue,value),255);
            GammaImage.setPixel(x,y, qRgb(r, g, b));

        }
    }
    return GammaImage;
}

//调节条伽马变换
void photoShop::on_GamaSlider_valueChanged(int value)
{
    if(srcDirPathList.isEmpty()){
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
        QImage image=QImage(srcDirPathList.at(imagenum));//读取当前图片
        double values=value;
        QImage GammaImage=Gamma(image,values/10);//伽马变换
        ui->GamalineEdit->setText(QString::number(values/10));//改变文本框内值为伽马因子
        ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(GammaImage,ui->piclabel)));//显示伽马变换图像
    }
}


//文本框伽马变换

void photoShop::on_GamalineEdit_textChanged(const QString &arg1)
{
    if(srcDirPathList.isEmpty()){
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
         double value=arg1.toDouble();
        if (value>=0 && value<=25)
        {
            QImage image=QImage(srcDirPathList.at(imagenum));
            QImage GammaImage=Gamma(image,value);//都是和上面一样的
            value=value*10;
            ui->GamaSlider->setValue(value);//当文本框内数值改变时，动态变化调节条位置
            ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(GammaImage,ui->piclabel)));
        }
        else
        {
            QMessageBox::information(this,tr("请输入正确数值"),
                                     tr("请输入0-25！"));
            return;
        }
    }
}


//高斯平滑滤波器
void generic_guess(double kernel[10][10],int size,double thelt){
    double pi=acos(-1);
    double sum=0;
    int mk=size/2;
    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            kernel[i][j]=exp(-sqrt(pow(i-mk,2)+pow(j-mk,2))/(2*thelt*thelt));
            kernel[i][j]/=2*pi*thelt*thelt;
            sum+=kernel[i][j];
        }
    }
    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            kernel[i][j]/=sum;
        }
    }
}
//高斯滤波
QImage photoShop::guass(QImage grayimg){
    QImage guassimg;
    int k=5;//高斯滤波器规模
    double kernel[10][10];
    generic_guess(kernel,k,1.4);//设置好高斯滤波器
    //t_img用于将灰度图填充到长款都加上高斯滤波器的尺寸大小
    QImage * t_img=new QImage(grayimg.width()+k-1,grayimg.height()+k-1,QImage::Format_ARGB32);
    guassimg=QImage(grayimg.width(),grayimg.height(),QImage::Format_ARGB32);

    //lefttop
    for(int i=0;i<k/2;++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i,j,grayimg.pixel(0,0));

        }
    }

    //righttop
    for(int i=0;i<k/2;++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i,j,grayimg.pixel(grayimg.width()-1,0));

        }
    }

    //rightbottom
    for(int i=grayimg.width()+k/2;i<t_img->width();++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i,j,grayimg.pixel(grayimg.width()-1,grayimg.height()-1));
        }
    }

    //leftbottom
    for(int i=0;i<k/2;++i){
        for(int j=grayimg.height()+k/2;j<t_img->height();++j){
            t_img->setPixel(i,j,grayimg.pixel(0,grayimg.height()-1));

        }
    }

    //top
    for(int i=0;i<grayimg.width();++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i+k/2,j,grayimg.pixel(i,0));

        }
    }

    //right
    for(int i=t_img->width()-k/2;i<t_img->width();++i){
        for(int j=0;j<grayimg.height();++j){
            t_img->setPixel(i,j+k/2,grayimg.pixel(i,grayimg.height()-1));

        }
    }

    //left
    for(int i=0;i<k/2;++i){
        for(int j=0;j<grayimg.height();++j){
            t_img->setPixel(i,j+k/2,grayimg.pixel(0,j));
        }
    }

    for(int i=0;i<grayimg.width();++i){
        for(int j=0;j<grayimg.height();++j){
            t_img->setPixel(i+k/2,j+k/2,grayimg.pixel(i,j));

        }
    }

    for(int i=k/2;i<t_img->width()-k/2;++i){
        for(int j=k/2;j<t_img->height()-k/2;++j){
            double temp=0;
            for(int ti=0;ti<k;++ti){
                for(int tj=0;tj<k;++tj){
                    temp+=kernel[ti][tj]*qRed(t_img->pixel(i-k/2+ti,j-k/2+tj));
                }
            }
            guassimg.setPixel(i-k/2,j-k/2,qRgb(temp,temp,temp));
        }

    }
    return guassimg;
}

QImage photoShop::calculate(QImage img,QImage img_gray,QImage img_guass)
{
    //计算梯度强度和方向
    double * gradx[img.width()];
    for(int i=0;i<img.width();++i)
        gradx[i]=new double[img.height()];
    double * grady[img.width()];
    for(int i=0;i<img.width();++i)
        grady[i]=new double[img.height()];
    double * grad[img.width()];
    for(int i=0;i<img.width();++i)
        grad[i]=new double[img.height()];
    double * dir[img.width()];
    for(int i=0;i<img.width();++i)
        dir[i]=new double[img.height()];
    int k=3;//sobel算子规模
    double kernelx[3][3]={-1,0,1,-2,0,2,-1,0,1};
    double kernely[3][3]={1,2,1,0,0,0,-1,-2,-1};

    QImage * t_img=new QImage(img_gray.width()+k-1,img_gray.height()+k-1,QImage::Format_ARGB32);
    //lefttop
    for(int i=0;i<k/2;++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i,j,img_gray.pixel(0,0));
        }
    }
    //righttop
    for(int i=0;i<k/2;++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i,j,img_gray.pixel(img_gray.width()-1,0));
        }
    }
    //rightbottom
    for(int i=img_gray.width()+k/2;i<t_img->width();++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i,j,img_gray.pixel(img_gray.width()-1,img_gray.height()-1));
        }
    }
    //leftbottom
    for(int i=0;i<k/2;++i){
        for(int j=img_gray.height()+k/2;j<t_img->height();++j){
            t_img->setPixel(i,j,img_gray.pixel(0,img_gray.height()-1));
        }
    }
    //top
    for(int i=0;i<img_gray.width();++i){
        for(int j=0;j<k/2;++j){
            t_img->setPixel(i+k/2,j,img_gray.pixel(i,0));
        }
    }
    //right
    for(int i=t_img->width()-k/2;i<t_img->width();++i){
        for(int j=0;j<img_gray.height();++j){
            t_img->setPixel(i,j+k/2,img_gray.pixel(img_gray.width()-1,j));
        }
    }
    //bottom
    for(int i=0;i<img_gray.width();++i){
        for(int j=t_img->height()-k/2;j<t_img->height();++j){
            t_img->setPixel(i+k/2,j,img_gray.pixel(i,img_gray.height()-1));
        }
    }
    //left
    for(int i=0;i<k/2;++i){
        for(int j=0;j<img_gray.height();++j){
            t_img->setPixel(i,j+k/2,img_gray.pixel(0,j));
        }
    }
    for(int i=0;i<img_gray.width();++i){
        for(int j=0;j<img_gray.height();++j){
            t_img->setPixel(i+k/2,j+k/2,img_gray.pixel(i,j));
        }
    }
    for(int i=k/2;i<t_img->width()-k/2;++i){
        for(int j=k/2;j<t_img->height()-k/2;++j){
            double tempx=0;
            double tempy=0;
            for(int ti=0;ti<k;++ti){
                for(int tj=0;tj<k;++tj){
                    tempx+=kernelx[ti][tj]*qRed(t_img->pixel(i-k/2+ti,j-k/2+tj));
                    tempy+=kernely[ti][tj]*qRed(t_img->pixel(i-k/2+ti,j-k/2+tj));
                }
            }
            gradx[i-k/2][j-k/2]=tempx;
            grady[i-k/2][j-k/2]=tempy;
            grad[i-k/2][j-k/2]=sqrt(pow(tempx,2)+pow(tempy,2));
            double theta=atan(tempy/tempx)+90;
            if (theta >= 0 && theta < 45)
                dir[i-k/2][j-k/2] = 2;
            else if (theta >= 45 && theta < 90)
                dir[i-k/2][j-k/2] = 3;
            else if (theta >= 90 && theta < 135)
                dir[i-k/2][j-k/2] = 0;
            else
                dir[i-k/2][j-k/2] = 1;
        }
    }


    //非极大值抑制图像制作

    QImage img_nms=QImage(img_guass.width(),img_guass.height(),QImage::Format_ARGB32);
    double temp=0;
    for(int i=0;i<img_nms.width();++i){
        for(int j=0;j<img_nms.height();++j){
            temp+=grad[i][j];
        }
    }
    temp/=img_nms.width()*img_nms.height();

    double highthresh=temp;
    double lowthresh=highthresh;

    double N,NE,E,SW,W,SE,S,NW;
    double grad1=0,grad2=0,tantheta=0;



    for(int i=1;i<img_nms.width()-1;++i){
        for(int j=1;j<img_nms.height();++j){
            N=grad[i][j-1];
            NE=grad[i+1][j-1];
            E=grad[i+1][j];
            SW=grad[i-1][j+1];
            W=grad[i-1][j];
            SE=grad[i+1][j+1];
            S=grad[i][j+1];
            NW=grad[i-1][j-1];
            if(dir[i][j]==0){
                tantheta=abs(grady[i][j]/gradx[i][j]);
                grad1=E*(1-tantheta)+NE*tantheta;
                grad2=W*(1-tantheta)+SW*tantheta;
            }
            else if(dir[i][j]==1){
                tantheta=abs(gradx[i][j]/grady[i][j]);
                grad1=N*(1-tantheta)+NE*tantheta;
                grad2=S*(1-tantheta)+SW*tantheta;
            }
            else if(dir[i][j]==2){
                tantheta=abs(gradx[i][j]/grady[i][j]);
                grad1=N*(1-tantheta)+NW*tantheta;
                grad2=S*(1-tantheta)+SE*tantheta;
            }
            else if(dir[i][j]==3){
                tantheta=abs(grady[i][j]/gradx[i][j]);
                grad1=W*(1-tantheta)+NW*tantheta;
                grad2=E*(1-tantheta)+SE*tantheta;
            }
            else{
                grad1=highthresh;
                grad2=highthresh;
            }
            if(grad[i][j]>grad1&&grad[i][j]>grad2){
                img_nms.setPixel(i,j,qRgb(255,255,255));//black边缘
                //                gradcp[i][j]=highthresh;
            }
            else{
                img_nms.setPixel(i,j,qRgb(0,0,0));//white
                grad[i][j]=0;
            }
        }
    }

    //双阈值检测
    QImage img_dt= QImage(img_nms.width(),img_nms.height(),QImage::Format_ARGB32);
    double * gradcp[img.width()];
    for(int i=0;i<img.width();++i)
        gradcp[i]=new double[img.height()];


    for(int i=0;i<img_dt.width();++i){
        for(int j=0;j<img_dt.height();++j){
            if(grad[i][j]>highthresh){//强边缘
                gradcp[i][j]=highthresh;
                img_dt.setPixel(i,j,qRgb(255,255,255));
            }
            else if(grad[i][j]>lowthresh){//弱边缘
                gradcp[i][j]=lowthresh;
                img_dt.setPixel(i,j,qRgb(255,255,255));
            }
            else{//非边缘
                gradcp[i][j]=0;
                img_dt.setPixel(i,j,qRgb(0,0,0));//抑制
            }
        }
    }

    QImage img_st=QImage(img_dt.width(),img_dt.height(),QImage::Format_ARGB32);
    int frac[8][2]={{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
    for(int i=1;i<img_st.width()-1;++i){
        for(int j=1;j<img_st.height()-1;++j){
            if(gradcp[i][j]==highthresh){//强边缘
                img_st.setPixel(i,j,qRgb(255,255,255));
            }
            else if(gradcp[i][j]==lowthresh){//弱边缘
                for(int p=0;p<8;++p){
                    if(gradcp[i+frac[p][0]][j+frac[p][1]]==highthresh){
                        img_st.setPixel(i,j,qRgb(255,255,255));//边缘
                        break;
                    }
                    img_st.setPixel(i,j,qRgb(0,0,0));//非边缘
                }
            }
            else//非边缘
                img_st.setPixel(i,j,qRgb(0,0,0));
        }
    }
    return img_st;

}

void photoShop::on_SideBtn_clicked()
{
    if(srcDirPathList.isEmpty())
    {
        QMessageBox::information(this,tr("请先选择图片"),
                                 tr("请先选择图片！"));
        return;
    }
    else{
        QImage image=QImage(srcDirPathList.at(imagenum));
        QImage grayimage=gray(image);
        QImage guassimage=guass(image);
        QImage stimage=calculate(image,grayimage,guassimage);
        ui->piclabel->setPixmap(QPixmap::fromImage(ImageSetSize(stimage,ui->piclabel)));
    }

}

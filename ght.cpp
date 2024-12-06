#include "ght.h"

//这是构造函数，一般都是无参的，不是自定义函数注意区分
Ght::Ght()
{
    //实例化,不需要实例化，而且在同一个类中，还没开始使用服务呢
    //QGht *image = new QGht(this);
}

QString Ght::imgToAscii(const QImage &image)
{
    QString result;
    // ASCII字符从亮到暗的排序
    const QString asciiChars = " .°*oO#@";;  // 注意这里字符顺序反过来了


    // 计算需要添加的左侧空格数来实现水平居中
    int spacePerLine = 15;  // 可以调整这个值来改变水平位置


    // 预分配字符串空间以提高性能
    result.reserve(image.width() * image.height() * 2);

    for (int y = 0; y < image.height(); y++) {

        // 添加每行的前导空格
        result += QString(spacePerLine, ' ');

        for (int x = 0; x < image.width(); x++) {
            QColor color = image.pixelColor(x, y);
            int gray = qGray(color.rgb());

            // 将灰度值（0-255）映射到ASCII字符数组的索引
            int index = gray * (asciiChars.length() - 1) / 255;
            result += asciiChars[index];
            result += " ";  // 在每个字符后添加空格，防止字符过于密集
        }
        result += "\n";
    }

    return result;

}










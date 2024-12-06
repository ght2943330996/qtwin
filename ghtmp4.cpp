#include "ghtmp4.h"
#include <QVBoxLayout>
#include <QFont>
#include <qfileinfo.h>

GhtMp4::GhtMp4(QWidget *parent) : QWidget(parent) {
    setupUI();
}

GhtMp4::~GhtMp4() {
    stop();
}

void GhtMp4::setupUI() {
    // 创建布局
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 初始化媒体播放组件
    player = new QMediaPlayer(this);
    videoSink = new QVideoSink(this);
    display = new QTextEdit(this);
    // 初始化音频组件
    audioPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    // 配置音频输出
    audioPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);

    // 设置显示字体
    QFont font("Courier New", 8);
    font.setFixedPitch(true);
    display->setFont(font);
    display->setReadOnly(true);   // 设置为只读模式

    // 设置纯黑背景和纯白前景
    // display->setStyleSheet("QTextEdit { background-color: #000000; color: #FFFFFF; }");
    // display->setLineWrapMode(QTextEdit::NoWrap);

    // 将显示组件添加到布局中
    layout->addWidget(display);
    setLayout(layout);

    // 连接信号和槽
    //视频帧捕捉QVideoSink会捕获视频的每一帧
    //它作为视频帧的接收器，当播放器解码出新的视频帧时会自动更新到 videoSink,更新过程是由 Qt 多媒体框架内部处理的
    player->setVideoSink(videoSink);
    connect(videoSink, &QVideoSink::videoFrameChanged,this, &GhtMp4::processFrame); //跳转到processFrame


}

void GhtMp4::openVideo(const QString& filePath) {
    currentFilePath = filePath;

    // 设置视频源位置,并将将本地文件路径转换为 QUrl 格式
    //实际的视频流读取是在调用 play() 后由 QMediaPlayer 内部处理的
    player->setSource(QUrl::fromLocalFile(filePath));

    // 构造对应的音频资源路径
    // 假设您的音频文件在 qrc:/audio/ 目录下
    QString audioFileName = QFileInfo(filePath).fileName();
    QString audioPath = "qrc:/voice/voice/" + audioFileName;  // 根据实际路径调整

    // 设置音频源
    audioPlayer->setSource(QUrl(audioPath));

    //不需要参数是因为 player 已经通过 setSource 知道要播放什么文件
    //play() 是一个触发器，告诉 QMediaPlayer 开始从之前设置的源读取和播放媒体
    play();
}

void GhtMp4::play() {
    player->play();
    audioPlayer->play();
}

void GhtMp4::pause() {
    player->pause();
    audioPlayer->pause();
}

void GhtMp4::stop() {
    player->stop();
    audioPlayer->stop();
}

//视频帧转图像
void GhtMp4::processFrame(const QVideoFrame& frame) {
    // 将视频帧转换为QImage
    QImage image = frame.toImage();
    // 缩放图像以获得合适的ASCII显示大小
    image = image.scaled(220, 60);
    // 转换为ASCII并显示
    display->setText(convertToAscii(image));
}

// 将图像转换为ASCII字符艺术
QString GhtMp4::convertToAscii(const QImage& image) {
    // 定义字符和它们对应的灰度范围
    struct CharRange {
        int minGray;    // 灰度最小值
        int maxGray;    // 灰度最大值
        QChar ch;       // 对应的ASCII字符
    };

    // 自定义每个字符对应的灰度范围
    //白底
    const QVector<CharRange> charRanges = {
        {0, 30, '$'},      // 最暗
        {31, 60, '@'},
        {61, 90, '#'},
        {91, 120, '%'},
        {121, 150, '+'},
        {151, 180, ':'},
        {181, 210, '.'},
        {211, 255, ' '}    // 最亮
    };

    //黑底
    // const QVector<CharRange> charRanges = {
    //     {0, 30, ' '},      // 最暗
    //     {31, 60, '.'},
    //     {61, 90, ':'},
    //     {91, 120, '+'},
    //     {121, 150, '%'},
    //     {151, 180, '#'},
    //     {181, 210, '@'},
    //     {211, 255, '$'}    // 最亮
    // };

    QString result;
    // 遍历图像的每个像素

    // 计算需要添加的左侧空格数来实现水平居中
    int spacePerLine = 5;  // 可以调整这个值来改变水平位置

    for (int y = 0; y < image.height(); ++y) {

        // 添加每行的前导空格
        result += QString(spacePerLine, ' ');

        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = image.pixel(x, y);
            int gray = qGray(pixel); // 获取像素的灰度值

            // 查找当前灰度值所属的范围
            QChar ch = ' ';  // 默认字符
            for (const CharRange& range : charRanges) {
                if (gray >= range.minGray && gray <= range.maxGray) {
                    ch = range.ch;
                    break;
                }
            }

            result += ch;
        }
        result += "\n";
    }

    return result;
}








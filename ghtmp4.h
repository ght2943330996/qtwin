#ifndef GHTMP4_H
#define GHTMP4_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoFrame>  // 添加这行
#include <QTextEdit>

#include <QAudioOutput>  // 添加音频输出头文件


class GhtMp4 : public QWidget {
    Q_OBJECT

public:
    explicit GhtMp4(QWidget *parent = nullptr);
    ~GhtMp4();

    // 视频控制函数
    void openVideo(const QString& filePath);// 打开视频文件
    void play();    // 播放视频    未实现
    void pause();   // 暂停视频
    void stop();    // 停止视频

    // 添加一些获取播放状态的方法   未实现
    bool isPlaying() const;
    QString currentFile() const;

private slots:
    // 处理视频帧
    void processFrame(const QVideoFrame& frame);

private:
    // UI初始化
    void setupUI();
    // 将图像转换为ASCII字符
    QString convertToAscii(const QImage& image);

    // 媒体播放相关组件
    QMediaPlayer* player;   // 视频播放器
    QVideoSink* videoSink;    // 视频输出接收器
    QTextEdit* display;      // ASCII显示区域
    QString currentFilePath;   // 当前视频文件路径

    // 音频相关成员
    QMediaPlayer* audioPlayer;  // 添加音频播放器
    QAudioOutput* audioOutput;  // 添加音频输出


};

#endif // GHTMP4_H











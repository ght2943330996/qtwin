#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ght.cpp"
#include "ghtmp4.h"

#include <QActionGroup>
#include <QToolButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QLabel>

#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>

#include <QFileDialog>
#include <QMdiSubWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "photoshop.h"

#include <QDebug>
#include <QMessageBox>

// 实现自定义启动画面类的构造函数
MySplash::MySplash(const QPixmap &pixmap) : QSplashScreen(pixmap)
{
    progressBar = new QProgressBar(this);

    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: none;"           // 移除边框
        // "   border: 2px solid grey;"
        "   border-radius: 5px;"
        "   background-color: rgba(255, 255, 255, 100);"  // 半透明背景
        // "   background-color: #FFFFFF;"
        "   text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #f0ddc2;"
        "   width: 20px;"
        "}"
        );

    progressBar->setGeometry(0,                    // x = 0，从最左边开始
                pixmap.height() - 20,   // y = 图片高度 - 进度条高度
                pixmap.width(),         // 宽度 = 图片宽度
                20);                    // 高度设为20像素
}

// 实现设置进度的方法
void MySplash::setProgress(int value)
{
    progressBar->setValue(value);
}

// MainWindow的实现
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    strFile = "D:/waverly/Study/junior/Qt_Item/229480308-ght-work6/";
    //加载 翻译文件
    // 检查翻译文件加载是否成功
    if (!ch.load(strFile + "229480308-ght-work6_zh_CN.qm")) {
        qDebug() << "Failed to load Chinese translation file";
    }

    if (!en.load(strFile + "229480308-ght-work6_en.qm")) {
        qDebug() << "Failed to load English translation file";
    }


    ght = new Ght();  // 实例化成对象

    // 设置字体选择器只显示等宽字体
    ui->fontComboBox->setFontFilters(QFontComboBox::MonospacedFonts);


    QMenu *editMenu = ui->menuBar->addMenu(tr("编辑(&E)")); // 添加编辑菜单
    QAction *action_Open = editMenu->addAction(             // 添加打开菜单
    QIcon(":/images/images/open.png"), tr("打开文件(&O)"));
    action_Open->setShortcut(QKeySequence("Ctrl+O"));       // 设置快捷键
    ui->mainToolBar->addAction(action_Open);                // 在工具栏中添加动作
    connect(action_Open,&QAction::triggered,this, &MainWindow::on_pushButton_clicked); //传递函数指针

    QActionGroup *group = new QActionGroup(this);           // 建立动作组
    QAction *action_L = group->addAction(tr("左对齐(&L)")); // 向动作组中添加动作
    action_L->setCheckable(true);                  // 设置动作checkable属性为true
    QAction *action_R = group->addAction(tr("右对齐(&R)"));
    action_R->setCheckable(true);
    QAction *action_C = group->addAction(tr("居中(&C)"));
    action_C->setCheckable(true);
    action_L->setChecked(true);                    // 最后指定action_L为选中状态
    editMenu->addSeparator();                      // 向菜单中添加分隔线
    editMenu->addAction(action_L);                 // 向菜单中添加动作
    editMenu->addAction(action_R);
    editMenu->addAction(action_C);

    connect(action_L, &QAction::triggered, this, [this]() { setTextLine(Qt::AlignLeft); });   //Qt::Alignment 是枚举类型，本质上就是一个整数
    connect(action_R, &QAction::triggered, this, [this]() { setTextLine(Qt::AlignRight); });
    connect(action_C, &QAction::triggered, this, [this]() { setTextLine(Qt::AlignCenter); });

    QToolButton *toolBtn = new QToolButton(this);          // 创建QToolButton 颜色容器
    toolBtn->setText(tr("颜色"));
    QMenu *colorMenu = new QMenu(this);                // 创建一个菜单（后面写成下拉模式）
    toolBtn->setMenu(colorMenu);                        // 添加菜单到容器
    toolBtn->setPopupMode(QToolButton::MenuButtonPopup); // 设置容器为弹出模式 QToolButton::MenuButtonPopup有箭头
    ui->mainToolBar->addWidget(toolBtn);                 // 向ui工具栏中添加QToolButton按钮容器

    // 添加颜色动作（容器内容）
    QAction *redAction = colorMenu->addAction(tr("红色"));
    QAction *greenAction = colorMenu->addAction(tr("绿色"));
    QAction *blueAction = colorMenu->addAction(tr("蓝色"));

    connect(redAction, &QAction::triggered, this, [this]() { changeTextColor(Qt::red); });   //1.triggered 是 QAction 类特有的信号,且响应比点击更广
    connect(greenAction, &QAction::triggered, this, [this]() { changeTextColor(Qt::green); }); //2.greenAction已经是指针了不用加&
    connect(blueAction, &QAction::triggered, this, [this]() { changeTextColor(Qt::blue); });  //3.Qt::blue是Qt中的枚举值，但是会被自动转换成QColor 对象


    spinBox = new QSpinBox(this);
    spinBox->setRange(6, 72);        // 设置字体大小范围
    spinBox->setValue(8);            // 设置默认值
    spinBox->setPrefix(tr("字体: ")); // 可选：添加前缀
    ui->mainToolBar->addWidget(spinBox);


    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeFontSize);


    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged,this, &MainWindow::changeTextFont);


    // 显示临时消息，显示2000毫秒即2秒
    ui->statusBar->showMessage(tr("欢迎使用多文档编辑器"), 5000);


    QLabel *permanent = new QLabel(this);
    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken); // 设置标签的边框样式
    permanent->setText("229480308官海涛");
    ui->statusBar->addPermanentWidget(permanent);// 将标签作为永久部件添加到状态栏右侧
    ui->statusBar->setSizeGripEnabled(false); //禁用状态栏右下角的大小调整控制柄

}

MainWindow::~MainWindow()
{
    delete ght;
    delete ui;
}


void MainWindow::on_action_N_triggered()
{
    // 新建文本编辑器部件
    QTextEdit *edit = new QTextEdit(this);
    // 使用QMdiArea类的addSubWindow()函数创建子窗口，以文本编辑器为中心部件
    QMdiSubWindow *child = ui->mdiArea->addSubWindow(edit);
    child->setWindowTitle(tr("多文档编辑器子窗口"));
    child->show();

}


void MainWindow::on_action_Dock_triggered()
{
    ui->dockWidget->show();
}


//打开文件
void MainWindow::on_pushButton_clicked()
{

    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开BMP文件"), "",tr("BMP Files (*.bmp);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    //加载图像
    QImage image(fileName);
    if (image.isNull()) {
        QMessageBox::warning(this, tr("错误"), tr("无法加载图像文件！"));
        return;
    }

    // 调整图像大小
    image = image.scaled(100, 75, Qt::KeepAspectRatio);

    // 新建文本编辑器部件
    QTextEdit *edit = new QTextEdit(this);
    // 使用QMdiArea类的addSubWindow()函数创建子窗口，以文本编辑器为中心部件
    QMdiSubWindow *child = ui->mdiArea->addSubWindow(edit);
    child->setWindowTitle(tr("多文档编辑器子窗口"));
    child->show();

    // 设置等宽字体
    QFont font("Courier New");  // 使用 Courier New 等宽字体
    font.setStyleHint(QFont::Monospace);  // 如果 Courier New 不可用，使用系统默认等宽字体
    font.setFixedPitch(true);
    font.setPointSize(8);  // 设置合适的字体大小
    edit->setFont(font);

    // 直接使用edit显示ASCII艺术，因为edit就是我们新创建的文本编辑器
    QString asciiArt = ght->imgToAscii(image);
    edit->setText(asciiArt);
}


//文本对齐
void MainWindow::setTextLine(Qt::Alignment alignment)
{
    // 获取当前活动的子窗口
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    if (!currentWindow) {
        return;  // 如果没有活动窗口，直接返回
    }

    // 将子窗口的widget转换为QTextEdit
    QTextEdit *edit = qobject_cast<QTextEdit*>(currentWindow->widget());
    if (!edit) {
        return;  // 如果转换失败，直接返回
    }

    // 获取当前文本编辑器的光标对象
    QTextCursor cursor = edit->textCursor();

    // 创建文本块格式对象
    QTextBlockFormat blockFormat;
    // 设置对齐方式（左对齐、右对齐或居中）
    blockFormat.setAlignment(alignment);

    // 检查是否有文本被选中
    if (cursor.hasSelection()) {
        // 创建光标副本，用于创建可撤销的编辑块
        QTextCursor c = cursor;
        // 开始一个编辑块，使多个操作可以作为一个整体撤销
        c.beginEditBlock();

        // 获取选中文本的起始位置
        int start = cursor.selectionStart();
        // 获取选中文本的结束位置
        int end = cursor.selectionEnd();
        // 将光标移动到选中文本的开始位置
        cursor.setPosition(start);

        // 循环处理选中范围内的每个文本块（段落）
        do {
            // 将当前文本块的格式与新的对齐方式合并
            cursor.mergeBlockFormat(blockFormat);
            // 将光标移动到下一个文本块
            cursor.movePosition(QTextCursor::NextBlock);
            // 继续循环，直到处理完所有选中的文本或到达文档末尾
        } while(cursor.position() < end && !cursor.atEnd());

        // 结束编辑块，使所有的格式更改可以一次性撤销
        c.endEditBlock();
    } else {
        // 如果没有选中文本，只对当前光标所在的文本块应用格式
        cursor.mergeBlockFormat(blockFormat);
    }
}

//颜色修改
void MainWindow::changeTextColor(const QColor &color)
{
    currentTextColor = color;

    // 获取当前活动的子窗口
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    if (currentWindow) {
        QTextEdit *edit = qobject_cast<QTextEdit*>(currentWindow->widget());
        if (edit) {
            // 更新文本编辑器的调色板
            QPalette palette = edit->palette();
            palette.setColor(QPalette::Text, currentTextColor);
            edit->setPalette(palette);
        }
    }
}

//字体修改
void MainWindow::changeTextFont(const QFont &font)
{
    // 保持等宽和大小设置
    currentFont = font;
    currentFont.setStyleHint(QFont::Monospace);
    currentFont.setFixedPitch(true);

    // 获取当前活动的子窗口
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
        if (currentWindow) {
            QTextEdit *edit = qobject_cast<QTextEdit*>(currentWindow->widget());
        if (edit) {
            edit->setFont(currentFont);
        }
    }
}
//字体大小
void MainWindow::changeFontSize(int size)
{
    // 更新当前字体大小
    currentFont.setPointSize(size);

    // 获取当前活动的子窗口
    QMdiSubWindow *currentWindow = ui->mdiArea->activeSubWindow();
    if (currentWindow) {
        QTextEdit *edit = qobject_cast<QTextEdit*>(currentWindow->widget());
        if (edit) {
            QFont font = edit->font();
            font.setPointSize(size);
            edit->setFont(font);
        }
    }
}

//视频
//MP4文件 -> 视频帧 -> 灰度图像 -> ASCII字符 -> 动画显示
void MainWindow::on_pushButton_2_clicked()
{
    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开mp4文件"), "",tr("mp4 Files (*.mp4);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    // 创建ASCII播放器窗口
    GhtMp4* player = new GhtMp4(this);
    QMdiSubWindow *child = ui->mdiArea->addSubWindow(player);

    // 设置窗口标题为文件名
    QFileInfo fileInfo(fileName);
    child->setWindowTitle(fileInfo.fileName());

    child->setMinimumSize(400, 300);
    child->show();

    // 开始播放视频
    player->openVideo(fileName);
}

//定时器
void MainWindow::on_pushButton_3_clicked()
{
    if(!ght_timer){
        ght_timer = new ghtTimer(this);
    }
    ght_timer->exec();
}




//打开图像处理
void MainWindow::on_processButton_clicked()
{
    if(!photoShop){
        photoShop = new class photoShop(this);
    }
    photoShop->exec();
}



//保存当前窗口图像
void MainWindow::on_action_S_triggered()
{
    // 获取当前活动的子窗口
    QMdiSubWindow *currentSubWindow = ui->mdiArea->currentSubWindow();
    if (!currentSubWindow)
        return;

    // 获取子窗口的内容控件
    QWidget *container = currentSubWindow->widget();
    if (!container)
        return;

    // 查找 QLabel（包含图片）
    QLabel *imageLabel = container->findChild<QLabel*>();
    if (!imageLabel || imageLabel->pixmap().isNull())
        return;

    // 打开保存对话框
    QString savePath = QFileDialog::getSaveFileName(this,tr("保存图片"), "",tr("PNG图像 (*.png);;JPEG图像 (*.jpg *.jpeg);;BMP图像 (*.bmp)"));

    if (!savePath.isEmpty()) {
        // 获取原始大小的图片（不是缩放后的）并保存
        QPixmap pixmap = imageLabel->pixmap();
        if (!pixmap.save(savePath)) {
            QMessageBox::warning(this, tr("保存失败"),tr("图片保存失败，请检查保存路径和权限。"));
        }
    }
}

//打开火灾检测
void MainWindow::on_pushButton_4_clicked()
{
    if(!fire){
        fire = new class fire(this);
    }
    fire->exec();
}



void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "中文"){
        qApp->installTranslator(&ch);
        ui->retranslateUi(this);
    }else{
        qApp->installTranslator(&en);
        ui->retranslateUi(this);
    }
}


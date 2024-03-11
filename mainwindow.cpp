#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mybutton.h"
#include "choosemapscene.h"
#include <QDebug>
#include <QTimer>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1260,900);
    this->setWindowIcon(QIcon(":/image/yellowdiamond4.png"));
    this->setWindowTitle("Tower Defence");
    //初始化图片
    pixbg=QPixmap(":/image/mainbg.png");
    pixyhb=QPixmap(":/image/pixyhb.png");
    pixyhb=pixyhb.scaled(QSize(pixyhb.width()*0.6,pixyhb.height()*0.6),Qt::KeepAspectRatio);
    pixtd=QPixmap(":/image/towerDefence.png");
    pixtd=pixtd.scaled(QSize(pixtd.width()*1.2,pixtd.height()*1.2),Qt::KeepAspectRatio);
    //初始化音效
    buttonPlayer = new QMediaPlayer;
    buttonAudioOutput = new QAudioOutput;
    buttonPlayer->setAudioOutput(buttonAudioOutput);
    buttonPlayer->setSource(QUrl::fromLocalFile("../project2/media/buttonClick.mp3"));

    //初始化按钮
    MyButton * startBtn=new MyButton(":/image/Button.png",0.85);
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.6);

    chooseMapScene * mapScene = new chooseMapScene;

    //触发开始信号 进入地图选项场景
    connect(startBtn,&QPushButton::clicked,this,[=](){
       //弹跳特效
        startBtn->zoomdown();
        startBtn->zoomup();
        //按钮音效
        buttonPlayer->play();
        //延时操作Qtimer，实现进入场景操作
        QTimer::singleShot(500,this,[=](){
            mapScene->show();
            this->hide();
        });
    });

}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pixbg);
    painter.drawPixmap(this->width()-pixyhb.width(),this->height()-pixyhb.height(),pixyhb.width(),pixyhb.height(),pixyhb);
    painter.drawPixmap((this->width()-pixtd.width())*0.5,0,pixtd.width(),pixtd.height(),pixtd);

}

MainWindow::~MainWindow()
{
    delete ui;
}


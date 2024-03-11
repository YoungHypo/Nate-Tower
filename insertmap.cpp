#include "insertmap.h"
#include "ui_insertmap.h"
#include "Common.h"
#include <string>
#include <QPainter>
#include <QMessageBox>
#include <QTimer>
#include <fstream>

InsertMap::InsertMap(QWidget *parent) :
    QWidget(parent),
    finishFlag(false),
    ui(new Ui::InsertMap)
{
    ui->setupUi(this);
    this->setFixedSize(1260,900);
    this->setWindowTitle("Tower Defence");
    this->setWindowIcon(QIcon(":/image/yellowdiamond4.png"));

    InitPixmap();

    InitMediaPlayer();

    //初始化按钮
    confirmBtn=new MyButton(":/image/right.png",0.18);
    confirmBtn->setParent(this);
    confirmBtn->move(this->width()*0.5-confirmBtn->width()*0.5,this->height()*0.45+confirmBtn->height());

    backBtn=new MyButton(":/image/backBtn.png",0.2);
    backBtn->setParent(this);
    backBtn->move(this->width()*0.5-backBtn->width()*0.5+12,this->height()*0.9-backBtn->height());

    connect(backBtn,&MyButton::clicked,this,[=](){
       //触发返回信号，timer延时
        backBtn->zoomdown();
        backBtn->zoomup();
        backPlayer->play();

        QTimer::singleShot(400,this,[=](){
            emit chooseBack();
        });
    });

    connect(confirmBtn,&MyButton::clicked,this,[=](){
        confirmBtn->zoomdown();
        confirmBtn->zoomup();
        //导入操作，并且返回
        std::string fileStr=ui->lineEdit->text().toStdString();
        if(fileStr.size()==0){
            errorPlayer->play();
            QMessageBox::critical(this,"错误","地图名称不能为空！");
        }
        else{
            if(!CheckFileNameLegitimacy(fileStr)){
                errorPlayer->play();
                QMessageBox::critical(this,"错误","地图名称不合法！");
            }
            else{
                //导入地图信息
                if(ImportMap(fileStr)){
                    succPlayer->play();
                    QMessageBox::information(this,"提示","成功导入地图！");
                    finishFlag=true;
                }
                else{
                    errorPlayer->play();
                    QMessageBox::critical(this,"错误","该地图不存在！");
                }
            }
        }
    });

}

void InsertMap::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pixbg);
    painter.drawPixmap(this->width()-pixyhb.width(),this->height()-pixyhb.height(),pixyhb.width(),pixyhb.height(),pixyhb);
    painter.drawPixmap(540,280,pixfileName);
}

bool InsertMap::ImportMap(std::string fileName)
{
    std::string fileStr="../project2/file/"+fileName+".txt";
    std::fstream fin(fileStr.c_str(),std::ios::in);
    if(!fin) return false;
    else{
        int t=-1;
        int curNum = -1, formerNum = -1;
        while(!fin.eof()){
            formerNum = curNum;
            fin >> curNum;
            if (curNum == -1) t++;
            else {
                if(formerNum!=curNum)
                    routePoint[t].push_back(curNum);
            }
        }
        return true;
    }
}

void InsertMap::InitPixmap()
{
    pixbg=QPixmap(":/image/mainbg.png");
    pixyhb=QPixmap(":/image/pixyhb.png");
    pixyhb=pixyhb.scaled(QSize(pixyhb.width()*0.6,pixyhb.height()*0.6),Qt::KeepAspectRatio);
    pixfileName=QPixmap(":/image/filename.png");
    pixfileName=pixfileName.scaled(QSize(pixfileName.width()*0.75,pixfileName.height()*0.75),Qt::KeepAspectRatio);
}

void InsertMap::InitMediaPlayer()
{
    backPlayer = new QMediaPlayer;
    backAudioOutput = new QAudioOutput;
    backPlayer->setAudioOutput(backAudioOutput);
    backPlayer->setSource(QUrl::fromLocalFile("../project2/media/messageReturn.mp3"));

    succPlayer = new QMediaPlayer;
    succAudioOutput = new QAudioOutput;
    succPlayer->setAudioOutput(succAudioOutput);
    succPlayer->setSource(QUrl::fromLocalFile("../project2/media/succ.mp3"));

    errorPlayer = new QMediaPlayer;
    errorAudioOutput = new QAudioOutput;
    errorPlayer->setAudioOutput(errorAudioOutput);
    errorPlayer->setSource(QUrl::fromLocalFile("../project2/media/error.mp3"));
}

InsertMap::~InsertMap()
{
    delete confirmBtn;
    delete backBtn;

    delete backPlayer;
    delete backAudioOutput;
    delete succPlayer;
    delete succAudioOutput;
    delete errorPlayer;
    delete errorAudioOutput;

    delete ui;
}

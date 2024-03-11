#include "custommap.h"
#include "ui_custommap.h"
#include "Common.h"
#include <QMouseEvent>
#include <QPainter>
#include <string>
#include <QMessageBox>
#include <QTimer>
#include <fstream>

CustomMap::CustomMap(QWidget *parent) :
    QWidget(parent),
    finishFlag(false),
    ui(new Ui::CustomMap),
    type(-1)
{
    ui->setupUi(this);
    this->setFixedSize(1260,900);
    this->setWindowTitle("Tower Defence");
    this->setWindowIcon(QIcon(":/image/yellowdiamond4.png"));

    //初始化工作
    InitPixmap();
    InitMediaPlayer();

    //初始化按钮
    pathBtn[0]=new MyButton(":/image/daoluyi.png",0.6);
    pathBtn[0]->setParent(this);
    pathBtn[0]->move(90-pathBtn[0]->width()*0.5,40);

    pathBtn[1]=new MyButton(":/image/daoluer.png",0.6);
    pathBtn[1]->setParent(this);
    pathBtn[1]->move(90-pathBtn[1]->width()*0.5,140);

    confirmBtn=new MyButton(":/image/right.png",0.18);
    confirmBtn->setParent(this);
    confirmBtn->move(90-confirmBtn->width()*0.5,this->height()*0.5+confirmBtn->height());

    backBtn=new MyButton(":/image/backBtn.png",0.2);
    backBtn->setParent(this);
    backBtn->move(90-backBtn->width()*0.5,this->height()-backBtn->height());

    //槽连接
    connect(pathBtn[0],&MyButton::clicked,this,[=](){
        pathBtn[0]->zoomdown();
        pathBtn[0]->zoomup();
        AudioPlay(buttonPlayer);
        type=0;
    });

    connect(pathBtn[1],&MyButton::clicked,this,[=](){
        pathBtn[1]->zoomdown();
        pathBtn[1]->zoomup();
        AudioPlay(buttonPlayer);
        type=1;
    });

    connect(confirmBtn,&MyButton::clicked,this,[=](){
        confirmBtn->zoomdown();
        confirmBtn->zoomup();
        //导出操作，并且返回
        if(routePoint[0].empty()&&routePoint[1].empty()){
            AudioPlay(errorPlayer);
            QMessageBox::critical(this,"错误","输入地图不能为空");
        }
        else {
            std::string fileStr=ui->lineEdit->text().toStdString();
            if(fileStr.size()==0){
                AudioPlay(errorPlayer);
                QMessageBox::critical(this,"错误","地图名称不能为空！");
            }
            else{
                if(!CheckFileNameLegitimacy(fileStr)){
                    AudioPlay(errorPlayer);
                    QMessageBox::critical(this,"错误","地图名称不合法！");
                }
                else{
                    //检测地图长度，最短为3
                    if(!CheckPathLength()){
                        AudioPlay(errorPlayer);
                        QMessageBox::critical(this,"错误","地图长度应大于3个单元格！");
                    }
                    else{//存储地图信息
                        AudioPlay(succPlayer);
                        StoreMap(fileStr);
                        QMessageBox::information(this,"提示","成功存储地图信息！");
                        finishFlag=true;
                    }
                }
            }
        }
    });

    connect(backBtn,&MyButton::clicked,this,[=](){
       //触发返回信号，timer延时
        backBtn->zoomdown();
        backBtn->zoomup();
        AudioPlay(backPlayer);

        QTimer::singleShot(400,this,[=](){
            emit chooseBack();
        });
    });

}

void CustomMap::InitMediaPlayer()
{
    buttonPlayer = new QMediaPlayer;
    buttonAudioOutput = new QAudioOutput;
    buttonPlayer->setAudioOutput(buttonAudioOutput);
    buttonPlayer->setSource(QUrl::fromLocalFile("../project2/media/buttonClick.mp3"));

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

    clickPlayer = new QMediaPlayer;
    clickAudioOutput = new QAudioOutput;
    clickPlayer->setAudioOutput(clickAudioOutput);
    clickPlayer->setSource(QUrl::fromLocalFile("../project2/media/click.mp3"));
}

void CustomMap::AudioPlay(QMediaPlayer* player){
    if(player->playbackState()==QMediaPlayer::PlayingState)
        player->stop();
    player->play();
}

void CustomMap::InitPixmap()
{
    pixmain=QPixmap(":/image/mainbg2.png");
    pixbg=QPixmap(":/image/background.png");
    pixpath[0]=QPixmap(":/image/groundpath1.png");
    pixpath[0]=pixpath[0].scaled(FixedSize,Qt::KeepAspectRatio);
    pixpath[1]=QPixmap(":/image/groundpath.png");
    pixpath[1]=pixpath[1].scaled(FixedSize,Qt::KeepAspectRatio);
    pixfileName=QPixmap(":/image/filename.png");
    pixfileName=pixfileName.scaled(QSize(pixfileName.width()*0.6,pixfileName.height()*0.6),Qt::KeepAspectRatio);
}

void CustomMap::StoreMap(std::string fileName){
    std::string fileStr="../project2/file/"+fileName+".txt";
    std::fstream fin(fileStr.c_str(),std::ios::out);
    if(!fin){
        qDebug()<<"文件打开失败";
        return;
    }
    StorePath(fin,0);
    StorePath(fin,1);
    fin.close();
}

void CustomMap::StorePath(std::fstream& fin,int t){
    fin<<-1<<std::endl;
    if(routePoint[t].size()!=0){
        for(int i=0;i<(int)routePoint[t].size();i++){
            fin<<routePoint[t][i]<<std::endl;
        }
    }
}

void CustomMap::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,180,this->height(),pixmain);
    painter.drawPixmap(0,0,this->width(),this->height(),pixbg);
    painter.drawPixmap(20,260,pixfileName);

    for (int i = 0; i < 120; i++){
        for(int j=0;j<2;j++){
            if (pointFlag[i].isRoute[j])
                painter.drawPixmap(GetPointPix(i),pixpath[j]);
        }
    }

    update();
}

void CustomMap::mousePressEvent(QMouseEvent * e)
{
    QString str = QString( "鼠标按下了 x = %1   y = %2" ).arg(e->pos().x()).arg(e->pos().y());
    qDebug()<<str;
    QPoint presspos=e->pos();
    int index=GetPointIndex(presspos);
    qDebug()<<index;

    if(pointFlag[index].isRoute[0]||pointFlag[index].isRoute[1]){
        AudioPlay(errorPlayer);
        return;
    }
    if(type!=-1) AddPath(index,type);
}

void CustomMap::AddPath(int index,int t)
{
    //空了直接加
    if(routePoint[t].empty()){
        if(!CheckIndexNeighbor(index,-1)) {
            AudioPlay(errorPlayer);
            return;
        }
        pointFlag[index].isRoute[t]=true;
        routePoint[t].push_back(index);
        AudioPlay(clickPlayer);
    }
    else{
        int lastIndex=routePoint[t].last();
        int ok=0;
        //同行或同列，才可被加入routePoint
        if(CheckIndexAdjacent(lastIndex,index,ok)){
            //任意两条路径不能紧邻，给炮台留有搭建的空间
            if(!CheckIndexNeighbor(index,lastIndex)){
                AudioPlay(errorPlayer);
                return;
            }
            //两个航点之间所有路径的is_route为true
            if(!GeneratePathBetweenIndex(lastIndex, index,ok,t)){
                AudioPlay(errorPlayer);
                return;
            }
            if(routePoint[t].size()>=2){
                int lastlastIndex=routePoint[t][routePoint[t].size()-2];
                //如果三个航点在一条线上，就将中间那个点删除
                if(CheckIndexInOneline(lastlastIndex,lastIndex,index)){
                    routePoint[t].pop_back();
                }
                routePoint[t].push_back(index);
                AudioPlay(clickPlayer);
            }
            else{
                pointFlag[index].isRoute[t]=true;
                routePoint[t].push_back(index);
                AudioPlay(clickPlayer);
            }
        }
        else{
            AudioPlay(errorPlayer);
        }
    }
}

bool CustomMap::CheckIndexNeighbor(int index,int lastIndex)
{
    bool leftFlag=false,rightFlag=false,upFlag=false,downFlag=false;
    int num=0;
    if(lastIndex>=0){
        if(index/12==lastIndex/12){//同行
            if(index<lastIndex){
                rightFlag=true;
            }
            else if(index>lastIndex){
                leftFlag=true;
            }
            num++;
        }
        else if(index%12==lastIndex%12){//同列
            if(index<lastIndex){
                downFlag=true;
            }
            else if(index>lastIndex){
                upFlag=true;
            }
            num++;
        }
    }

    if(!upFlag&&index-12>=0)
    {
        if(pointFlag[index-12].isRoute[0]||pointFlag[index-12].isRoute[1])
            num++;
    }
    if(!downFlag&&index+12<=119)
    {
        if(pointFlag[index+12].isRoute[0]||pointFlag[index+12].isRoute[1])
            num++;
    }
    if(!rightFlag&&index+1<=(index/12+1)*12-1)
    {
        if(pointFlag[index+1].isRoute[0]||pointFlag[index+1].isRoute[1])
            num++;
    }
    if(!leftFlag&&index-1>=(index/12)*12)
    {
        if(pointFlag[index-1].isRoute[0]||pointFlag[index-1].isRoute[1])
            num++;
    }
    if(num>=2) return false;
    else return true;
}

bool CustomMap::GeneratePathBetweenIndex(int firstIndex,int lastIndex,int ok,int t)
{
    int p = std::min(firstIndex, lastIndex);
    int q = std::max(firstIndex, lastIndex);
    int delta=0;
    if(ok==1) delta=1;
    else if(ok==2) delta=12;

    for (int k = p; k <= q; k+=delta){
        if(k==firstIndex) continue;
        if(pointFlag[k].isRoute[0]||pointFlag[k].isRoute[1]) return false;//生成路径重复了
    }
    for (int k = p; k <= q; k+=delta){
        pointFlag[k].isRoute[t]=true;
    }
    return true;
}

bool CustomMap::CheckIndexAdjacent(int firstIndex,int lastIndex,int& ok)
{
    if(firstIndex/12==lastIndex/12){//同行
        ok=1;
        return true;
    }
    else if(firstIndex%12==lastIndex%12){//同列
        ok=2;
        return true;
    }
    else return false;//既不同行也不同列
}

bool CustomMap::CheckIndexInOneline(int firstIndex,int secondIndex,int thirdIndex)
{
    //看看三个点是否同行
    if(firstIndex/12==secondIndex/12){
        if(thirdIndex/12==secondIndex/12)
            return true;
        else return false;
    }
    //看看三个点是否同列
    if(firstIndex%12==secondIndex%12){
        if(thirdIndex%12==secondIndex%12)
            return true;
        else return false;
    }
    return false;
}

bool CustomMap::CheckPathLength()
{
    for(int t=0;t<2;t++){
        if(routePoint[t].isEmpty()) continue;
        if(routePoint[t].size()==1) return false;
        int former=routePoint[t][0],length=1;
        for(int i=1;i<routePoint[t].size();i++){
            int latter=routePoint[t][i];
            if(latter%12==former%12){ //同列
                length+=abs(former-latter)/12;
            }
            else if(latter/12==former/12){//同行
                length+=abs(former-latter);
            }
            if(length>=3){
                break;
            }
            else{
                former=latter;
            }
        }
        if(length<3) return false;
    }
    return true;
}

CustomMap::~CustomMap()
{
    delete pathBtn[0];
    delete pathBtn[1];
    delete confirmBtn;
    delete backBtn;
    delete buttonPlayer;
    delete buttonAudioOutput;
    delete backPlayer;
    delete backAudioOutput;
    delete succPlayer;
    delete succAudioOutput;
    delete errorPlayer;
    delete errorAudioOutput;
    delete clickPlayer;
    delete clickAudioOutput;

    delete ui;
}

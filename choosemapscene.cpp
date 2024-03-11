#include "choosemapscene.h"
#include "custommap.h"
#include "insertmap.h"
#include "mybutton.h"
#include <QTimer>

chooseMapScene::chooseMapScene(QWidget *parent)
    : QMainWindow{parent},
    game(NULL),
    customMapScene(NULL),
    insertMapScene(NULL) //初始化呀初始化，调了一早上bug
{
    //设置场景大小及窗口图标
    this->setFixedSize(1260,900);
    this->setWindowTitle("Feature Selection");
    this->setWindowIcon(QIcon(":/image/yellowdiamond4.png"));

    //初始化图片
    pixbg=QPixmap(":/image/mainbg.png");
    pixyhb=QPixmap(":/image/pixyhb.png");
    pixyhb=pixyhb.scaled(QSize(pixyhb.width()*0.6,pixyhb.height()*0.6),Qt::KeepAspectRatio);

    InitRoutePoint();

    //初始化音效
    buttonPlayer = new QMediaPlayer;
    buttonAudioOutput = new QAudioOutput;
    buttonPlayer->setAudioOutput(buttonAudioOutput);
    buttonPlayer->setSource(QUrl::fromLocalFile("../project2/media/buttonClick.mp3"));

    //初始化按钮
    insertMapBtn=new MyButton(":/image/insert_map.png");
    insertMapBtn->setParent(this);
    insertMapBtn->move(this->width()*0.5-insertMapBtn->width()*0.5,this->height()*0.34);

    defaultMapBtn=new MyButton(":/image/start_game.png");
    defaultMapBtn->setParent(this);
    defaultMapBtn->move(this->width()*0.5-defaultMapBtn->width()*0.5,this->height()*0.6);

    customMapBtn=new MyButton(":/image/self_map.png");
    customMapBtn->setParent(this);
    customMapBtn->move(this->width()*0.5-customMapBtn->width()*0.5,customMapBtn->height());

    //槽连接
    connect(customMapBtn,&QPushButton::clicked,this,[=]{
        customMapBtn->zoomdown();
        customMapBtn->zoomup();
        AudioPlay(buttonPlayer);

        if(customMapScene==NULL) customMapScene=new CustomMap;

        QTimer::singleShot(300,this,[=](){
            customMapScene->show();
            customMapScene->setGeometry(this->geometry());
            this->hide();

            //监听返回信号
            connect(customMapScene,&CustomMap::chooseBack,this,[=](){
                this->show();
                customMapScene->hide();
                this->setGeometry(customMapScene->geometry());
                if(customMapScene->finishFlag){
                    if(customMapScene->routePoint[0].isEmpty()) routePoint[0].clear();
                    else routePoint[0]=customMapScene->routePoint[0];
                    if(customMapScene->routePoint[1].isEmpty()) routePoint[1].clear();
                    else routePoint[1]=customMapScene->routePoint[1];
                }
                delete customMapScene;
                customMapScene=NULL;
            });
        });

    });

    connect(insertMapBtn,&QPushButton::clicked,this,[=]{
        insertMapBtn->zoomdown();
        insertMapBtn->zoomup();
        AudioPlay(buttonPlayer);

        if(insertMapScene==NULL) insertMapScene=new InsertMap;

        QTimer::singleShot(300,this,[=](){
            insertMapScene->show();
            insertMapScene->setGeometry(this->geometry());
            this->hide();
            //监听返回信号
            connect(insertMapScene,&InsertMap::chooseBack,this,[=](){
                this->show();
                insertMapScene->hide();
                this->setGeometry(insertMapScene->geometry());
                if(insertMapScene->finishFlag){
                    if(insertMapScene->routePoint[0].isEmpty()) routePoint[0].clear();
                    else routePoint[0]=insertMapScene->routePoint[0];
                    if(insertMapScene->routePoint[1].isEmpty()) routePoint[1].clear();
                    else routePoint[1]=insertMapScene->routePoint[1];
                }
                delete insertMapScene;
                insertMapScene=NULL;
            });
        });

    });


    connect(defaultMapBtn,&QPushButton::clicked,this,[=]{
        defaultMapBtn->zoomdown();
        defaultMapBtn->zoomup();
        AudioPlay(buttonPlayer);

        if(game==NULL) game = new GameScene(routePoint[0],routePoint[1],this);

        QTimer::singleShot(300,this,[=](){
            game->show();
            game->setGeometry(this->geometry());
            emit enterGame();
            game->StartGame();

            this->hide();

            //监听返回信号
            connect(game,&GameScene::chooseBack,this,[=](){
                this->show();
                game->hide();
                this->setGeometry(game->geometry());
                delete game;
                game=NULL;
            });

        });
    });

}

void chooseMapScene::AudioPlay(QMediaPlayer* player){
    if(player->playbackState()==QMediaPlayer::PlayingState)
        player->stop();
    player->play();
}

void chooseMapScene::InitRoutePoint()
{
    int routeVec0[]={12,16,88,84};
    int len0=sizeof(routeVec0)/sizeof(int);
    for(int i=0;i<len0;i++) routePoint[0].push_back(routeVec0[i]);
    int routeVec1[]={23,18,90,95};
    int len1=sizeof(routeVec1)/sizeof(int);
    for(int i=0;i<len1;i++) routePoint[1].push_back(routeVec1[i]);
}

void chooseMapScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pixbg);
    painter.drawPixmap(this->width()-pixyhb.width(),this->height()-pixyhb.height(),pixyhb.width(),pixyhb.height(),pixyhb);
}

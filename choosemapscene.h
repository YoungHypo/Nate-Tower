#ifndef CHOOSEMAPSCENE_H
#define CHOOSEMAPSCENE_H

#include <QMainWindow>
#include <QVector>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "gamescene.h"
#include "custommap.h"
#include "insertmap.h"

class chooseMapScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit chooseMapScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

    GameScene * game;
    CustomMap * customMapScene;
    InsertMap * insertMapScene;

    //音效
    QMediaPlayer * buttonPlayer;
    QAudioOutput * buttonAudioOutput;

    //图片
    QPixmap pixbg;
    QPixmap pixyhb;

    //按钮
    MyButton * insertMapBtn;
    MyButton * defaultMapBtn;
    MyButton * customMapBtn;

signals:
    void enterGame();

private:
    QVector<int> routePoint[2];
    void InitRoutePoint();
    void AudioPlay(QMediaPlayer* player);
};

#endif // CHOOSEMAPSCENE_H

#ifndef CUSTOMMAP_H
#define CUSTOMMAP_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QWidget>
#include <string>
#include "mybutton.h"
#include "Common.h"

namespace Ui {
class CustomMap;
}

class CustomMap : public QWidget
{
    Q_OBJECT

public:
    explicit CustomMap(QWidget *parent = nullptr);
    ~CustomMap();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * e);

    /*检查地图的合法性*/
    bool CheckIndexAdjacent(int firstIndex,int lastIndex,int& ok);
    bool CheckIndexInOneline(int firstIndex,int secondIndex,int thirdIndex);
    bool CheckIndexNeighbor(int index,int lastIndex);
    bool GeneratePathBetweenIndex(int firstIndex,int lastIndex,int ok,int t);
    bool CheckPathLength();

    void AddPath(int index,int t);              //添加地图
    void StoreMap(std::string fileName);        //存储地图
    void StorePath(std::fstream& fin,int t);    //存储路径
    void AudioPlay(QMediaPlayer* player);       //播放音乐

    /*初始化*/
    void InitPixmap();
    void InitMediaPlayer();

    QVector<int> routePoint[2];
    bool finishFlag;    //完成地图存储的标志

private:
    Ui::CustomMap *ui;

    //按钮
    MyButton * pathBtn[2];
    MyButton * confirmBtn;
    MyButton * backBtn;

    //图片
    QPixmap pixbg;
    QPixmap pixpath[2];
    QPixmap pixfileName;
    QPixmap pixmain;

    //音效
    QMediaPlayer * buttonPlayer;
    QAudioOutput * buttonAudioOutput;
    QMediaPlayer * backPlayer;
    QAudioOutput * backAudioOutput;
    QMediaPlayer * succPlayer;
    QAudioOutput * succAudioOutput;
    QMediaPlayer * errorPlayer;
    QAudioOutput * errorAudioOutput;
    QMediaPlayer * clickPlayer;
    QAudioOutput * clickAudioOutput;

    PointFlags pointFlag[120];

    int type;

signals:
    void chooseBack();
};

#endif // CUSTOMMAP_H

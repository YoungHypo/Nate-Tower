#ifndef INSERTMAP_H
#define INSERTMAP_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <string>
#include "mybutton.h"

namespace Ui {
class InsertMap;
}

class InsertMap : public QWidget
{
    Q_OBJECT
public:
    explicit InsertMap(QWidget *parent = nullptr);
    ~InsertMap();
    void paintEvent(QPaintEvent *);
    bool ImportMap(std::string fileName);   //导入地图

    /*初始化*/
    void InitPixmap();
    void InitMediaPlayer();

    QVector<int> routePoint[2];
    bool finishFlag;    //完成导入的标志

private:
    Ui::InsertMap *ui;

    //按钮
    MyButton * confirmBtn;
    MyButton * backBtn;

    //图片
    QPixmap pixfileName;
    QPixmap pixbg;
    QPixmap pixyhb;

    //音效
    QMediaPlayer * backPlayer;
    QAudioOutput * backAudioOutput;
    QMediaPlayer * succPlayer;
    QAudioOutput * succAudioOutput;
    QMediaPlayer * errorPlayer;
    QAudioOutput * errorAudioOutput;

signals:
    void chooseBack();
};

#endif // INSERTMAP_H

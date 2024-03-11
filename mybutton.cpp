#include "mybutton.h"
#include <QPushButton>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QSize>
#include <QDebug>

MyButton::MyButton(QString normalImg,double pix_size):
    QPushButton(0),
    animationUp(NULL),
    animationDown(NULL)
{
    //设置按钮加载图片
    QPixmap pix;
    pix.load(normalImg);
    pix=pix.scaled(QSize(pix.width()*pix_size,pix.height()*pix_size),Qt::KeepAspectRatio);
    this->setFixedSize(pix.width(),pix.height());
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");//边框0像素
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}

void MyButton::zoomup()
{
    //设置动画
    animationUp=new QPropertyAnimation(this,"geometry");
    animationUp->setDuration(400);
    animationUp->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animationUp->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animationUp->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animationUp->start();
}

void MyButton::zoomdown()
{
    animationDown=new QPropertyAnimation(this,"geometry");
    animationDown->setDuration(400);
    animationDown->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animationDown->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animationDown->setEasingCurve(QEasingCurve::OutBounce);
    animationDown->start();
}

MyButton::~MyButton()
{
    delete animationUp;
    delete animationDown;
}

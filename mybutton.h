#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>


class MyButton : public QPushButton
{
    Q_OBJECT
public:
    MyButton(QString normalImg,double pix_size=1.0);
    ~MyButton();

    //弹起特效
    void zoomup();
    void zoomdown();

private:
    /*动画*/
    QPropertyAnimation * animationUp;
    QPropertyAnimation * animationDown;
};

#endif // MYBUTTON_H

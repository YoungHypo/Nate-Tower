#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *);

private:
    //图片
    QPixmap pixbg;
    QPixmap pixyhb;
    QPixmap pixtd;

    //音效
    QMediaPlayer * buttonPlayer;
    QAudioOutput * buttonAudioOutput;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

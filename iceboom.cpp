#include "boom.h"
#include "gamescene.h"
#include <QPoint>
#include <QTimerEvent>
#include <QTimer>

IceBoom::IceBoom(Enemy * enemy,GameScene * g,int t):
    Boom(enemy->getPos()+QPoint(0,10),t,g),
    endFlag(false),
    targetEnemy(enemy),
    frozenTime(5000)
{
    QTimer::singleShot(frozenTime,this,[=](){
        if(game->enemyList.indexOf(targetEnemy)!=-1) targetEnemy->EndFrozen();
    });
}

void IceBoom::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId()==eventId){
        if(index<=11){
            index++;
        }
        else{//判定是否动画结束
            if(!endFlag){//要么敌人死了，要么敌人逃脱了冰冻
                if(!targetEnemy->FrozenFlag()||game->enemyList.indexOf(targetEnemy)==-1){
                    endFlag=true;
                }
            }
            if(endFlag){
                index++;
                if(index>=pix.size()){
                    index=pix.size()-1;
                    game->DeleteBoom(this);
                }
            }
        }
    }
}

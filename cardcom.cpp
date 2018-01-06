#include "cardcom.h"
CardCom::CardCom()
{

}

void CardCom::init()
{
    char tmp[6] = {0x22,0x22,0x22,0x22,0x22,0x22};
    memcpy(m_secretKey,tmp,sizeof(tmp));
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(slotOperateCardQueue()));
    timer->start(200);
    m_queue.clear();
    m_queue.reserve(4096);
}

void CardCom::receive()
{
    char buf[1024] = {};

    int ret = this->readSerial(buf,sizeof(buf));
    for(int i =0;i<ret;i++)
    {
        m_queue.enqueue(buf[i]);
    }
}

/*
 *
 *
 */

void CardCom::slotOperateCardQueue()
{
    if(!m_queue.isEmpty())
    {

    }
}

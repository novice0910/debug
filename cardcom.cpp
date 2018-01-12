#include "cardcom.h"


RINGQ *queue = new RINGQ;
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
    queue->head = 0;
    queue->tail = 0;
    queue->size = QUEUE_MAX;
    memset(queue->space,0,queue->size);
}

void CardCom::receive()
{
    char buf[1024] = {};
    if(ringq_is_full(queue))
    {
        qDebug()<<"this ring has overload,plead enlarg!";
    }
    else
    {
        int ret = this->readSerial(buf,sizeof(buf));
        for(int i =0;i<ret;i++)
        {
            queue->space[queue->tail] = buf[i];
            queue->tail = (queue->tail + 1) % queue->size;
        }
    }
}

/*1,get the head
 *
 *
 */

void CardCom::slotOperateCardQueue()
{
    int msgLength = 0;
    char buf[1024] = {0};
    if(!ringq_is_empty(queue))
    {
        if(queue->space[queue->head] != CARD_STX)      //is not the start bit
        {
            queue->head = (queue->head + 1) % queue->size;
        }
        else{
            int surplus = queue->tail - queue->head;    //the remainning message length
            if(surplus >= 3)                            //can get the message length
            {
                //the message overall length
                msgLength = (((queue->space[queue->head + 1] & 0xff ) << 8)|
                        (queue->space[queue->head + 2]))+ 5;
                if(surplus >= msgLength)        //have a whole message
                {
                    memcpy(buf,&queue->space[queue->head],msgLength);
                    queue->head = (queue->head + msgLength) % queue->size;
                    //deal the message
                    dealMsg(buf,msgLength);
                }
            }
        }
    }
    else{
        qDebug()<<"the queue is empty!";
    }
}

void CardCom::dealMsg(char *data, int len)
{
    unsigned char tmp = data[4];
    if(data[len-2] != xorCheck(&data[3],len - 5))
    {
        char buf[1] = {0x15};           //feedback false
        writeSerial(buf,1);
    }
    if(data[3] == 0x30)
    {
    }
    else if(data[3] == 0x31)
    {
        switch (tmp) {
        case 0x11:
            feedbackCardReaderInfo_11();
            break;
        case 0x13:
            qDebug()<<"open the buzzer!";
            feedback_31_13();
            break;
        case 0x14:
            feedbackSuccess();
            break;
        case 0x90:
            feedbackSuccess();
            break;
        case 0x91:
            feedbackSuccess();
            break;
        default:
            break;
        }
    }
    else if(data[3] == 0x32)
    {
        switch (tmp){
        case 0x24:
            qDebug()<<"feed card serial number!!";
            feedbackCardSerialNumber();
            break;
        default:
            break;
        }
    }
    else if(data[3] == 0x02)
    {
        switch (data[4]) {
        case 0x46:
            qDebug()<<"affirm card success!!";
            feedbackSuccess();
            break;
        case 0x47:
            break;
        default:
            break;
        }
    }
    else{

    }
}

void CardCom::feedbackCardReaderInfo_11()
{
    char sendData[6];
    sendData[0] = CARD_STX;
    sendData[1] = 0x00;
    sendData[2] = 0x02;
    sendData[3] = 0x00;
    sendData[4] = 0x00;
    sendData[5] = xorCheck(&sendData[3],0x02);
    sendData[6] = CARD_ETX;
    this->writeData(sendData,sizeof(sendData));
}

void CardCom::feedback_31_13()
{
    char sendData[7];
    sendData[0] = CARD_STX;
    sendData[1] = 0x00;
    sendData[2] = 0x02;
    sendData[3] = 0x00;
    sendData[4] = 0x00;
    sendData[5] = xorCheck(&sendData[3],0x02);
    sendData[6] = CARD_ETX;
    this->writeData(sendData,sizeof(sendData));
}

void CardCom::feedbackSuccess()
{
    char sendData[7];
    sendData[0] = CARD_STX;
    sendData[1] = 0x00;
    sendData[2] = 0x02;
    sendData[3] = 0x00;
    sendData[4] = 0x00;
    sendData[5] = xorCheck(&sendData[3],0x02);
    sendData[6] = CARD_ETX;
    this->writeData(sendData,sizeof(sendData));
}

void CardCom::feedbackCardSerialNumber()
{
    quint8 sendData[] =
    {0x02,0x00,0x08,0x00,0x00,0x1A,0x04,0xE4,
     0x85,0xAD,0xB6,0x64,0x03};
    this->writeData((char *)sendData,sizeof(sendData));
}

void CardCom::feedbackBlockInfo(int block)
{
    char buf[23] = {0x02,0x00,0x12,0x00,0x00};
//                  0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
//                  0x88 ,0x99 ,0xAA,0xBB,0xCC,0xDD,0xEE ,
//                  0xFF,0x00,0x03};

    switch (block) {
    case BLOCK_CARD_KEYA:

        break;
    case BLOCK_CARD_NUMBER:
    {
        buf[4] = INT_2_BCD(0x05);
        buf[5] = INT_2_BCD(0x32);
        buf[6] = INT_2_BCD(0x12);
        buf[7] = INT_2_BCD(0x34);
        buf[8] = INT_2_BCD(0x56);
        buf[9] = INT_2_BCD(0x78);
        buf[21] = xorCheck(buf,sizeof(buf));
        buf[22] = CARD_ETX;
    }
        break;
    case BLOCK_CARD_MONEY:
    {
        buf[4] = INT_2_BCD(0x12);
        buf[5] = INT_2_BCD(0x34);
        buf[6] = INT_2_BCD(0x56);
        buf[7] = INT_2_BCD(0x78);
        buf[21] = xorCheck(buf,sizeof(buf));
        buf[22] = CARD_ETX;
    }
        break;
    case BLOCK_CARD_LOCK:
    {
        buf[4] = INT_2_BCD(0x01);
        buf[21] = xorCheck(buf,sizeof(buf));
        buf[22] = CARD_ETX;
    }
        break;
    default:
        break;
    }
    this->writeSerial(buf,sizeof(buf));
}

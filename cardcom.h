/**
 * 模拟ZLG600A-DCP 读卡器
 *
*/
#ifndef CARDCOM_H
#define CARDCOM_H
#include <QTimer>
#include <QDebug>
#include <QList>
#include <QQueue>
#include "serial.h"
#include "data.h"

#define CARD_OVERTIME   5
#define CARD_STX 0x02  //起始字节
#define CARD_ETX 0x03   //结束字节
#define CARD_NAK 0x15   //接收错误数据
#define CARD_VALID_DATA 5 //有效数据的起始位
#define KEY_A 0x60
#define KEY_B 0x61
//块号
#define BLOCK_CARD_KEYA     0X07
#define BLOCK_CARD_NUMBER   0x04
#define BLOCK_CARD_MONEY    0X05
#define BLOCK_CARD_LOCK     0x06

#define QUEUE_MAX 4096
#define ringq_is_empty(q) (q->head == q->tail)
#define ringq_is_full(q) ((q->head == (q->tail + 1)%q->size))

typedef struct ringq{
    int head; /* 头部，出队列方向*/
    int tail; /* 尾部，入队列方向*/
    int tag ;
    int size ; /* 队列总尺寸 */
    char space[QUEUE_MAX]; /* 队列空间 */
}RINGQ;

extern RINGQ *queue;

class CardCom : public Serial
{
    Q_OBJECT
public:
    CardCom();
public slots:
    void init();
    void receive();
signals:
private slots:
    void slotOperateCardQueue();
private:
    char m_secretKey[6];
    QTimer * timer;
    CARD_INFO m_cardInfo;                       //读取的卡信息
    void dealMsg(char *data , int len);
    void feedbackCardReaderInfo_11();           //feedback card reader information
    void feedback_31_13();                      //buzzer
    void feedbackSuccess();                     //feadback success
    void feedbackCardSerialNumber();            //feedback card serial serial number
    void feedbackBlockInfo(int block);          //feedback block information
};
#endif // CARDCOM_H

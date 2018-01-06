/**  *ZLG600A-DCP 读卡器
*开机： 寻卡，读信息，  锁卡，发送信息
*停机： 寻卡，读信息   发送信息
*解卡： 寻卡，读信息   发送信息
*结算： 寻卡读信息，   写余额，解锁，发送信息
*/
#ifndef CARDCOM_H
#define CARDCOM_H
#include <QTimer>
#include <QDebug>
#include <QQueue>

#include "serial.h"
#include "data.h"

#define CARD_OVERTIME   5
#define CARD_STX 0x02  //起始字节
#define CARD_ETX 0x03   //结束字节
#define CARD_NAK 0x15   //接收错误数据
#define CARD_VALID_DATA 5 //有效数据的起始位
#define KEY_A 0x60
#define KEY_B 0x61 //块号
#define BLOCK_CARD_KEYA     0X07
#define BLOCK_CARD_NUMBER   0x04
#define BLOCK_CARD_MONEY    0X05
#define BLOCK_CARD_LOCK     0x06
/**
 * 0,STANDBY    无操作
* 1,FIND       寻卡
 * 2,VERFIFY    验证卡
 * 3,READ_NUM   读卡号
 * 4,READ_MONEY 读余额
 * 5,READ_LOCK  读锁卡次数
 * 6,WRITE_LOCK 写灰锁次数
 * 7,WRITE_UNLOCK   解锁卡
 * 8,WRITE_ACCOUNT_MONEY  扣除费写余额
 * 9,WRITE_UNLOCK_MONEY  解锁卡时扣除或返还金钱
* 10,WRITE_WITHHOLD_MONEY 扣除预扣金额  */
enum CARD_STAGE{
    STANDBY,FIND,VERIFY,READ_NUM,READ_MONEY,READ_LOCK,WRITE_LOCK,
    WRITE_UNLOCK,WRITE_ACCOUNT_MONEY,WRITE_UNLOCK_MONEY,WRITE_WITHHOLD_MONEY
};
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
    CARD_INFO m_cardInfo;                           //读取的卡信息

    QQueue<char> m_queue;
};
#endif // CARDCOM_H

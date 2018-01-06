#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <stdio.h>
#include <QMap>

#define CAL_WEEK(y,m,d)  (d+ 2*m + 3*(m +1)/5 + y + y*4 - y/100 + y/400)%7;
#define INT_2_BCD(data) (((data)/10*16) + ((data)%10))
#define BCD_2_INT(data) (((data)/16*10) + ((data)%16))

#define INTERFACE 0     //枪接口标识
#define CONTROL_SWITCH 0x02
#define MAJOR_VERSION 0x01
#define MINOR_VERSION 0x10
#define ONE_FRAME_LEN 8
#define UI_ERROR_INDEX 3
#define RUN_PATH QCoreApplication::applicationDirPath()
#define TCU_STOP_REASON_NORMAL 0x01
#define TCU_STOP_REASON_EKM_COM_FAULT 0x03

#define CARD_MAX_LOCK_NUM 6

//输入金额，电量，时长 门限值
#define MAX_INPUT_MONEY 999     //999元
#define MIN_INPUT_MONEY 0
#define MAX_INPUT_KWH   999     //999kwh
#define MIN_INPUT_KWH   0
#define MAX_INPUT_TIME  999      //999s
#define MIN_INPUT_TIME  0

typedef QList<int> INT_LIST;
typedef QMap <int ,bool> MAP_INT_BOOL;

inline int xorCheck(char *data,int len)
{
    int BCC = data[0];
    for(int i = 1;i<len;i++)
    {
        BCC ^= data[i];
    }

    return BCC;
}
inline void voicePrompt(const char *str)
{
    char tmp[50] = {0};
    sprintf(tmp,"mplayer /opt/voice/%s&",str);
    system(tmp);
}

/**
 *说明： 家界面从后面加不然出问题
 *界面说明：
 *0,主界面： INDEX_STANDBY
 *1,无计费启动界面: INDEX_FREE_START 2,正在启动： INDEX_STARTING_CHAGER
 *3,充电中：INDEX_CHARGING,
 *4,正常充电结束： INDEX_NORMAL_END, 有结算按键
 *5,点击充电后启动或者充电失败故障显示： INDEX_FAULT_STOP, 有结算按键
 *6,待机出现的故障页： INDEX_STANDBY_FAULT
 *7,充电停止中：  INDEX_STOPING
 *8,等待刷卡：   INDEX_WAIT_SWIP_CARD
 *9,显示卡信息： INDEX_SHOW_CARD_INFO
 *10,完成结算： INDEX_SHOW_ACCOUNT_INFO //结算完成，提示拔枪结束
 *11,请连接充电枪提示： INDEX_CONNECT_GUN
 *12,选择充电方式界面 :INDEX_SELECT_CHAEGER_WAY
 *13,按金额充输入充电金额： INDEX_INPUT_CHARGER_MONEY
 *14,按时间充输入充电时长： INDEX_INPUT_CHARGER_TIME
 *15,按电量充输入充电电量： INDEX_INPUT_CHARGER_KWH
 *16,卡解锁中请勿拿开卡片： INDEX_UNLOCKING_CARD
 *17,提示没有卡解锁信息： INDEX_NO_UNLOCK_CARD_INFO
 *18,显示卡解锁信息: INDEX_SHOW_UNLOCK_CARD_INFO
 *19,刷卡结算提示页: INDEX_ACCOUNTING_CARD
 */
enum UI_INDEX{
    INDEX_STANDBY,INDEX_FREE_START,INDEX_STARTING,INDEX_CHARGING,INDEX_NORMAL_STOP,
    INDEX_FAULT_STOP,INDEX_STANDBY_FAULT,INDEX_STOPING,INDEX_WAIT_SWIP_CARD,
    INDEX_SHOW_CARD_INFO,INDEX_SHOW_ACCOUNT_INFO,INDEX_CONNECT_GUN,INDEX_SELECT_CHAEGER_WAY,
    INDEX_INPUT_CHARGER_MONEY,INDEX_INPUT_CHARGER_TIME,INDEX_INPUT_CHARGER_KWH,
    INDEX_UNLOCKING_CARD,INDEX_NO_UNLOCK_CARD_INFO,INDEX_SHOW_UNLOCK_CARD_INFO,
    INDEX_ACCOUNTING_CARD
};

/**
 * 待机阶段的功能需求 非充电流程
 * ManageBeforeCharger
 * 0,待机状态 UI_NONE
 * 1,查询卡 STANDBY_REQUEST_INQUIRE_CARD
 */
enum STANDYBY_REQUEST{
    STANDBY_REQUEST_NONE,STANDBY_REQUEST_INQUIRE_CARD,STANDBY_UNLOCK_CARD
};

/**
 *  充电桩充电阶段
 *  0,待机，CHARGER_STAGE_STANDBY
 *  1,开始启动，CHARGER_STAGE_START
 *  2,充电启动中，CHARGER_STAGE_STARTING
 *  3,开始停机 CHARGER_STAGE_NORMAL_STOP
 *  4,停机中，请勿拔枪 正常停机才有 CHARGER_STAGE_STOPING
 *  5,等待结算 CHARGER_STAGE_WAIT_ACCOUNT
 *  6,结算完成， CHARGER_STAGE_FINISH_ACCOUNG
 *  7,故障停机，异常启动，启动异常 CHARGER_STAGE_FAULT_STOP
 *  8,充电中 CHARGER_STAGE_CHARGING
 *  9,待机故障 CHARGER_STAGE_STANDBY_FAULT
 */
enum CHARGER_STAGE{
    CHARGER_STAGE_STANDBY,CHARGER_STAGE_START,CHARGER_STAGE_STARTING,CHARGER_STAGE_NORMAL_STOP,
    CHARGER_STAGE_STOPING,CHARGER_STAGE_WAIT_ACCOUNT,CHARGER_STAGE_FINISH_ACCOUNG,
    CHARGER_STAGE_FAULT_STOP,CHARGER_STAGE_CHARGING,CHARGER_STAGE_STANDBY_FAULT
};

/**
 *卡相关的操作指令
 *0 无操作         CARD_NONE
 *1 寻卡          CARD_FIND
 *2 锁卡          CARD_LOCK
 *3 解卡          CARD_UNLOCK
 *4 启动充电       CARD_START
 *5 刷卡停止充电    CARD_STOP
 *6 刷卡结算       CARD_ACCOUNT
 *7 预扣费，锁卡    CARD_WITHHOLD_LOCK
 */
enum OPERATER_CARD{
    CARD_NONE,CARD_FIND,CARD_LOCK,CARD_UNLOCK,CARD_START,CARD_STOP,
    CARD_ACCOUNT,CARD_INQUIRE,CARD_WITHHOLD_LOCK
};

/**
 *支付方式选择
 *0 无需操作           PAY_NONE
 *1 卡支付             PAY_ONLINE_CARD
 *2 帐号支付           PAY_ACOUNT
 *3 二维码             PAY_RENCODE
 *4 无需支付           PAY_FREE
 *5 离线卡             PAY_CARD
 */
enum PAY_WAY{
    PAY_NONE,PAY_ONLINE_CARD,PAY_ACOUNT,PAY_RENCODE,PAY_FREE,PAY_CARD
};

/**
 *充电方式选择
 *0 无需操作            CHARGER_NONE
 *1 按时间             CHARGER_TIME
 *2 按电量             CHARGER_KWH
 *3 按金额             CHARGER_MONEY
 *4 自动充满           CHARGER_AUTO
 */
enum CHARGER_WAY{
    CHARGER_NONE,CHARGER_TIME,CHARGER_KWH,CHARGER_MONEY,CHARGER_AUTO
};

struct CHARGER_SET_INFO{
    PAY_WAY pay;            //支付方式
    CHARGER_WAY chargerWay; //充电方式
    quint64 limitValue;     //秒，元(×100)，Kwh(x100)
};

/**
 *计费状态 准备，开始，结束,结账
 *BILLING_PREPARE 准备阶段清空计费
 *BILLING_START 开始计费
 *BILLING_STOP 结束计费
 *BILLING_ACCOUNT 完成结算
 */
enum BILLING_STATUS{
    BILLING_PREPARE,BILLING_START,BILLING_STOP,BILLING_ACCOUNT
};


//TCU下发给KZQ停机原因
enum TCU_STOP_REASON{
    TCU_STOP_NONE,TCU_STOP_NORMAL = 1,TCU_STOP_SELF_FAULT,TCU_STOP_KZQ_FAULT
};

//TCU检测的故障 0x01 与控制器通信故障,0x02 读卡器通信故障,0x03 电表通信故障,
enum TCU_CHECK_FAULT{
    TCU_KZQ_COM_FALUT = 0x01,TCU_CHECK_FAULT_CARD,TCU_CHECK_FAULT_EKM
};

//TCU 故障原因
typedef struct
{
    bool onOff;     //true,发生 false,消除
    TCU_CHECK_FAULT reason;
} TCU_FAULT_REASON;
/**
 *TCU 显示的自身错误提示框
 *0，充电卡解锁失败 MSG_UNLOCK_ERROR
 *1,请输入合法的充电电量 MSG_INPUT_KWH_ERROR
 *2,请输入合法的充电金额 MSG_INPUT_MONEY_ERROR
 *3,请输入合法的充电时长 MSG_INPUT_TIME_ERROR
 *4,帐号余额不足       MSG_MONEY_SHORT_ERROR
 *5,卡已灰锁           MSG_CARD_LOCKED_ERROR
 *6,卡预扣费失败        MSG_CARD_WITHHOLD_LOCK_ERROR
 *7,停机使用的非同一种卡 MSG_STOP_USE_DIF_CARD_ERROR
 *8,结算使用的非同一种卡 MSG_ACCOUNT_USE_DIF_CARD_ERROR
 *9,该卡存在未结算记录  MSG_CARD_EXIT_NULOCK_ERROR
 *10,读取卡片超时      MSG_CARD_FIND_OVERTIME_ERROR
 *11,结算失败         MSG_CARD_ACCOUNT_ERROR
 */
enum MSG_ERROR_SHOW{
    MSG_UNLOCK_ERROR,MSG_INPUT_KWH_ERROR,MSG_INPUT_MONEY_ERROR,MSG_INPUT_TIME_ERROR,
    MSG_MONEY_SHORT_ERROR,MSG_CARD_LOCKED_ERROR,MSG_CARD_WITHHOLD_LOCK_ERROR,
    MSG_STOP_USE_DIF_CARD_ERROR,MSG_ACCOUNT_USE_DIF_CARD_ERROR,MSG_CARD_EXIT_NULOCK_ERROR,
    MSG_CARD_FIND_OVERTIME_ERROR,MSG_CARD_ACCOUNT_ERROR
};

//枪连接状态 GUN_CONNECT_STATUS
enum GUN_CONNECT_STATUS{
    GUN_UNCONNECT,GUN_CONNECT
};

//充电信息包括 充电时长，充电费用，充电电量
typedef struct{
    quint64 chargerTime;    //多少秒
    QString time;           //--：--:--格式
    quint64 withholdMoney;  //预扣金额 放大100倍
    quint64 totalCost;      //总费用 放大100倍 12.22元 -> 1222 实际花销
    quint64 returnMoney;    //返还金额 放大100倍
    quint64 deductionMoney; //补扣金额 放大100倍 除去预扣费再扣除的金额(totalCost - withholdMoney)
    quint64 totalKwh;       //总电量 放大100倍 12.22kwh -> 1222
}COST_INFO;

//充电卡信息
typedef struct {
    char serialNum[4];      //卡序列号
    QString number;         //卡号
    quint32 money;          //剩余金额 放大100倍
    quint8 lockNum;         //已灰锁次数
    quint8 unlockNum;       //未灰锁次数
    quint8 erroInfo;        //错误信息
}CARD_INFO;

//卡片解锁的总信息
typedef struct{
    QString number;
    quint64 returnMoney;    //返还金额
    quint64 deductionMoney; //补扣金额
    quint8 unlockNum;       //须解锁次数
}CARD_UNLOCK_INFO;

//卡预扣费 信息
typedef struct{
    QString number;
    quint64 withholdMoney;  //预扣金额
}CARD_WITHHOLD_LOCK_INFO;

//支付方式的共有信息 用于计费，扣费，插入数据库
struct USER_COMMENT_INFO{
    QString account;        //帐号
    quint64 money;          //帐号余额  *100
    quint64 withholdMoney;  //预扣金额  *100
};

//结算信息
struct ACCOUNT_INFO{
    USER_COMMENT_INFO userInfo;
    COST_INFO cost;
};

//电表信息
typedef struct{
    quint64 eng;//电能
    bool comFault;//状态 1通信故障，0 正常。
}EKM_INFO;

//数据库信息
struct FAULT_RECORD{
    QString time;
    int erroCode;
};

struct CHARGER_RECORD{
    COST_INFO cost;
    QString account;
    QString startTime;
    QString endTime;
//    quint64 cost;       //(元 *100）
//    quint64 kwh;        //kwh *100
//    quint64 duration;   //充电时长(s)
    quint8 status;        //结账 1, 没结账0
};

typedef QList< FAULT_RECORD > FAULT_RECORD_LIST;
typedef QList< CHARGER_RECORD > CHARGER_RECORD_LIST;

//j1939扩展帧仲裁场定义
/*	D28 D27 D26 D25 D24 D23 D22 D21
 * 	D20 D19 D18 SRR IDE D17 D16 D15
 *  D14 D13 D12 D11 D10 D09 D08 D07
 *  D06 D05 D04 D03 D02 D01 D00 RTR
 *
 *  优先权P：D28 D27 D26
 *  保留位R：D25
 *  数据页DP：D24
 *  报文类型PF：D23~D16
 *  目标地址PS：D15~D08
 *  源地址SA：D07~D00
*/
#define R				0		//保留位R
#define DP				0		//数据页DP
#define ChargingUnit	0x8A	//计费单元地址
#define Controller		0xF6	//控制器地址
#define DATA_LEN		0X08	//数据长度

#define CommandFrame	0x01	//命令帧
#define StatusFrame		0x02	//状态帧
#define	DataFrame		0x03	//数据帧
#define HeartFrame		0x04	//心跳帧
#define	ErrorFrame		0x05	//错误帧

#define PF_StartCharging			0x01	//启动充电（计费单元->控制器）
#define PF_StartChargingAck			0x02	//启动充电应答（控制器->计费单元）

#define PF_StopCharging				0x03	//停止充电（计费单元->控制器）
#define PF_StopChargingAck			0x04	//应答（控制器->计费单元）

#define PF_SetTime					0x05	//下发对时（计费单元->控制器）
#define PF_SetTimeAck				0x06	//对时应答（控制器->计费单元）

#define PF_CheckVersion				0x07	//下发版本校验（计费单元->控制器）
#define PF_CheckVersionAck			0x08	//版本校验应答（控制器->计费单元）

#define PF_SetPara					0x09	//下发充电桩参数信息（计费单元->控制器）
#define PF_SetParaAck				0x0A	//充电桩参数信息确认（控制器->计费单元）

#define PF_ServiceControl      	 	0x0B    //充电服务启停控制（计费单元->控制器）
#define PF_ServiceControlAck   	 	0x0C    //充电服务启停控制反馈（控制器->计费单元）


#define PF_LockControl				0x0D	//电子锁控制（计费单元->控制器）PF_LockControl
#define PF_LockControlAck			0x0E	//电子锁控制反馈（控制器->计费单元）PF_LockControlAck

#define PF_PowerControl				0x0F	//功率调节控制（计费单元->控制器）PF_PowerControl
#define PF_PowerControlAck			0x10	//功率调节反馈（控制器->计费单元）PF_PowerControlAck

#define PF_StartFinish				0x11	//启动完成状态信息（控制器->计费单元）
#define PF_StartFinishAck			0x12	//启动完成状态信息应答（计费单元->控制器）

#define PF_StopFinish				0x13	//停止充电完成状态（控制器->计费单元）
#define PF_StopFinishAck			0x14	//停止充电完成状态应答（计费单元->控制器）

#define PF_Telemetry_20				0x20	//直流遥测（控制器->计费单元）
#define PF_Telemetry_21				0x21	//直流遥测（计费单元->控制器）
#define PF_Telesignal_22			0x22	//直流遥信（控制器->计费单元）
#define PF_Telesignal_23			0x23	//直流遥信（控制器->计费单元）

#define PF_Telemetry_30				0x30	//交流遥测（控制器->计费单元）
#define PF_Telemetry_31				0x31	//交流遥测（计费单元->控制器）
#define PF_Telesignal_32			0x32	//交流遥信（控制器->计费单元）
#define PF_Telesignal_33			0x33	//交流遥信

#define PF_Heartbeat_40				0x40	//心跳（计费单元->控制器）
#define PF_Heartbeat_41				0x41	//心跳（控制器->计费单元）

#define PF_ErrFrame_51				0x51	//错误帧（计费单元->控制器）
#define PF_ErrFrame_52				0x52	//错误帧（控制器->计费单元）

//故障代码表
#define KZQ_COM_FAILED 0x01
#define EKM_COM_FAILE 3                 //电表通信超时
#define BMS_COM_FAILE 10                //BMS通信超时
#define DC_OUTPUT_OVER_V_ALARM 11       //直流母线输出过压告警
#define DC_OUTPUT_UNDER_V_ALARM 12      //直流母线输出欠压告警
//#define BATTERY_OUTPUT_OVERCURRENT 13 //蓄电池充电过流告警
//#define BATTERY_OVER_TEM 14           //蓄电池模块采样点过温
#define RESERVED
#define SCRAM_BUTTON_FAULT 16           //急停开关故障
#define INSULATION_MONITOR_FAULT 17     //绝缘监测故障
#define BATTERY_REVERSE_FAULT 18        //电池反接故障
#define ARRESTER_FAULT 19               //避雷器故障
#define CHARGING_GUN_NOPLACE 20         //充电枪未归位
#define PILE_OVER_TEMPERATURE_FAULT 21  //充电桩过温故障
#define SMOKE_ALARM 22                  //烟雾告警
#define INPUT_OVERVOLTAGE_ALARM 23      //输入过压告警
#define INPUT_UNDERVOLTAGE_ALARM 24     //输入欠压告警
#define CHARGEMODULE_FAULT 25           //充电模块故障

#define MODULE_FAN_FAULT 27             //充电模块风扇故障
#define MODULE_OVER_TEMPERATURE 28      //充电模块过温告警
#define MODULE_INPUT_ALARM 29           //模块交流输入告警
#define MODULE_OUTPUT_SHORT 30          //模块输出短路故障
#define MODULE_OUTPUT_OVERCURRENT 31    //充电模块直流输出过流告警
#define	MODULE_OUTPUT_OVERVOLTATE 32    //充电模块直流输出过压告警
#define MODULE_OUTPUT_UNDERVOLTAGE 33   //充电模块直流输出欠压告警
#define MODULE_INPUT_OVERVOLTAGE 34     //充电模块交流输入过压告警
#define MODULE_INPUT_UNDERVOLTAGE 35    //充电模块交流输入欠压告警
#define MODULE_INPUT_DEFAULTPHASE 36    //充电模块交流输入缺相告警
#define MODULE_COMM_FAULT 37            //充电模块通信告警
#define GUIDANCE_FAULT 38               //充电中车辆控制导引告警
#define AC_BREAKER_FAULT 39             //交流断路器故障
#define ACBUS_OUTPUT_OVERCURRENT 40      //直流母线输出过流告警
#define DCBUS_OUTPUT_FUSE 41            //直流母线输出熔断器故障
#define DCBUS_OUTPUT_CONTACTOR 42       //直流母线输出接触器故障
#define CHARGE_PORT_LOCK_FAULT 43       //充电接口电子锁故障
#define CHARGE_FAN_FAULT 44             //充电机风扇故障
#define CHARGEGUN_OVER_TEMPERATURE 45   //充电枪过温故障
#define TCU_OTHER_FAULT 46              //
#define PILE_OHTER_FAULT 47             //
#define ENTRA_GUARD_FAULT 48            //门禁故障
#define DC_OUTPUT_CONTACTOR_SYNECHIA_FAULT 49       //直流输出接触器粘链故障
#define INSULATION_MONITOR_ALARM 50                //绝缘监测故障
#define VOLTAGE_RELEASE_CIRCUIT_FAULT 51           //泻放回路告警
#define PILE_OVER_TEMPERATURE_ALARM 52              //充电桩过温告警
#define CHARGEGUN_OVER_TEMPERATURE_ALARM 53         //充电枪过温告警
#define OTHER_FAULT 54
#define AC_INPUT_CONTACTOR_MALOPERATION_FAULT 55    //交流输入接触器据动/误动故障
#define AC_INPUT_CONTACTOR_SYNECHIA_FAULT 56        //交流输入接触器粘链故障
#define APS_FAULT 57                                //辅助电源故障
#define MUTIPLE_CONTACTOR_MALOPERATION_FAULT 58     //并联接触器据动/误动故障
#define MUTIPLE_CONTACTOR_SYNECHIA_FAULT 59         //并联接触器粘链故障


//#ifdef Debug
//quint8	startcharge_debug;				//启动应答帧
//quint8	finishstartcharge_debug;		//启动完成帧
//quint8	stopcharge_debug;				//停止应答帧
//quint8 	finishstopcharge_debug;			//停止完成帧
//#endif


#define RecvFrameTypeNum 11
#define commandFrameTypeNum 8
#define statusFrameTypeNum 2
#define FrameTypeNum 32   //32   27

//#ifdef DCCHARGING
//#define CONTROLSWITCH	0x02//0x01
//#endif

//#ifdef ACCHARGING
//#define CONTROLSWITCH 	0x02
//#endif

//#ifdef	ONEGUN
//#define GunNum 1//一桩一枪
//#define INTERFACE 0	//单枪充电接口编号为0
//#endif

//#ifdef	TWOGUN
//#define GunNum 2//一桩双枪
//#define INTERFACE 1	//单枪充电接口编号为0
//#endif

#pragma pack(1)
typedef struct
{
    quint8 sendPF;//发送报文类型
    quint8 recvPF;//接收报文类型
}   CommandFrameInfo;//命令帧
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 recvPF;//接收报文类型
    quint8 sendPF;//发送报文类型
}StatusFrameInfo;//状态帧
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 frameType;//帧类型
    quint8 type;//报文类型0x01~0x52 function number fc
    quint16 pgn;//PGN
    quint8 priority;//优先级
//	quint8 length;//数据长度
    quint16 period;//周期ms
    quint8 overTime;//超时时间
    quint8 srcAddr;//源地址
    quint8 destAddr;//目标地址
    quint8 prm;//启动站or从动站
}FrameInfo;
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 controlSwitch;//负荷控制开关（1-启用；2-关闭）
}StartChargingInfo;//充电启动PF=0x01（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 controlSwitch;//负荷控制开关（1-启用；2-关闭）
    quint8 succFlag;//成功标识（0-成功；1-失败）
}StartChargingAckInfo;//充电启动应答PF=0x02（控制器->计费单元）
#pragma pack()



#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 reason;//停止充电原因（1-计费单元正常停止；2-计费单元故障停止）
}StopChargingInfo;//充电停止PF=0x03（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 succFlag;//成功标识（0-成功；1-失败）
}StopChargingAckInfo;//停止应答PF=0x04（控制器->计费单元）
#pragma pack()

typedef struct
{
    quint8 year;//年（0~99）
    quint8 month;//月（0~12）
    quint8 day;//日（1~31）
    quint8 weekDay;//星期几（1-7）
    quint8 hour;//时（0~23）
    quint8 minute;//分（0~59）
    quint16 millisecond;//毫秒
}CP56time2a;//时间

typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    CP56time2a datetime;//时间
}SetTimeInfo;//下发对时PF=0x05（计费单元->控制器）

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    CP56time2a datetime;//控制器时间
}SetTimeAckInfo;//对时应答PF=0x06（控制器->计费单元）
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 majorVersion;//主版本
    quint8 minorVersion;//次版本
}CheckVersionInfo;//下发版本校验PF=0x07（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 majorVersion;//主版本
    quint8 minorVersion;//次版本
}CheckVersionAckInfo;//版本确认PF=0x08（控制器->计费单元）
#pragma pack()



#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 area[3];//充电桩区域编码（低在前，高在后）
    quint8 id[4];//充电桩编号（低在前，高在后）
}SetParaInfo;//下发充电桩参数PF=0x09（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 succFlag;//成功标识（0-成功；1-失败）
    quint8 reason; //失败原因
}SetParaAckInfo;//参数确认PF=0x0A（控制器->计费单元）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 command;//操作指令（1-充电服务停止；2-充电服务恢复）
}ServiceControlInfo;//充电服务启停控制PF=0x0B（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 command;//操作指令（1-充电服务停止；2-充电服务恢复）
    quint8 result;//执行结果反馈（1-操作成功；2-操作失败）
}ServiceControlAckInfo;//充电服务启停控制反馈PF=0x0C（控制器->计费单元）
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 index;//电子锁序号（从1开始）
    quint8 command;//操作指令（1-上锁；2-解锁）
}LockControlInfo;//电子锁控制PF=0x0D（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 index;//电子锁序号（从1开始）
    quint8 command;//操作指令（1-上锁；2-解锁）
    quint8 result;//执行结果反馈（1-操作成功；2-操作失败）
}LockControlAckInfo;//电子锁控制反馈PF=0x0E（控制器->计费单元）
#pragma pack()



#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 command;//指令类型（1-功率绝对值；2-百分比）
    quint8 para[2];//功率调节参数（绝对值：-1000.0kw~1000.0kw；百分比：0~100%）
}PowerControlInfo;//功率调节控制PF=0x0F（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 command;//指令类型（1-功率绝对值；2-百分比）
    quint8 para[2];//功率调节参数（绝对值：-1000.0kw~1000.0kw；百分比：0~100%）
    quint8 result;//执行结果反馈（1-设定成功；2-设定失败）
}PowerControlAckInfo;//功率调节反馈PF=0x10（控制器->计费单元）
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 controlSwitch;//负荷控制开关（1-启用；2-关闭）
    quint8 succFlag;//成功标识（0-成功；1-失败）
    quint8 errorCode;//失败原因
    quint8 CprotocolversionB[3];			//充电桩与BMS通信协议版本号
    quint8 BprotocolversionC[3];			//BMS与充电桩通信协议版本号
    quint8 handshake;					//握手结果
    quint8 batterytype;					//电池类型
    quint8 Maxallowabletemperature;		//最高允许温度
    quint16 BMSMaxvoltage;//BMS最高允许充电电压
    quint16 Maxvoltage;//单体最高允许充电电压
    quint16 Maxelectricity;//最高允许充电电流
    quint16 carBatteryTotalVoltage;//整车动力蓄电池额定总电压
    quint16 carBatteryCurrentVoltage;//整车动力蓄电池当前电压
    quint16 carBatteryCapacity;//整车动力蓄电池额定容量
    quint16 carBatteryNominalEnergy;//整车动力蓄电池标称能量
    quint16 carBatteryHeDian;//整车动力蓄电池荷电状态
    quint16 pileMaxOutputVoltage;//充电机最高输出电压
    quint16 pileMinOutputVoltage;//充电机最低输出电压
    quint16 pileMaxOutputElectricity;//充电机最大输出电流
    quint16 pileMinOutputElectricity;//充电机最大输出电流
    quint8  vehicleIdentificationNum[17];//车辆识别吗
}StartFinishInfo_11;//充电启动完成PF=0x11（控制器->计费单元）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 controlSwitch;//负荷控制开关（1-启用；2-关闭）
    quint8 succFlag;//成功标识（0-成功；1-失败）
}StartFinishAckInfo;//充电启动完成应答PF=0x12（计费单元->控制器）
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 reason;//停止原因（1-计费单元控制停止充电；2-充电桩故障，控制器主动终止充电）
    quint8 succFlag;//成功标识（0-成功；1-失败）
//#ifdef	DCCHARGING
//    quint8	BMSstopreason;//BMS中止充电原因
//    quint8	BMSfaultreason[2];//BMS充电故障原因
//    quint8  	BMSerrorreason;//BMS中止错误原因
//    quint8	SOC;			//中止荷电状态soc%
//    quint8   batteryCellMinvoltage[2];//动力蓄电池单体最低电压
//    quint8   batteryCellMaxvoltage[2];//动力蓄电池单体最高电压
//    quint8 	batteryCellMinTemperature;//动力蓄电池单体最低温度
//    quint8 	batteryCellMaxTemperature;//动力蓄电池单体最高温度
//    quint8	revenvironment00;		//接收SPN22560=0x00的充电机辨识报文超时
//    quint8	revenvironmentAA;		//接收SPN22560=0xAA的充电机辨识报文超时
//    quint8	revenvironmenttime;		//接收充电机的时间同步和充电机最大输出能力报文超时
//    quint8	revenvironmentfinish;	//接收充电机完成充电准备报文超时
//    quint8	revenvironmentcharge;	//接收充电机充电状态报文状态报文超时
//    quint8	revenvironmentstopcharge;//接收充电机中止充电报文超时
//    quint8	revenvironmenttotalovertime;//接收充电机充电统计报文超时
//    quint8	BMSfault;//BMS其他错误
//    quint8	revBMScarovertime;//接收BMS和车辆的辨识报文超时
//    quint8	revbatteryparaovertime;//接收电池充电参数报文超时
//    quint8	revBMSfinishovertime;//接收BMS完成充电准备报文超时
//    quint8	revbatterytotalovertime;//接收电池充电总状态报文超时
//    quint8	revbattaryovertime;//接收电池充电要求报文超时
//    quint8	revBMSstopchargeovertime;//接收BMS中止充电报文超时
//    quint8	revBMSchargeovertime;//接收BMS充电统计报文超时
//    quint8	environmentfault;//充电机其他错误

//	quint8	BMSstopreason;//BMS中止充电原因
//	quint8	BMSfaultreason[2];//BMS充电故障原因
//	quint8  	BMSerrorreason;//BMS中止错误原因
//	quint8	SOC;			//中止荷电状态soc%
//	quint8   batteryCellMinvoltage[2];//动力蓄电池单体最低电压
//	quint8   batteryCellMaxvoltage[2];//动力蓄电池单体最高电压
//	quint8 	batteryCellMinTemperature;//动力蓄电池单体最低温度
//	quint8 	batteryCellMaxTemperature;//动力蓄电池单体最高温度
//	quint8	revenvironment00[2];		//接收SPN22560=0x00的充电机辨识报文超时
//	quint8	revenvironmentAA[2];		//接收SPN22560=0xAA的充电机辨识报文超时
//	quint8	revenvironmenttime[2];		//接收充电机的时间同步和充电机最大输出能力报文超时
//	quint8	revenvironmentfinish[2];	//接收充电机完成充电准备报文超时
//	quint8	revenvironmentcharge[2];	//接收充电机充电状态报文状态报文超时
//	quint8	revenvironmentstopcharge[2];//接收充电机中止充电报文超时
//	quint8	revenvironmenttotalovertime[2];//接收充电机充电统计报文超时
//	quint8	BMSfault[6];//BMS其他错误
//	quint8	revBMScarovertime[2];//接收BMS和车辆的辨识报文超时
//	quint8	revbatteryparaovertime[2];//接收电池充电参数报文超时
//	quint8	revBMSfinishovertime[2];//接收BMS完成充电准备报文超时
//	quint8	revbatterytotalovertime[2];//接收电池充电总状态报文超时
//	quint8	revbattaryovertime[2];//接收电池充电要求报文超时
//	quint8	revBMSstopchargeovertime[2];//接收BMS中止充电报文超时
//	quint8	revBMSchargeovertime[2];//接收BMS充电统计报文超时
//	quint8	environmentfault[6];//充电机其他错误
//#endif
}StopFinishInfo;//停止充电完成PF=0x13（控制器->计费单元）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 	interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 	reason;//停止原因（1-计费单元控制停止充电；2-充电桩故障，控制器主动终止充电）
    quint8	ackFlag;//确认标识（0-成功；1-失败）
}StopFinishAckInfo;//停止充电完成应答PF=0x14（计费单元->控制器）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8	interface;//充电接口标识
    quint16	outputVoltage;//充电输出电压
    quint16 outputCurrent;//充电输出电流
    quint8 	SOC;
    quint8 	batteryMinTemperature;//电池组最低温度
    quint8 	batteryMaxTemperature;//电池组最高温度
    quint16 batteryCellMaxVoltage;//单体电池最高电压
    quint16	batteryCellMinVoltage;//单体电池最低电流
    quint8 	environmentTemperature;//充电机环境温度
    quint16	guidanceVoltage;//充电导引电压
    quint16 BMSNeedV;//BMS需求电压
    quint16 BMSNeedA;//BMS需求电流
    quint8  chargerMode;//充电模式
    quint16 BMSMeasureV;//BMS充电电压测量值
    quint16 BMSMeasureA;//BMS充电电流测量值
    quint16 estimateSurplusTime;//估计剩余充电时间
    quint8  probeOne_DC;
    quint8  probeTwo_DC;
    quint8  probeThree_DC;
    quint8  probeFour_DC_;
}DC_TelemetryInfo_20;//直流遥测PF=0x20（控制器->计费单元）
#pragma pack()

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 currEnergy[2];//当前充电电量（精确到小数点后1位）
    quint8 totalMinute[2];//累计充电时间（分钟）
}DC_TelemetryInfo_21;//直流遥测PF=0x21（计费单元->控制器）
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8	interface;//充电接口标识1 data0
    //Data1 (bit0-bit7)
    quint8	workingStatus:2;//工作状态
    quint8  totalFault:1;//总故障
    quint8  totalAlarm:1;//总告警
    quint8 	scram_button_fault:1;//急停按钮动作故障
    quint8 	smoke_alarm:1;//烟雾报警告警
    quint8 	AC_breaker_fault:1;//交流断路器故障
    quint8	DCbus_output_contactor:1;//直流母线输出接触器故障
    //Data2 (bit0-bit7)
    quint8 	DCbus_output_fuse:1;//直流母线输出熔断器故障
    quint8 	charge_port_lock_fault:1;//充电接口电子锁故障
    quint8	charge_fan_fault:1;//充电机风扇故障
    quint8 	arrester_fault:1;//避雷器故障
    quint8 	insulation_monitor_fault:1;//绝缘监测故障
    quint8 	battery_reverse_fault:1;//电池反接故障
    quint8 	guidance_fault:1;//充电中车辆控制导引告警
    quint8 	pile_over_temperature_fault:1;//充电桩过温故障
    //Data3 (bit0-bit7)
    quint8	chargegun_over_temperature_fault:1;//充电枪过温故障
    quint8 	charging_gun_Noplace:1;//充电枪未归位
    quint8	BMS:1;//BMS通信异常
    quint8 	input_overvoltage:1;//输入电压过压
    quint8 	input_undervoltage:1;//输入电压欠压
    quint8 	DCbus_output_overvoltage:1;//直流母线输出过压告警
    quint8 	DCbus_output_undervoltage:1;//直流母线输出欠压告警
    quint8 	battery_output_overcurrent:1;//直流母线输出过流告警
    //Data4 (bit0-bit7)
    quint8 	chargemodule_fault:1;//充电模块故障
    quint8 	module_input_alarm:1;//充电模块交流输入告警
    quint8 	module_input_overvoltage:1;//充电模块交流输入过压告警
    quint8 	module_input_undervoltage:1;//充电模块交流输入欠压告警
    quint8 	module_input_defaultphase:1;//充电模块交流输入缺相告警
    quint8 	module_output_short:1;//充电模块直流输出短路故障
    quint8 	module_output_overcurrent:1;//充电模块直流输出过流告警
    quint8 	module_output_overvoltage:1;//充电模块直流输出过压告警

    //Data5 (bit0-bit7)
    quint8 	module_output_undervoltage:1;//充电模块直流输出欠压告警
    quint8 	module_over_temperature:1;//充电模块过温告警
    quint8 	module_comm_fault:1;//充电模块通信告警
    quint8 	module_fan_fault:1;//充电模块风扇故障
    quint8 	onoff_link_car:1;//是否连接车辆   //是否连接车辆 0 连接
    quint8	chargegun_status:1;//充电桩充电枪座状态
    quint8	chargeport_lockstatus:1;//充电接口电子锁状态
    quint8 	output_contactor:1;//直流输出接触器状态

    quint8 	alarm_information_number;//其它类型故障-充电机私有告警编号Data6
    quint8 	alarm_information_value;//其它类型故障-充电机私有故障信息Data7
}DC_TelesignalInfo_22;//直流充电桩遥信PF=0x22（控制器->计费单元）
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8 interface;                               //充电接口标识1
    quint8 entra_guadr_fault :1;                    //门禁故障
    quint8 DC_output_contactor_synechia_fault:1;    //直流输出接触器粘链故障
    quint8 insulation_monitor_alarm:1;              //绝缘监测告警
    quint8 voltage_release_circuit_fault:1;         //泻放回路故障
    quint8 pile_over_temperature_alarm :1;          //充电桩过温告警
    quint8 chargegun_over_temperature_alarm:1;      //充电枪过温告警
    quint8 AC_input_contactor_maloperation_fault:1; //交流输入接触器据动/误动故障
    quint8 AC_input_contactor_synechia_fault:1;     //交流输入接触器粘链故障
    quint8 APS_fault:1;                             //辅助电源故障
    quint8 mutiple_contactor_maloperation_fault:1;  //并联接触器据动/误动故障
    quint8 mutiple_contactor_synechia_fault:1;      //并联接触器粘链故障
}DC_TelesignalInfo_23;//直流充电桩遥信PF=0x23（控制器->计费单元）
#pragma pack()


typedef struct
{
    quint8	interface;//充电接口标识1

    quint8	workingStatus;//工作状态2
    quint8 	scram_button_fault;//急停按钮动作故障9
    quint8 	smoke_alarm;//烟雾报警告警15
    quint8 	AC_breaker_fault;//交流断路器故障32
    quint8	DCbus_output_contactor;//直流母线输出接触器故障Data1-1bit(bit7)

    quint8 	DCbus_output_fuse;//直流母线输出熔断器故障34Data2-1bit(bit0)
    quint8 	charge_port_lock_fault;//充电接口电子锁故障
    quint8	charge_fan_fault;//充电机风扇故障
    quint8 	arrester_fault;//避雷器故障12
    quint8 	insulation_monitor_fault;//绝缘监测故障10
    quint8 	battery_reverse_fault;//电池反接故障11
    quint8 	guidance_fault;//充电中车辆控制导引告警31
    quint8 	charging_over_temperature;//充电桩过温故障14

    quint8	chargegun_over_temperature;//充电枪过温故障
    quint8 	charging_gun_Noplace;//充电枪未归位13
    quint8	BMS;//BMS通信异常3
    quint8 	input_overvoltage;//输入电压过压16
    quint8 	input_undervoltage;//输入电压欠压17
    quint8 	DCbus_output_overvoltage;//直流母线输出过压告警4
    quint8 	DCbus_output_undervoltage;//直流母线输出欠压告警5
    quint8 	battery_input_overcurrent;//蓄电池充电过流告警6

    quint8 	chargemodule_fault;//充电模块故障18Data4
    quint8 	module_input_fault;//充电模块交流输入告警22
    quint8 	module_input_overvoltage;//充电模块交流输入过压告警27
    quint8 	module_input_undervoltage;//充电模块交流输入欠压告警28
    quint8 	module_input_defaultphase;//充电模块交流输入缺相告警29
    quint8 	module_output_short;//充电模块直流输出短路故障23
    quint8 	module_output_overcurrent;//充电模块直流输出过流告警24
    quint8 	module_output_overvoltage;//充电模块直流输出过压告警25

    quint8 	module_output_undervoltage;//充电模块直流输出欠压告警26Data5
    quint8 	module_over_temperature;//充电模块过温告警21
    quint8 	module_comm_fault;//充电模块通信告警30
    quint8 	module_fan_fault;//充电模块风扇故障20
    quint8 	onoff_link_car;//是否连接车辆8
    quint8	chargegun_status;//充电桩充电枪座状态
    quint8	chargeport_lockstatus;//充电接口电子锁状态
    quint8 	output_contactor;//直流输出接触器状态19

    quint8 	alarm_information_number;//其它类型故障-充电机私有告警编号Data6
    quint8 	alarm_information_value;//其它类型故障-充电机私有故障信息Data7
//	quint8	 interface;//充电接口标识
//
//  	quint8	workingStatus;//工作状态2
//  	quint8	BMS;//BMS通信异常3
//  	quint8 DCbus_output_overvoltage;//直流母线输出过压告警4
//  	quint8 DCbus_output_undervoltage;//直流母线输出欠压告警5
//  	quint8 battery_input_overcurrent;//蓄电池充电过流告警6
//
//  	quint8 battery_over_temperature;//蓄电池模块采样点过温告警7
//  	quint8 onoff_link_battery;//是否连接电池8
//  	quint8 scram_button_fault;//急停按钮动作故障9
//  	quint8 insulation_monitor_fault;//绝缘监测故障10
//  	quint8 battery_reverse_fault;//电池反接故障11
//  	quint8 arrester_fault;//避雷器故障12
//  	quint8 charging_gun_Noplace;//充电枪未归位13
//  	quint8 charging_over_temperature;//充电桩过温故障14
//
//  	quint8 smoke_alarm;//烟雾报警告警15
//  	quint8 input_overvoltage;//输入电压过压16
//  	quint8 input_undervoltage;//输入电压欠压17
//  	quint8 module_fault;//充电模块故障18
//  	quint8 output_contactor;//输出接触器状态19
//  	quint8 module_fan_fault;//充电模块风扇故障20
//  	quint8 module_over_temperature;//充电模块过温告警21
//  	quint8 module_input_fault;//充电模块交流输入告警22
//
//  	quint8 module_output_short;//充电模块输出短路故障23
//  	quint8 module_output_overcurrent;//充电模块输出过流告警24
//  	quint8 module_output_overvoltage;//充电模块输出过压告警25
//  	quint8 module_output_undervoltage;//充电模块输出欠压告警26
//  	quint8 module_input_overvoltage;//充电模块输入过压告警27
//  	quint8 module_input_undervoltage;//充电模块输入欠压告警28
//  	quint8 module_input_defaultphase;//充电模块输入缺相告警29
//  	quint8 module_comm_fault;//充电模块通信告警30
//
//  	quint8 guidance_fault;//充电中车辆控制导引告警31
//  	quint8 AC_breaker_fault;//交流断路器故障32
//  	quint8 DCbus_output_overcurrent;//直流母线输出过流告警33
//  	quint8 DCbus_output_fuse;//直流母线输出熔断器故障34
//
//  	quint8 alarm_information_number;//其它类型故障
//  	quint8 alarm_information_value;//其它类型故障
}DC_TelesignalInfo_23_OLD;//直流充电桩遥信PF=0x23（控制器->计费单元）

typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 Ua[2];//充电输出A相电压（精确到小数点后1位）
    quint8 Ub[2];//充电输出B相电压（精确到小数点后1位）
    quint8 Uc[2];//充电输出C相电压（精确到小数点后1位）
    quint8 Ia[2];//充电输出A相电流（精确到小数点后2位）
    quint8 Ib[2];//充电输出B相电流（精确到小数点后2位）
    quint8 Ic[2];//充电输出C相电流（精确到小数点后2位）
    quint8 guidanceVoltage[2];//充电导引电压（精确到小数点后2位）
}AC_TelemetryInfo_30;//遥测PF=0x30（控制器->计费单元）

typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 Ua[2];//充电输出A相电压（精确到小数点后1位）
    quint8 Ub[2];//充电输出B相电压（精确到小数点后1位）
    quint8 Uc[2];//充电输出C相电压（精确到小数点后1位）
    quint8 Ia[2];//充电输出A相电流（精确到小数点后2位）
    quint8 Ib[2];//充电输出B相电流（精确到小数点后2位）
    quint8 Ic[2];//充电输出C相电流（精确到小数点后2位）
    quint8 currEnergy[2];//当前充电电量（精确到小数点后1位）
    quint8 totalMinute[2];//累计充电时间（分钟）
}AC_TelemetryInfo_31;//遥测PF=0x31（计费单元->控制器）



typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）

    quint8 workingStatus;//工作状态（0-待机；1-工作；2-充满；3-告警；4-故障）
    quint8 switchStatus;//连接确认开关状态（0-连接；1-未连接）
    quint8 emergencyStopAlarm;//急停动作告警（0-正常；1-异常）
    quint8 arresterFault;//避雷器故障（0-正常；1-异常）
    quint8 gunUnHoming;//充电枪未归位（0-正常；1-异常）

    quint8 overTemperatureFault;//过温故障（0-正常；1-异常）
    quint8 overVoltage;//输入电压过压（0-正常；1-异常）
    quint8 underVoltage;//输入电压欠压（0-正常；1-异常）
    quint8 ouputContactorStatus;//输出接触器状态（0-分段；1-闭合）
    quint8 guidanceAlarm;//充电中车辆控制导引告警（0-正常；1-异常）
    quint8 acContactorFault;//交流接触器故障（0-正常；1-异常）
    quint8 overCurrentAlarm;//输出过流告警（0-正常；1-异常）
    quint8 overCurrentProtect;//输出过流保护动作（0-正常；1-保护）

    quint8 breakerFault;//交流断路器故障（0-正常；1-异常）
    quint8 interLockStatus;//充电接口电子锁状态（0-解锁；1-锁止）
    quint8 interLockFaultStatus;//充电接口电子锁故障状态（0-正常；1-异常）
    quint8 interoverTemperatureFault;//充电接口过温故障（0-正常；1-异常）
    quint8 linkStatus;//充电连接状态CC（0-未连接；1-已连接）
    quint8 controlStatus;//充电控制状态CP（0-其他；1-状态1（12v）；2-状态2（9v）；3-状态3（6v））
    quint8 PEFault;//PE断线故障（0-正常；1-异常）

    quint8 gerAlarm;//通用告警和故障
    quint8 otherAlarm[2];//其他类型故障
}AC_TelesignalInfo_32;//交流充电桩遥信PF=0x32（控制器->计费单元）

typedef struct
{
    quint8 interface;//充电接口标识
    quint8 workingStatus;//工作状态（0-待机；1-工作；2-充满；3-告警；4-故障）
    quint8 switchStatus;//连接确认开关状态（0-连接；1-未连接）
    quint8 emergencyStopAlarm;//急停动作告警（0-正常；1-异常）
    quint8 arresterFault;//避雷器故障（0-正常；1-异常）
    quint8 gunUnHoming;//充电枪未归位（0-正常；1-异常）

    quint8 overTemperatureFault;//过温故障（0-正常；1-异常）
    quint8 overVoltage;//输入电压过压（0-正常；1-异常）
    quint8 underVoltage;//输入电压欠压（0-正常；1-异常）
    quint8 ouputContactorStatus;//输出接触器状态（0-分段；1-闭合）
    quint8 guidanceAlarm;//充电中车辆控制导引告警（0-正常；1-异常）
    quint8 acContactorFault;//交流接触器故障（0-正常；1-异常）
    quint8 overCurrentAlarm;//输出过流告警（0-正常；1-异常）
    quint8 overCurrentProtect;//输出过流保护动作（0-正常；1-保护）

    quint8 breakerFault;//交流断路器故障（0-正常；1-异常）
    quint8 interLockStatus;//充电接口电子锁状态（0-解锁；1-锁止）
    quint8 interLockFaultStatus;//充电接口电子锁故障状态（0-正常；1-异常）
    quint8 interoverTemperatureFault;//充电接口过温故障（0-正常；1-异常）
    quint8 linkStatus;//充电连接状态CC（0-未连接；1-已连接）
    quint8 controlStatus;//充电控制状态CP（0-其他；1-状态1（12v）；2-状态2（9v）；3-状态3（6v））
    quint8 PEFault;//PE断线故障（0-正常；1-异常）

    quint8 gerAlarm;//通用告警和故障
    quint8 otherAlarm[2];//其他类型故障
}AC_TelesignalInfo_33;//交流充电桩遥信PF=0x32（控制器->计费单元）

typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 counter;//心跳发送计数（1~255循环）
    quint8 recvStatus;//报文接收状态（0-正常：能收到充电器发来的报文；1-异常：3s内没收到控制器发来的任何报文）
}HeartbeatInfo_40;//心跳PF=0x40（计费单元->控制器）

#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 counter;//心跳发送计数（1~255循环）
    quint8 recvStatus;//报文接收状态（0-正常：能收到充电器发来的报文；1-异常：3s内没收到控制器发来的任何报文）
    quint8 currStatus;//当前充电服务状态（1-充电服务停止状态；2-充电服务可用状态）
}HeartbeatInfo_41;//心跳PF=0x41（控制器->计费单元）
#pragma pack()


#pragma pack(1)
typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 errorCode;//错误码（0-5）位有效，6 7无效待定
}ErrFrameInfo_51;//错误帧PF=0x51（计费单元->控制器）
#pragma pack()

typedef struct
{
    quint8 interface;//充电接口标识（一桩一枪时，为0；一桩多枪时，为1~255）
    quint8 errorCode;//错误码（0-1）位有效(启动完成状态确认超时-停止完成状态确认超时)，其余位待定
}ErrFrameInfo_52;//错误帧PF=0x52（控制器->计费单元）

typedef struct
{
    StartChargingInfo			PGN_01;
    StartChargingAckInfo		PGN_02;
    StopChargingInfo			PGN_03;
    StopChargingAckInfo			PGN_04;
    SetTimeInfo					PGN_05;
    SetTimeAckInfo				PGN_06;
    CheckVersionInfo			PGN_07;
    CheckVersionAckInfo			PGN_08;
    SetParaInfo					PGN_09;
    SetParaAckInfo				PGN_0A;
    ServiceControlInfo			PGN_0B;
    ServiceControlAckInfo		PGN_0C;
    LockControlInfo				PGN_0D;
    LockControlAckInfo			PGN_0E;
    PowerControlInfo			PGN_0F;
    PowerControlAckInfo			PGN_10;
    StartFinishInfo_11			PGN_11;
    StartFinishAckInfo			PGN_12;
    StopFinishInfo				PGN_13;
    StopFinishAckInfo			PGN_14;
    DC_TelemetryInfo_20			PGN_20;
    DC_TelemetryInfo_21			PGN_21;
    DC_TelesignalInfo_22		PGN_22;
    DC_TelesignalInfo_23		PGN_23;
    AC_TelemetryInfo_30			PGN_30;
    AC_TelemetryInfo_31			PGN_31;
    AC_TelesignalInfo_32		PGN_32;
    AC_TelesignalInfo_33		PGN_33;
    HeartbeatInfo_40			PGN_40;
    HeartbeatInfo_41			PGN_41;
    ErrFrameInfo_51				PGN_51;
    ErrFrameInfo_52				PGN_52;
}GlobalPAPA;

enum PRM
{
    Request,	//计费单元发送-请求
    Respond,	//计费单元发送-应答
    Other		//其他
};

union CHAR_SHORT
{
    quint8 ch[2];
    quint16 sh;
};


#define QREGISTERMETATYPE()  {\
    qRegisterMetaType< INT_LIST >("INT_LIST");\
    qRegisterMetaType< StartFinishInfo_11 >("StartFinishInfo_11");\
    qRegisterMetaType< COST_INFO >("COST_INFO");\
    qRegisterMetaType< EKM_INFO >("EKM_INFO");\
    qRegisterMetaType< TCU_FAULT_REASON >("TCU_FAULT_REASON");\
    qRegisterMetaType< MAP_INT_BOOL >("MAP_INT_BOOL");\
    qRegisterMetaType< CARD_INFO >("CARD_INFO");\
    qRegisterMetaType< STANDYBY_REQUEST >("STANDYBY_REQUEST");\
    qRegisterMetaType< UI_INDEX >("UI_INDEX");\
    qRegisterMetaType< OPERATER_CARD >("OPERATER_CARD");\
    qRegisterMetaType< PAY_WAY >("PAY_WAY");\
    qRegisterMetaType< CHARGER_WAY > ("CHARGER_WAY");\
    qRegisterMetaType< CHARGER_SET_INFO >("CHARGER_SET_INFO");\
    qRegisterMetaType< CHARGER_STAGE >("CHARGER_STAGE");\
    qRegisterMetaType< GUN_CONNECT_STATUS >("GUN_CONNECT_STATUS");\
    qRegisterMetaType< BILLING_STATUS >("BILLING_STATUS");\
    qRegisterMetaType< TCU_STOP_REASON >("TCU_STOP_REASON");\
    qRegisterMetaType< ACCOUNT_INFO >("ACCOUNT_INFO");\
    qRegisterMetaType< FAULT_RECORD >("FAULT_RECORD");\
    qRegisterMetaType< CHARGER_RECORD >("CHARGER_RECORD");\
    qRegisterMetaType< FAULT_RECORD_LIST >("FAULT_RECORD_LIST");\
    qRegisterMetaType< CHARGER_RECORD_LIST >("CHARGER_RECORD_LIST");\
    qRegisterMetaType< CARD_UNLOCK_INFO >("CARD_UNLOCK_INFO");\
    qRegisterMetaType< MSG_ERROR_SHOW >("MSG_ERROR_SHOW");\
    qRegisterMetaType< CARD_WITHHOLD_LOCK_INFO >("CARD_WITHHOLD_LOCK_INFO");\
    qRegisterMetaType< USER_COMMENT_INFO >("USER_COMMENT_INFO");\
    }
#endif // DATA_H

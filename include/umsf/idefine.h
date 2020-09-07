#ifndef __SSA_UMSF_IDEFINE_H
#define __SSA_UMSF_IDEFINE_H
namespace ssa
{
	//	数据和组的输入/输出类型定义
	typedef enum _tagInOutType
	{
		IOT_INNER = 0,
		IOT_INPUT,
		IOT_OUTPUT
	} xmEIOType;

	//	数据匹配目标
	typedef enum _tagDataMatchTarget
	{
		DMT_NULL = 0,
		DMT_SYSTEM,
		DMT_NODE
	} xmEDataMatchTarget;

	//	数据IO时类型检查方式
	typedef enum _tagDataCheckType
	{
		DCT_AUTO = 0,
		DCT_TRY,
		DCT_WEAK,
		DCT_STRICT
	} xmEDataCheckType;

	//	仿真系统状态定义
	typedef enum _tagSimuSystemState
	{
		SSS_FINISH = 0,
		SSS_RUNNING = (1 << 0),
		SSS_REPLAY = (1 << 1),
		SSS_FREEZE = (1 << 2)
	}xmESSState;

	//	触发器类型
	typedef enum _tagTriggerType
	{
		TT_NO_TRIGGER,
		TT_TIME_TRIGGER,
		TT_EVENT_TRIGGER,
		TT_COMMAND_TRIGGER,
		TT_DATA_TRIGGER
	} xmETriggerType;

	//	实例的消息队列类型
	typedef enum _tagInstanceMsgHandle
	{
		IMH_INSTANCE = 0,	//	技术+调度
		IMH_TECHNICIAN,		//	技术
		IMH_SCHEDULER		//	调度
	} xmEInstanceMsgHandle;

	//	接收到的系统消息响应函数
	//	用于统计消息响应函数的执行时间
	typedef enum _tagSystemMessageFunction
	{
		SMF_OnWorldClick = 0,
		SMF_OnSystemTime,
		SMF_OnDataUpdate,
		SMF_OnCtrlCommand,
		SMF_OnSystemEvent,
		SMF_OnBulletinGet,
		SMF_OnMessageGet,
		SMF_ALL,
		SMF_ERROR
	} xmESystemMessageFunction;

	//	模型实例中的消息响应函数
	//	用于统计消息响应函数的执行时间
	typedef enum _tagEInstanceFunction
	{
		IF_OnWorldClick = 0,
		IF_OnSystemTime,
		IF_OnCtrlCommand,
		IF_OnTrigger,
		IF_OnApplyGroupIO,
		IF_OnApplyDataIO,
		IF_ALL,
		IF_ERROR
	} xmEInstanceFunction;


	typedef enum _tagEModelInterface
	{
		//	控制接口
		MI_OnLoad = (1 << 0),
		MI_OnUnload = (1 << 1),
		MI_IsReady = (1 << 2),
		//	命令接口
		MI_OnStart = (1 << 8),
		MI_OnFinish = (1 << 9),
		MI_OnFreeze = (1 << 10),
		MI_OnUnfreeze = (1 << 11),
		MI_OnSpeed = (1 << 12),
		MI_OnCommand = (1 << 13),
		//	驱动接口
		MI_OnDrive = (1 << 16),
		MI_OnTime = (1 << 17),
		//	IO接口
		MI_OnPrepareGroupInput = (1 << 24),
		MI_OnFinishGroupInput = (1 << 25),
		MI_OnPrepareGroupOutput = (1 << 26),
		MI_OnFinishGroupOutput = (1 << 27),
		MI_OnApplyGroupIO = (1 << 28),
		MI_OnApplyDataIO = (1 << 29),
		MI_ALL = 0xFFFFFFFF
	} xmEModelInterface;

	//	UMSF工作模式定义
	typedef enum _tagEUmsfMode
	{
		UM_NORMAL = 0,
		UM_DEBUG,
		UM_DEVELOP
	} xmEUmsfMode;
}
#endif  //  __SSA_UMSF_IDEFINE_H


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

	//	仿真系统状态定义
	typedef enum _tagSimuSystemState
	{
		SSS_FINISH = 0,
		SSS_RUNNING = (1 << 0),
		SSS_REPLAY = (1 << 1),
		SSS_FREEZE = (1 << 2)
	}xmESSState;

	//	UMSF中除了模型实例以外的消息队列对象
	//	用于统计每个消息队列的工作负载
	typedef enum _tagMsgHandleObject
	{
		MHO_MANAGER = 0,
		MHO_TIME_TRIGGER,
		MHO_EVENT_TRIGGER,
		MHO_DATA_TRIGGER
	} xmEMsgHandleObject;


	//	Manager中的消息响应函数
	//	用于统计消息响应函数的执行时间
	typedef enum _tagEManagerFunction
	{
		MF_OnWorldClick = 0,
		MF_OnSystemTime,
		MF_OnDataUpdate,
		MF_OnCtrlCommand,
		MF_OnSystemEvent,
		MF_OnBulletinGet,
		MF_OnMessageGet,
		MF_ALL,
		MF_ERROR
	} xmEManagerFunction;

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
}
#endif  //  __SSA_UMSF_IDEFINE_H


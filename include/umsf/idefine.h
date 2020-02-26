#ifndef __SSA_UMSF_IDEFINE_H
#define __SSA_UMSF_IDEFINE_H
namespace ssa
{
	//	���ݺ��������/������Ͷ���
	typedef enum _tagInOutType
	{
		IOT_INNER = 0,
		IOT_INPUT,
		IOT_OUTPUT
	} xmEIOType;

	//	����ƥ��Ŀ��
	typedef enum _tagDataMatchTarget
	{
		DMT_NULL = 0,
		DMT_SYSTEM,
		DMT_NODE
	} xmEDataMatchTarget;

	//	����ϵͳ״̬����
	typedef enum _tagSimuSystemState
	{
		SSS_FINISH = 0,
		SSS_RUNNING = (1 << 0),
		SSS_REPLAY = (1 << 1),
		SSS_FREEZE = (1 << 2)
	}xmESSState;

	//	UMSF�г���ģ��ʵ���������Ϣ���ж���
	//	����ͳ��ÿ����Ϣ���еĹ�������
	typedef enum _tagMsgHandleObject
	{
		MHO_MANAGER = 0,
		MHO_TIME_TRIGGER,
		MHO_EVENT_TRIGGER,
		MHO_DATA_TRIGGER
	} xmEMsgHandleObject;


	//	Manager�е���Ϣ��Ӧ����
	//	����ͳ����Ϣ��Ӧ������ִ��ʱ��
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

	//	ģ��ʵ���е���Ϣ��Ӧ����
	//	����ͳ����Ϣ��Ӧ������ִ��ʱ��
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


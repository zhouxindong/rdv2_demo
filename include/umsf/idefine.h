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

	//	����IOʱ���ͼ�鷽ʽ
	typedef enum _tagDataCheckType
	{
		DCT_AUTO = 0,
		DCT_TRY,
		DCT_WEAK,
		DCT_STRICT
	} xmEDataCheckType;

	//	����ϵͳ״̬����
	typedef enum _tagSimuSystemState
	{
		SSS_FINISH = 0,
		SSS_RUNNING = (1 << 0),
		SSS_REPLAY = (1 << 1),
		SSS_FREEZE = (1 << 2)
	}xmESSState;

	//	����������
	typedef enum _tagTriggerType
	{
		TT_NO_TRIGGER,
		TT_TIME_TRIGGER,
		TT_EVENT_TRIGGER,
		TT_COMMAND_TRIGGER,
		TT_DATA_TRIGGER
	} xmETriggerType;

	//	ʵ������Ϣ��������
	typedef enum _tagInstanceMsgHandle
	{
		IMH_INSTANCE = 0,	//	����+����
		IMH_TECHNICIAN,		//	����
		IMH_SCHEDULER		//	����
	} xmEInstanceMsgHandle;

	//	���յ���ϵͳ��Ϣ��Ӧ����
	//	����ͳ����Ϣ��Ӧ������ִ��ʱ��
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


	typedef enum _tagEModelInterface
	{
		//	���ƽӿ�
		MI_OnLoad = (1 << 0),
		MI_OnUnload = (1 << 1),
		MI_IsReady = (1 << 2),
		//	����ӿ�
		MI_OnStart = (1 << 8),
		MI_OnFinish = (1 << 9),
		MI_OnFreeze = (1 << 10),
		MI_OnUnfreeze = (1 << 11),
		MI_OnSpeed = (1 << 12),
		MI_OnCommand = (1 << 13),
		//	�����ӿ�
		MI_OnDrive = (1 << 16),
		MI_OnTime = (1 << 17),
		//	IO�ӿ�
		MI_OnPrepareGroupInput = (1 << 24),
		MI_OnFinishGroupInput = (1 << 25),
		MI_OnPrepareGroupOutput = (1 << 26),
		MI_OnFinishGroupOutput = (1 << 27),
		MI_OnApplyGroupIO = (1 << 28),
		MI_OnApplyDataIO = (1 << 29),
		MI_ALL = 0xFFFFFFFF
	} xmEModelInterface;

	//	UMSF����ģʽ����
	typedef enum _tagEUmsfMode
	{
		UM_NORMAL = 0,
		UM_DEBUG,
		UM_DEVELOP
	} xmEUmsfMode;
}
#endif  //  __SSA_UMSF_IDEFINE_H


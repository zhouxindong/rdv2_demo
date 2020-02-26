#pragma once
#ifndef __SSA_VDR_SYSTEMHANDLER_H
#define __SSA_VDR_SYSTEMHANDLER_H

#include "sensmsgs.h"

namespace ssa
{
	class xmVDR_EXPORT xmLogHandler : public xmMsgHandler
	{
	public:
		xmLogHandler(void)
		{
			m_pWnd = 0;
		};
		virtual ~xmLogHandler(void)
		{
			xmMH_DESTRUCT_IMMEDIATELY();	// or xmMH_DESTRUCT_WAIT_IDLE();
		}
	public:
		virtual void OnMSGLog(const xmMSGLog& aMsg);

		void* m_pWnd;

		xmCREATE_MESSAGE_MAP(xmLogHandler);
	};

	class xmVDR_EXPORT xmDataSetHandler : public xmMsgHandler
	{
	public:
		xmDataSetHandler(void)
		{
		};
		virtual ~xmDataSetHandler(void)
		{
			xmMH_DESTRUCT_IMMEDIATELY();	// or xmMH_DESTRUCT_WAIT_IDLE();
		}
	public:
		virtual void OnMSGDataSet(const xmMSGDataSetEvent& aMsg);
		xmCREATE_MESSAGE_MAP(xmDataSetHandler);
	};

	class xmVDR_EXPORT xmNodeHandler : public xmMsgHandler
	{
	public:
		xmNodeHandler(void)
		{
		};
		virtual ~xmNodeHandler(void)
		{
			xmMH_DESTRUCT_IMMEDIATELY();	// or xmMH_DESTRUCT_WAIT_IDLE();
		}
	public:
		virtual void OnMSGNode(const xmMSGNodeEvent& aMsg);
		xmCREATE_MESSAGE_MAP(xmNodeHandler);
	};
}

#endif

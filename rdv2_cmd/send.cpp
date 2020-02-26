#include "stdafx.h"
#include "send.h"

using namespace std;
using namespace rdv2;

std::atomic_flag _SPIN_LOCK = ATOMIC_FLAG_INIT;

void send_proc(rdv2::rdv_2 * rdv)
{
	static thread_local uint8_t step = 1;	

	while (_SPIN_LOCK.test_and_set(memory_order_acquire))
	{
		int r = rand() % 7;
		switch (r)
		{
		case 0:
			rdv->send_cmd(CtrlCmd::Load);
			break;

		case 1:
			rdv->send_cmd(CtrlCmd::Init);
			break;

		case 2:
			rdv->send_cmd(CtrlCmd::Start);
			break;

		case 3:
			rdv->send_cmd(CtrlCmd::Freeze);
			break;

		case 4:
			rdv->send_cmd(CtrlCmd::Unfreeze);
			break;

		case 5:
			rdv->send_cmd(CtrlCmd::Speed, ++step);
			break;

		case 6:
			rdv->send_cmd(CtrlCmd::Finish);
			break;

		default:
			break;
		}

		Sleep(500);
	}
}

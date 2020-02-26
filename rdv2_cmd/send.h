#pragma once
#include "rdv2.h"
#include <thread>
#include <atomic>

extern std::atomic_flag _SPIN_LOCK;

void send_proc(rdv2::rdv_2* rdv);



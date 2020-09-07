#ifndef MMTIMER_H
#define MMTIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void WINAPI CALLBACK MMTimer_Proc(unsigned int, unsigned int, DWORD_PTR, DWORD_PTR, DWORD_PTR);

class MMTimer
{
public:
    MMTimer(void * pClient);
   ~MMTimer();

public:
   void Start(int interval);
   void Stop();
   bool isActive();
   void *m_pClient;
   int  m_nSyncTime;
   int  m_nSyncCycles;

private:
   bool m_bIsActive;
   int  m_idTimer;
};

#endif // MMTIMER_H

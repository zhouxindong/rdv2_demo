#ifndef _PROCESS_TOOL_
#define _PROCESS_TOOL_

/*************************************************
Description: process communication tools,use for  multiprocess exchanging messages.
1. shared memory
**************************************************/

#include <iostream>
#include <string.h>
using namespace std;
#ifdef _WIN32
#include <Windows.h>
#include <assert.h>
#endif

#ifdef __linux
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //getpagesize(  )  
#include <sys/ipc.h>  
#include <sys/shm.h>  
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/sem.h>
#endif

namespace ssa
{
#define MAX_MSG_LENGTH 1024*102
#define MY_SHM_ID 67483  

	typedef struct {
		long nContentsize;
		char cContent[MAX_MSG_LENGTH];
	}MsgHead;

#ifdef __linux
	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short *arry;
	};
#endif

	class xmProcessTool {
	public:
		explicit xmProcessTool();
		~xmProcessTool();
		//one side chName 
		//shoud be number string:like "12345"
		void* CreateChannel(char* chName, int *result, long size = MAX_MSG_LENGTH);
		//other side
		//shoud be number string:like "12345"
		void* OpenChannel(char* chName, int *result);

		void  CloseChannel();
		//get memory pointer
		void* GetShareMemory();
		//block recv 
		void SetBlock(bool bblock = true);
		//send msg cContent
		int  Write(MsgHead *buffer);
		//recv msg cContent
		int  Read(MsgHead *buffer);
	private:
		int  SetSemValue();
		void DelSemValue();
		int  SemaphoreP();
		int  SemaphoreV();
	private:
		void* m_pSharedMemory;
		void* m_bufferMutex;
		bool  m_bBlock;
		void* m_fileMapping;
		int   m_semId;
	};
}
#endif //_PROCESS_TOOL_

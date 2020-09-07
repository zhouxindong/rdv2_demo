#pragma once
#ifndef __SSA_VDR_MEMORYPAGE_H
#define __SSA_VDR_MEMORYPAGE_H

#include "../../include/base/sharememory.h"

#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
#include <atomic>
#include <shared_mutex>
#endif

namespace ssa
{
#define MAX_MEMORYPAGE_SIZE  1024*1024
#define MEMORYPAGE_HEAD_SIZE 128

	class xmMemoryPage
	{
	public:
		//�����û��ƶ����������ڴ����߷ǹ����ڴ�顣
		xmMemoryPage();
		~xmMemoryPage(void);

		bool                 m_bIsShare;
		std::string          m_strName;
		unsigned int         m_nName;
		unsigned int         m_nParaCount;
		unsigned int         m_nVersion;

		typedef struct __tagMemoryInfo
		{
			__tagMemoryInfo(void) : m_uSize(0) {};
			unsigned int m_uSize;
		}__SMemoryInfo;

		__SMemoryInfo        m_MemoryInfo;

	public:
		unsigned char*       GetPointer() const;
		unsigned int         GetSize() const;
		void                 Destroy();
		bool                 IsVaild() const;

		bool                 Make(unsigned int nName, unsigned int uLength, bool isShare = true);

		unsigned int         Read(unsigned int uOffset, void* pBuffer, unsigned int uLength, unsigned int uMaxWaitTime = INFINITE);
		unsigned int         ReadWithSign(unsigned int uOffset, void* pBuffer, unsigned int uLength, unsigned int uSignOffset, void* pSign, unsigned int uSignSize, unsigned int uMaxWaitTime = INFINITE);
		unsigned int         Write(unsigned int uOffset, const void* pBuffer, unsigned int uLength, unsigned int uMaxWaitTime = INFINITE);
		unsigned int         WriteWithSign(unsigned int uOffset, const void* pBuffer, unsigned int uLength, unsigned int uSignOffset, const void* pSign, unsigned int uSignSize, unsigned int uMaxWaitTime = INFINITE);

	private:
		bool                 CreateMemory(unsigned int uLength);

	private:
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾		
		mutable std::shared_mutex    m_Mutex;
#else
		mutable xmMutex              m_Mutex;
#endif
		bool                         m_bCreater;

		//	�ڴ�ӳ���ļ����
#ifdef xmOS_WINDOWS
		HANDLE                       m_hMapFile;
#else 
		key_t                        m_hMapFile;
#endif
		//�ڴ��п�ʼ�洢�û�ֵ��ָ���ַ��MEMORYPAGE_HEAD_SIZE֮��Ŀǰ�߻�ǰ����128���ֽڵı����ռ�
		unsigned char*               m_pBaseAddress;
	};
}

#endif //__SSA_VDR_MEMORYPAGE_H


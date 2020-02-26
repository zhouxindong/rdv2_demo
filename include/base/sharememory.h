/******************************************************************************/
/** @file sharememory.h
@brief
	实现共享内存的操作使用接口。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	sharememory.cpp
@see ssa::xmShareMemory
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_SHAREMEMORY_H
#define __SSA_BASE_SHAREMEMORY_H
#include "buffer.h"
#include "value.h"
#include "singleton.h"
#include "async.h"
#include "rwlock.h"

namespace ssa
{
	/******************************************************************************/
	/** @class xmShareMemory
	@brief
		实现共享内存的操作使用接口。
	@details
		共享内存可在同一台计算机的进程间高速共享数据， xmShareMemory 提供了创建、使用共享
		内存的方法，创建后的共享内存不可以复制。
	@par 多线程安全
		是
	@par 修改记录
		@par
		2017.11.6 
			-# 修改了读写接口
		@par
		2014.03.24 
			-# 代码初次完成。
	*******************************************************************************/
	class xmBASE_EXPORT xmShareMemory : public xmUncopyable
	{
	public:
		/** 创建一个共享内存
		@param [in] nName
			共享内存名称，必须设置为大于0。该名称应在共享内存中唯一。
		@param [in] uSize
			创建共享内存的大小。
		@param [in] uInitValue
			创建共享内存的初始值。
		@param [in] nProtectorName
			对本共享内存进行读写保护的时候使用的全局IPC名称，不同进程如使用同一个共享内存交换
			数据，除了nName参数要一致外， nProtectorName 也要一致，才能起到异步读写保护的作用，
			默认输入为-1的情况下，nProtectorName 将参数nName按位取反（最高位保持为0），由
			xmShareMemory类自动生成。
		@exception xmExCreateShareMemory
			创建共享内存失败，或者计算机中由其他进程创建了同名共享内存，但两者长度不一致时抛出。
		*/
		xmShareMemory(int nName, unsigned int uSize, unsigned char uInitValue = 0, int nProtectorName = -1);

		/** 析构函数，析构时自动销毁共享内存空间。
		*/
		~xmShareMemory(void);

	public:
		/** 判断当前共享内存是否有效
		@return 当构造函数抛出异常时，当前对象无效，返回true
		*/
		inline bool IsInvalid(void) const
		{
			return m_ShareMemoryBuffer.IsNULL();
		}
		/** 判断是否为创建者
		@retval true
			是创建者，表示该进程首先创建共享内存。
		@retval false
			不是创建者，表示本对象使用其他进程创建的共享内存。
		*/
		inline bool IsCreater(void) const
		{
			return m_bCreater;
		}
		/** 获取共享内存名称
		@return 名称
		*/
		inline int GetName(void) const
		{
			return m_nName;
		}
		/** 获取共享内存大小
		@return 大小
		*/
		inline unsigned int GetSize(void) const
		{
			return m_ShareMemoryInfo.m_uSize;
		}

		/** 共享内存数据复位
		@param [in] uValue
			复位值。
		@param [in] uMaxWaitTime
			最长等待时间，默认无限等待。
		@return 
			共享内存大小
		@exception xmExTimeout
			等待超时时抛出。
		*/
		int ResetMeomey(unsigned char uValue = 0, unsigned int uMaxWaitTime = INFINITE);

		/** 读数据，输出到指定地址
		@param [in] uOffset
			起始位置。
		@param [out] pBuffer
			输出地址。
		@param [in] uSize
			读数据大小。
		@param [in] uMaxWaitTime
			最长等待时间，默认无限等待。
		@return 
			实际读数据大小
		@exception xmExIllegalInput
			pBuffer为NULL或者uSize越界时抛出。
		@exception xmExTimeout
			等待超时时抛出。
		*/
		int Read(unsigned int uOffset, void* pBuffer, unsigned int uSize, unsigned int uMaxWaitTime = INFINITE);
		/** 读数据，输出为 xmBuffer
		@param [in] uOffset
			起始位置。
		@param [in] uSize
			读数据大小。
		@param [in] uMaxWaitTime
			最长等待时间，默认无限等待。
		@return 
			xmBuffer 对象
		@exception xmExIllegalInput
			uSize越界时抛出。
		@exception xmExTimeout
			等待超时时抛出。
		*/
		xmBuffer Read(unsigned int uOffset, unsigned int uSize, unsigned int uMaxWaitTime = INFINITE);

		/** 写数据，由指定地址输入
		@param [in] uOffset
			起始位置。
		@param [in] pBuffer
			输入地址。
		@param [in] uSize
			写数据大小。
		@param [in] uMaxWaitTime
			最长等待时间，默认无限等待。
		@return 
			实际写数据大小
		@exception xmExIllegalInput
			pBuffer为NULL或者uSize越界时抛出。
		@exception xmExTimeout
			等待超时时抛出。
		*/
		int Write(unsigned int uOffset, const void* pBuffer, unsigned int uSize, unsigned int uMaxWaitTime = INFINITE);
		/** 写数据，由 xmBuffer 对象输入
		@param [in] uOffset
			起始位置。
		@param [in] aBuffer
			输入 xmBuffer 对象。
		@param [in] uMaxWaitTime
			最长等待时间，默认无限等待。
		@return 
			实际写数据大小
		@exception xmExIllegalInput
			aBuffer为NULL或者长度越界时抛出。
		@exception xmExTimeout
			等待超时时抛出。
		*/
		int Write(unsigned int uOffset, const xmBuffer& aBuffer, unsigned int uMaxWaitTime = INFINITE);

	private:
#pragma pack(1)
		typedef struct xmBASE_EXPORT __tagShareMemoryInfo
		{
			__tagShareMemoryInfo(void) : m_uSize(0){};
			unsigned int m_uSize;
		}__SShareMemoryInfo;
#pragma pack()
		//	共享内存信息
		__SShareMemoryInfo m_ShareMemoryInfo;
		//	供用户使用的共享内存操作接口
		xmStaticBuffer m_ShareMemoryBuffer;

		//	全局的共享内存读写保护句柄
		xmRWLock m_GlobalLock;
		//	本进程内的共享内存读写保护
		mutable xmMutex m_LocalMutex;


		//	用户指定的共享内存名称
		int m_nName;
		//	是否是创建者
		bool m_bCreater;
		//	内存映射文件句柄
#ifdef xmOS_WINDOWS
		HANDLE  m_hMapFile;
#else 
		key_t   m_hMapFile;
#endif
		//	实际分配的共享内存基地址
		void* m_pBaseAddress;

		//	创建共享内存
		void __Create(int nName, unsigned int uSize);

		//	销毁共享内存
		void __Destroy();
	};
}
#endif	//	__SSA_BASE_SHAREMEMORY_H
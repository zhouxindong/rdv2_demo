#ifndef __SSA_BASE_PIPE_H
#define __SSA_BASE_PIPE_H
#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "buffer.h"

namespace ssa
{
	//	管道返回的读句柄，无法通过等信号的方式判断是否可读，因此不能继承xmWaitable
	class xmBASE_EXPORT xmPipe	// : public xmWaitable
	{
	public:
		xmPipe(size_t uMaxCache = 4096) : m_hReadPipe(NULL), m_hWritePipe(NULL), m_uMaxCache(uMaxCache) {}
		virtual ~xmPipe()
		{
			Close();
		}

		int Create(void);
		void Close(void);
		bool IsCreated() const;

		//	下列读写函数为同步函数，返回，即可认为读写操作完成。
		//	Read、Write函数均返回实际读写的字节数，读写失败返回-1。

		//	Read函数中，如果outBuffer为空，则读成功后，outBuffer为读取数据的长度
		//	如果不为空，则使用读取的数据填充，读取数据不足的，使用0补充。
		//	如果管道中没有字符，Read函数将阻塞，直到有内容可读。
		//	如果不希望阻塞，可以先调用TotalBytes()函数查看可读字符数。
		int Read(xmBuffer& outBuffer) const;
		int Write(const xmBuffer& inBuffer);
		int Write(const char* inBuffer, size_t uSize);
		
		//	清空管道中的内容
		void Clear()
		{
			while (true)
			{
				xmBuffer aBuffer;
				if (TotalBytes() > 0)
				{
					Read(aBuffer);
				}
				else
				{
					break;
				}
			}
		}
		//	获取管道中可读的字节数
		int TotalBytes() const
		{
			DWORD totalBytesAvail = 0;
			BOOL bRet = PeekNamedPipe(m_hReadPipe, NULL, 0, NULL, &totalBytesAvail, 0);
			return bRet ? totalBytesAvail : -1;
		}


		HANDLE ReadHandle() const
		{
			return m_hReadPipe;
		}
		HANDLE WriteHandle() const
		{
			return m_hWritePipe;
		}

		//HANDLE GetHandle() const
		//{
		//	return m_hReadPipe;
		//}
	private:
		HANDLE m_hReadPipe;
		HANDLE m_hWritePipe;

		//	由管道中读数据的时候，一次可以读取的最大数，默认4K
		const size_t m_uMaxCache;
	};
}

#endif	//__SSA_BASE_PIPE_H

#ifndef __SSA_VDR_ICOMMPROXY_H
#define __SSA_VDR_ICOMMPROXY_H
#include "export.h"
#include "packet.h"
#include "vdrattr.h"
#include "iservice.h"
#include "iproxy.h"

namespace ssa
{
	/**
	* 通信接口的定义，通过代理模式，可以实现插件方式对通信接口进行管理。
	*/
	class xmVDR_EXPORT xmICommProxy : public xmIProxy
	{
	public:
		xmICommProxy();
		virtual ~xmICommProxy();

		/**
		* 初始化接口，需要继承实现。
		* @param void* 初始化时传入的参数指针，因为在执行初始化时不同的通信接口，参数是不一样的，所以只能使用void*类型进行表示。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool                Initialize(void*) = 0;
		/**
		* 获取VDR的属性信息，需要继承实现。
		* @return 属性信息指针，可根据返回的信息，进一步将信息具体化为某一个类型。
		*/
		virtual xmVDRAttr*          GetVDRAttr() =0;
		/**
		* 获取通信接口的类型，需要继承实现。
		* @return 接口信息类型。
		*/
		virtual xmEVDRCommProxyType GetCommProxyType()= 0;
		/**
		* 获取/设置默认通信方式，包括可靠和不可靠两种模式。
		* @return 通信方式。
		*/		
		virtual int                 GetDefaultQos();
		virtual void                SetDefaultQos(int nQos);
		/**
		* 获取通信接口的ID号，目前使用接口类型来作为ID号标示不同的通信接口。
		* @return 接口ID号。
		*/
		virtual int                 GetProxyID() { return GetCommProxyType(); };
		/**
		* 关闭接口，需要继承实现。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool                Close() = 0;
		/**
		* 启动工作，需要继承实现。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual int                 Start() = 0;
		/**
		* 发送报文至VDR网络，需要继承实现。
		* @param pkt 需要发送的报文。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool                Send(const xmPacket& pkt)  = 0;
		/**
		* 指定处理数据的服务，使用服务接口表示，可以使用不同的服务来处理数据。
		* @param pService 数据处理服务接口。
		* @return 空。
		*/
		virtual void                SetService(xmIService* pService);
		/**
		* 获取指定处理数据的服务。
		* @return 数据处理服务接口指针，0表示未获取到。
		*/
		virtual xmIService*         GetService();
		/**
		* 收到报文，进行分发处理。
		* @param buf 报文缓冲区首地址。
		* @param buf 报文缓冲区长度。
		* @return 处理结果，0表示成功，-1表示处理失败。
		*/
		virtual int                 Recv(char* buf, int nLen);

	private:
		xmIService*                 m_pService;
		int                         m_nQos;
	};
}

#endif //__SSA_VDR_ICOMMPROXY_H
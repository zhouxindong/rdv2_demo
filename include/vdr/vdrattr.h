#pragma once
#ifndef __SSA_VDR_VDRATTR_H
#define __SSA_VDR_VDRATTR_H

#include "export.h"
#include "defs.h"
#include <string>

namespace ssa
{

	/**
	*  VDR系统属性基类，用于系统设计使用，具体的系统描述信息通过子类的定义实现，描述了VDR的基本信息：
	*  1.VDR的ID号；
	*  2.作用域；
	*  3.通信方式；
	*  4.当前VDR所扮演的角色。
	*/
	class xmVDR_EXPORT xmVDRAttr
	{
	public:
		xmVDRAttr();
		/**
		* VDR的ID号。
		* VDR通过ID，可以在一台机器上实现多个数据仓库。
		*/
		unsigned short          m_ID;
		/**
		* 作用域。
		* 一个VDR里面可以存在多个域。
		*/
		unsigned short          m_uRegion;
		/**
		* 通信方式。
		* 采用的通信方式，目前实现了三种,也可以自行定义ID大于xmECM_MAX既可,在创建的时候配对使用。
		*/
		xmEVDRCommProxyType     m_eCommType;
		/**
		* 角色定义。
		* 区分当前VDR接口在进程中所扮演的角色，可以是服务器、单数据集合以及运行时环境RTE。
		*/
		xmEEndpointType         m_eEndpointType;
	};

	/**
	*  多播方式的VDR属性，继承于xmVDRAttr，描述了多播通信方式的基本参数
	*  对其中涉及网络属性的设置，建议采用函数形式，也可以直接对属性变量赋值。
	*/
	class xmVDR_EXPORT xmMCastVDRAttr : public xmVDRAttr
	{
	public:
		xmMCastVDRAttr();
		xmMCastVDRAttr(const xmMCastVDRAttr& attr);
		virtual ~xmMCastVDRAttr(void);

		/**
		* 判断两个属性藐视是否一致。
		* @param attr 被比较的属性值。
		* @return 结果，true表示成功，false表示失败。
		*/
		bool operator==(const xmMCastVDRAttr &attr);
		/**
		* 设置服务器地址和端口号，用于服务器接收外部的控制等单独的服务控制需求。
		* @param servIP 服务器的IP地址。
		* @param uPort 服务器的端口号。
		* @return 为空。
		*/
		void SetServerAddr(const char* servIP, unsigned short uPort);
		/**
		* 设置客户端地址和端口号，用于对服务器与客户端之间的服务控制需求。
		* @param userIP 客户端的IP地址。
		* @param uPort 客户端的端口号。
		* @return 为空。
		*/
		void SetUserAddr(const char* userIP, unsigned short uPort);
		/**
		* 设置用于仿真系统实际传输使用的组播地址和端口号，必须设置。
		* @param mcastIP 组播的的IP地址。
		* @param uMcastPort 客户端的端口号。
		* @return 为空。
		*/
		void SetGroupAddr(const char* mcastIP, unsigned short uMcastPort);
		/**
		* 设置用于仿真系统实际传输使用的用户名和密码，用于对数据的加密和的仿真系统的用户权限，待实现。
		* @param userName 用户名字。
		* @param userPwd 密码。
		* @return 为空。
		*/
		void SetUserPwd(const char* userName, const char*  userPwd);

	public:
		/**
		* 服务器地址。
		*/
		std::string         m_srvIP;
		/**
		* 服务器端口。
		*/
		unsigned short      m_srvPort;
		/**
		* 客户端地址。
		*/
		std::string         m_usrIP;
		/**
		* 客户端端口。
		*/
		unsigned short      m_usrPort;
		/**
		* 组播地址。
		*/
		std::string         m_groupIP;
		/**
		* 组播端口号。
		*/
		unsigned short      m_groupPort;
		/**
		* 用户名字。
		*/
		std::string         m_usrName;
		/**
		* 用户密码。
		*/
		std::string         m_usrPwd;
	};

	/**
	*  本地通信方式的VDR属性，继承于xmVDRAttr，描述了本地通信方式的基本参数
	*/
	class xmVDR_EXPORT xmLocalVDRAttr : public xmVDRAttr
	{
	public:
		xmLocalVDRAttr(void);
		xmLocalVDRAttr(const xmLocalVDRAttr& attr);
		virtual ~xmLocalVDRAttr(void) {}

		/**
		* 设置成为主数据集的方式。
		*/
		typedef enum __tagToBeMajor
		{
			xmETBM_AUTO = 0,	/**< 表示由VDR自动选择。 */
			xmETBM_OTHER,		/**< 表示指定主数据集，但主数据集不是本数据集。 */
			xmETBM_THIS,		/**< 表示指定主数据集，主数据集是本数据集。 */
		}xmETBMType;
		/**
		* 主数据集合选择方式。
		*/
		xmETBMType   m_eToBeMajor;
		/**
		* VDR时钟节拍，单位：ms。
		*/
		unsigned int m_uTimer;
	};
}

#endif //__SSA_VDR_VDRATTR_H
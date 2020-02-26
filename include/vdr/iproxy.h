#pragma once
#ifndef __SSA_IPROXY_H
#define __SSA_IPROXY_H

class xmIProxy
{
public:
	xmIProxy() { m_bValid = true; };
	~xmIProxy() {};

public:
	/**
	* 代理的编号，用户使用10000以上的编号。
	*/
	typedef enum __tagProxyUserID
	{
		xmEPUID_COMM_BEGAIN  = 0,     /**< 通信协议编码号起始ID。为了不重复定义，
									  各种类型的通信协议的ID与通信类型的定义一致，具体参见xmEVDRCommProxyType。 */
		xmEPUID_LOGIC_BEGAIN = 100,   /**< 事物处理协议编码号起始ID。 */
		xmEPUID_SENSMSGPROXY,         /**< 敏感消息的处理业务ID。*/
		xmEPUID_SYNC_LOGIC,           /**< 同步逻辑关系处理业务ID。*/

		xmEPUID_USER_BEGAIN  = 10000, /**< 用户自己定义的协议编码号起始ID。 */

	} xmEProxyUserID;

	/**
	* 需要一个ID号作为工厂创建代理的标志，具体编号的分类参照xmEProxyUserID，需要子类进行具体实现。
	* @return 接口ID号。
	*/
	virtual int  GetProxyID() = 0;
	/**
	* 初始化接口，需要继承实现。
	* @param void* 初始化时传入的参数指针，因为在执行初始化时不同的代理类型，参数是不一样的，所以只能使用void*类型进行表示。
	* @return 结果，true表示成功，false表示失败。
	*/
	virtual bool Initialize(void*) = 0;
	/**
	* 判断代理是否有效。
	* @param bool 初始化时传入的参数指针，因为在执行初始化时不同的代理类型，参数是不一样的，所以只能使用void*类型进行表示。
	* @return 代理是否有效标志，true表示有效，false表示无效。
	*/
	virtual bool IsValid() { return m_bValid; };
	/**
	* 设置代理是否有效。
	* @param bool 代理有效标志，true表示有效，false表示无效。
	* @return 空。
	*/
	virtual void SetValid(bool bValid) { m_bValid = bValid; };

private:
	bool         m_bValid;

};

#endif

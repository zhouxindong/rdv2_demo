#ifndef __SSA_UMSF_INODE_H
#define __SSA_UMSF_INODE_H
#include "imessage.h"
/******************************************************************************/
/** @class xmINode
@brief
	调度节点数据的访问接口
@details
	定义了UMSF本地数据区域全部功能。
@par 多线程安全
	否
*******************************************************************************/
#include "../base/base.h"
namespace ssa
{
#define xmINODE_VERSION "V5.0.00_20190412"
	class xmINode
	{
	public:
		virtual xmRet RegisterMsgHandler(xmMsgHandler* pHandler) = 0;
		virtual xmRet RegisterData(const xmString& strDataName, const xmValue& aValue, const xmString& strAttribute = NULL) = 0;
		virtual xmRet RegisterSensDate(const xmString& strDataPath) = 0;

		virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO) = 0;
		virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO) = 0;
		virtual xmString PrintData(const xmString& strDataName = NULL) = 0;

	public:
		//	inode.h文件版本
		//	用户不需要修改，仅用于UMSF对接口版本的比对
		virtual const char* InterfaceVersion(void) const { return xmINODE_VERSION; };

	protected:
		xmINode() {};
		virtual ~xmINode() {};
	};
}
#endif  //  __SSA_UMSF_INODE_H


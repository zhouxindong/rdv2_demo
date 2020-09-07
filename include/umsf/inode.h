#ifndef __SSA_UMSF_INODE_H
#define __SSA_UMSF_INODE_H
#include "imessage.h"
/******************************************************************************/
/** @class xmINode
@brief
	���Ƚڵ����ݵķ��ʽӿ�
@details
	������UMSF������������ȫ�����ܡ�
@par ���̰߳�ȫ
	��
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
		//	inode.h�ļ��汾
		//	�û�����Ҫ�޸ģ�������UMSF�Խӿڰ汾�ıȶ�
		virtual const char* InterfaceVersion(void) const { return xmINODE_VERSION; };

	protected:
		xmINode() {};
		virtual ~xmINode() {};
	};
}
#endif  //  __SSA_UMSF_INODE_H


#ifndef __UMSF_SYSTEM_LOCAL_H
#define __UMSF_SYSTEM_LOCAL_H

#include "../isystem.h"
using namespace ssa;

class SystemLocal : public xmISystem
{
public:
	SystemLocal(const char* pPropertyJson) {};
	virtual ~SystemLocal() {};

	virtual xmString Version() 
	{ 
		return "Local Communication, to bo continue..."; 
	};
	virtual xmRet Login(const xmString& strNodeID, const xmString& strAttribute = NULL);
	virtual xmRet Logout();
	virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO);
	virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO);
};
#endif  //  __UMSF_SYSTEM_LOCAL_H


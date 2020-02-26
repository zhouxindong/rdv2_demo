#pragma once

#ifndef _RDV2_DIRECTOR_H_
#define _RDV2_DIRECTOR_H_

#include "zvals.h"
#include <direct.h>
#include <string>
#include <regex>

#ifdef RDV2_EXPORTS
#define RDV2_API __declspec(dllexport)
#else
#define RDV2_API __declspec(dllimport)
#endif

_RDV2_BEGIN

class RDV2_API director
{
public:
	static std::string cwd();
	//static std::string to_absolute(std::string relate);

private:
	//static std::string _quate(std::string s);
};

_RDV2_END

#endif /* _RDV2_DIRECTOR_H_ */
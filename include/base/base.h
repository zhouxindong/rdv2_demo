/******************************************************************************/
/** @file base.h
@brief
	base包的接口文件。包含本文件，可以直接使用所有base包提供的功能。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	无
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/

#ifndef __SSA_IBASE_H
#define __SSA_IBASE_H

#include "environment.h"
#include "define.h"
#include "type.h"

#include "errors.h"
#include "exceptions.h"
#include "assertmsg.h"
#include "rtti.h"
#include "ptr.h"
#include "singleton.h"
#include "omap.h"
//#include "refcnt.h"	//	暂时不开放给用户使用

#include "timeutil.h"
#include "stringutil.h"
#include "struct.h"
#include "class.h"
#include "value.h"
#include "buffer.h"

#include "waitable.h"
#include "waitset.h"
#include "async.h"
#include "pipe.h"
#include "socket.h"
#include "thread.h"
#include "threadw.h"
#include "message.h"
#include "msghandler.h"
#include "sharememory.h"
#include "lib.h"

#include "crash.h"

#include "log.h"
#endif // __SSA_IBASE_H

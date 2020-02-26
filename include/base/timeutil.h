/******************************************************************************/
/** @file timeutil.h
@brief
	提供计算代码运行时间、处理相对时间、绝对时间的功能。
@author 
	侯全超
	邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	timeutil.cpp
@see ssa::xmRTime
@see ssa::xmATime
@par 修改记录
	@par
	2014.03.03
		-# 侯全超完成xmETimeFormat、xmETime、xmATime。
		-# 邓华完成xmCALC_RUNTIME组宏。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_TIMEUTIL_H
#define __SSA_BASE_TIMEUTIL_H

#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "stringutil.h"
#ifdef xmOS_WINDOWS
#include <time.h>
#else
#include <sys/time.h>
#endif


namespace ssa
{
/******************************************************************************/
/** @name 计算代码运行时间，精度为微妙。
@{
*/

/** 
@def xmCALC_RUNTIME
	定义定时器。由 xmCALC_RUNTIME 、 xmBEGIN_RUN 、 xmEND_RUN 和 xmRUN_TIME 四个
	宏提供一组带名称的定时器，共同完成代码执行时间测量。其中 xmCALC_RUNTIME 用于定
	义一个计时器， xmBEGIN_RUN 用于启动一个计时器， xmEND_RUN 用于停止一个计时器，
	停止计时器后，使用宏 xmRUN_TIME 获得该段时间，xmRUN_TIME宏返回一个double类型数，
	单位为毫秒，精确到小数点后3位。
@param [in] timeFlag
	计时器名称。
@par 示例
	@par
	@code
        xmCALC_RUNTIME(T1);

		xmBEGIN_RUN(T1);
		for(int i=0; i<0xFF; i++)
		{
			printf("Testing ...\n");
		}
		printf("Test Done\n");
		xmEND_RUN(T1);

		//	打印结果为：Test Time: 20.614
		printf("Test Time: %.3f\n", xmRUN_TIME(T1));
	@endcode
*/
/**
@def xmBEGIN_RUN
	启动一个计时器。
@param [in] timeFlag
	计时器名称。
@see xmCALC_RUNTIME
*/
/**
@def xmEND_RUN
	停止一个计时器。
@param [in] timeFlag
	计时器名称。
@see xmCALC_RUNTIME
*/
/**
@def xmRUN_TIME
	该定时器计时时间。
@param [in] timeFlag
	计时器名称。
@see xmCALC_RUNTIME
*/
#ifdef xmOS_WINDOWS
#define xmCALC_RUNTIME(timeFlag)  					\
	LARGE_INTEGER __tB_##timeFlag;					\
	LARGE_INTEGER __tE_##timeFlag;					\
	LARGE_INTEGER __tC_##timeFlag;					\
	double __tU_##timeFlag

#define xmPREPARE_RUN(timeFlag)						\
	do{												\
	QueryPerformanceFrequency(&__tC_##timeFlag);	\
	}while(0)

#define xmBEGIN_RUN_QUICK(timeFlag)					\
	do{												\
	QueryPerformanceCounter(&__tB_##timeFlag);		\
	}while(0)

#define xmBEGIN_RUN(timeFlag)						\
	do{												\
	QueryPerformanceFrequency(&__tC_##timeFlag);	\
	QueryPerformanceCounter(&__tB_##timeFlag);		\
	}while(0)

#define xmEND_RUN(timeFlag)							\
	do{												\
	QueryPerformanceCounter(&__tE_##timeFlag);		\
	__tU_##timeFlag = (((double)(__tE_##timeFlag.QuadPart - __tB_##timeFlag.QuadPart))/(double)(__tC_##timeFlag.QuadPart))*(double)1000.0;   \
	}while(0)


#else // Linux
#define xmCALC_RUNTIME(timeFlag)					\
	struct timeval __tB_##timeFlag,__tE_##timeFlag;	\
	double __tU_##timeFlag

#define xmPREPARE_RUN(timeFlag)	

#define xmBEGIN_RUN_QUICK(timeFlag)					\
	gettimeofday(&__tB_##timeFlag, NULL)

#define xmBEGIN_RUN(timeFlag)						\
	gettimeofday(&__tB_##timeFlag, NULL)

#define xmEND_RUN(timeFlag)							\
	do{												\
	gettimeofday(&__tE_##timeFlag,NULL);			\
	__tU_##timeFlag = 1000000*(__tE_##timeFlag.tv_sec-__tB_##timeFlag.tv_sec) + __tE_##timeFlag.tv_usec-__tB_##timeFlag.tv_usec;   \
	__tU_##timeFlag /= 1000000;						\
	}while(0)
#endif

#define xmRUN_TIME(timeFlag)	__tU_##timeFlag

/**	@} ************************************************************************/



	/** @enum xmETimeFormat
	@brief
		定义时间打印的格式，供xmRTime、xmATime使用
	*/
	typedef enum
	{						/*  	Relative Time Format	Absolute Time Format */
		xmTF_SYM_S = 0,		/**<	4 15:08:30，				2010-04-13 20:30:45 */
		xmTF_EN_S,			/**<	4d15h08m30s，			2010y04m12d15h23m32s */
		xmTF_CN_S,			/**<	4天15小时08分30秒		2010年04月13日20点30分45秒 */
		xmTF_SYM_MS,		/**<	4 15:08:30.080			2010-04-13 20:30:45.120 */
		xmTF_EN_MS,			/**<	4d15h08m30s120ms		2010y04m12d15h23m32s120ms */
		xmTF_CN_MS,			/**<	4天15小时08分30秒080毫秒	2010年04月13日20点30分45秒120毫秒 */

		xmTF_TIME_SYM_S,	/**<	15:08:30				20:30:45 */
		xmTF_TIME_EN_S,		/**<	15h08m30s120ms			20h30m45s */
		xmTF_TIME_CN_S,		/**<	15小时08分30秒080毫秒	20点30分45秒 */
		xmTF_TIME_SYM_MS,	/**<	15:08:30.080			20:30:45.080 */
		xmTF_TIME_EN_MS,	/**<	15h08m30s120ms			20h30m45s120ms */
		xmTF_TIME_CN_MS,	/**<	15小时08分30秒080毫秒	20点30分45秒080毫秒 */

		xmTF_FILE_NAME,		/**<	4 15h08m30				2010-04-13 20h30m45 */
		xmTF_LOG_FLAG,		/**<	4 15:08:30.080			04/13 20:30:45.080 */
	}xmETimeFormat;




	/******************************************************************************/
	/** @class xmRTime
	@brief
		处理相对时间（Relative Time）
	@details
		相对时间包括：天、时、分、秒、毫秒和符号，用于描述两段时间的间隔，相对时间有正负符号。
		其中：
			- 天  ：0≤D
			- 时  ：0≤H＜24
			- 分  ：0≤M＜60
			- 秒  ：0≤S＜60
			- 毫秒：0≤MS＜1000
			- 符号：相对时间计算方向

	@par 多线程安全
		否
	*******************************************************************************/
	class xmBASE_EXPORT xmRTime
	{
	public:
		/******************************************************************************/
		/** @name 构造函数与析构函数
		@{
		*/

		/** 创建为0的相对时间
		*/
		xmRTime(void) : m_uDay(0), m_uHour(0), m_uMinute(0), m_uSecond(0), m_uMillisecond(0), m_bNegative(false)
		{};
		/** 使用浮点数“秒”创建相对时间，小数部分被设置为毫秒，小数点后第4位起的数据被丢掉。
		@param [in] fSecond
			浮点数“秒”。
		*/
		xmRTime(double fSecond) : m_uDay(0), m_uHour(0), m_uMinute(0), m_uSecond(0), m_uMillisecond(0), m_bNegative(false)
		{
			SetRTime(fSecond);
		}
		/** 使用整数“秒”、“毫秒”和符号位创建相对时间
		@param [in] uSecond
			整型的秒。
		@param [in] uMillisecond
			整型的豪秒，超过1000的部分将被转换为秒。
		@param [in] bNegative
			相对时间的符号，ture表示负数。
		*/
		xmRTime(unsigned long uSecond, unsigned int uMillisecond = 0, bool bNegative = false) : m_uDay(0), m_uHour(0), m_uMinute(0), m_uSecond(0), m_uMillisecond(0), m_bNegative(false)
		{
			SetRTime(uSecond, uMillisecond, bNegative);
		}
		/** 使用天、时、分、秒、毫秒和符号位创建相对时间
		@param [in] uDay
			天数。
		@param [in] uHour
			时，超过23的部分将被转换为天数。
		@param [in] uMinute
			分，超过59的部分将被转换为小时。
		@param [in] uSecond
			秒，超过59的部分将被转换为分。
		@param [in] uMillisecond
			豪秒，超过999的部分将被转换为秒。
		@param [in] bNegative
			相对时间的符号，ture表示负数。
		*/
		xmRTime(unsigned int uDay, unsigned int uHour, unsigned int uMinute, unsigned int uSecond, unsigned int uMillisecond = 0, bool bNegative = false) : m_uDay(0), m_uHour(0), m_uMinute(0), m_uSecond(0), m_uMillisecond(0), m_bNegative(false)
		{
			SetRTime(uDay, uHour, uMinute, uSecond, uMillisecond, bNegative);
		}
		/** 拷贝构造函数
		@param [in] rtm
			另一个相对时间对象。
		*/
		xmRTime(const xmRTime& rtm): 
			m_uDay(rtm.Day()), 
			m_uHour(rtm.Hour()), 
			m_uMinute(rtm.Minute()), 
			m_uSecond(rtm.Second()), 
			m_uMillisecond(rtm.Millisecond()), 
			m_bNegative(rtm.IsNegative())
		{};
		/** 析构函数
		*/
		~xmRTime(void){};

		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 设置类函数
		@{
		*/

		/** 使用浮点数“秒”设置相对时间，小数部分被设置为毫秒，小数点后第4位起的数据被丢掉。
		@param [in] fSecond
			浮点数“秒”。
		*/
		void SetRTime(double fSecond);
		/** 使用整数“秒”、“毫秒”和符号位设置相对时间
		@param [in] uSecond
			整型的秒。
		@param [in] uMillisecond
			整型的豪秒，超过1000的部分将被转换为秒。
		@param [in] bNegative
			相对时间的符号，ture表示负数。
		*/
		void SetRTime(unsigned long uSecond, unsigned int uMillisecond = 0, bool bNegative = false);
		/** 使用天、时、分、秒、毫秒和符号位设置相对时间
		@param [in] uDay
			天数。
		@param [in] uHour
			时，超过23的部分将被转换为天数。
		@param [in] uMinute
			分，超过59的部分将被转换为小时。
		@param [in] uSecond
			秒，超过59的部分将被转换为分。
		@param [in] uMillisecond
			毫秒，超过999的部分将被转换为秒。
		@param [in] bNegative
			相对时间的符号，ture表示负数。
		*/
		void SetRTime(unsigned int uDay, unsigned int uHour, unsigned int uMinute, unsigned int uSecond, unsigned int uMillisecond, bool bNegative = false);

		/** 设置时间为负数
		*/
		inline void SetNegative(void) 
		{ 
			m_bNegative = true; 
		};
		/** 设置时间为正数
		*/
		inline void ClearNegative(void) 
		{ 
			m_bNegative = false; 
		};

		/**	@} ************************************************************************/


		/******************************************************************************/
		/** @name 基本使用函数
		@{
		*/

		/** 获取天数
		@return 天数，范围：0 ≤ D
		*/
		inline unsigned int Day(void) const
		{
			return m_uDay;
		};
		/** 获取小时
		@return 小时，范围：0 ≤ H ＜ 24
		*/
		inline unsigned int Hour(void)const 
		{
			return m_uHour;
		};
		/** 获取分钟
		@return 分钟，范围：0 ≤ M ＜ 60
		*/
		inline unsigned int Minute(void) const 
		{
			return m_uMinute;
		};
		/** 获取秒
		@return 秒，范围：0 ≤ S ＜ 60
		*/
		inline unsigned int Second(void)const 
		{
			return m_uSecond;
		};
		/** 获取毫秒
		@return 毫秒，范围：0 ≤ MS ＜ 1000
		*/
		inline unsigned int Millisecond(void) const 
		{
			return m_uMillisecond;
		};
		/** 获取符号
		@return true为正，false为负。
		*/
		inline bool IsNegative(void) const
		{
			return m_bNegative;
		};

		/** 将相对时间转换为浮点数秒，包括时间正负符号，小数点后表示毫秒值	
		@return 用秒表示的时间。
		*/
		inline double ToSecTime(void) const
		{
			return (m_uDay*86400.0 + m_uHour*3600.0 + m_uMinute*60.0 + m_uSecond*1.0 + m_uMillisecond/1000.0)*(m_bNegative ? -1.0 : 1.0);
		};

		/** 将相对时间以 xmETimeFormat 描述的格式打印成字符串	
		@return 时间字符串。
		*/
		xmString Print(xmETimeFormat eFormat = xmTF_SYM_MS) const;

		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 相对时间的运算符重载
		@{
		*/

		/** 赋值
		@return 自身的引用。
		*/
		xmRTime& operator=(const xmRTime& rtm)
		{
			SetRTime(rtm.Day(), rtm.Hour(), rtm.Minute(), rtm.Second(), rtm.Millisecond(), rtm.IsNegative());
			return *this;
		}
		/** 赋值加
		@return 自身的引用。
		*/
		xmRTime& operator+=(const xmRTime& rtm)
		{
			SetRTime(ToSecTime() + rtm.ToSecTime());
			return *this;
		}
		/** 赋值减
		@return 自身的引用。
		*/
		xmRTime& operator-=(const xmRTime& rtm)
		{
			SetRTime(ToSecTime() - rtm.ToSecTime());
			return *this;
		}
		/** 自增，步长为秒
		@return 自身的引用。
		*/
		xmRTime& operator++(void)
		{
			SetRTime(ToSecTime() + 1.0);
			return *this;
		}
		/** 自减，步长为秒
		@return 自身的引用。
		*/
		xmRTime& operator--(void)
		{
			SetRTime(ToSecTime() - 1.0);
			return *this;
		}
		/** 加
		@return 新的相对时间对象
		*/
		xmRTime operator+(const xmRTime& rtm) const 
		{ 
			return xmRTime(ToSecTime() + rtm.ToSecTime()); 
		};
		/** 减
		@return 新的相对时间对象
		*/
		xmRTime	operator-(const xmRTime& rtm) const 
		{ 
			return xmRTime(ToSecTime() - rtm.ToSecTime()); 
		};
		/** 小于比较
		@return 小于返回true
		*/
		bool operator<(const xmRTime& rtm) const 
		{ 
			return (ToSecTime() < rtm.ToSecTime()); 
		};
		/** 大于比较
		@return 大于返回true
		*/
		bool operator>(const xmRTime& rtm) const 
		{ 
			return (ToSecTime() > rtm.ToSecTime()); 
		};
		/** 小于等于比较
		@return 小于等于返回true
		*/
		bool operator<=(const xmRTime& rtm) const 
		{ 
			return (!operator>(rtm)); 
		};
		/** 大于等于比较
		@return 大于等于返回true
		*/
		bool operator>=(const xmRTime& rtm) const 
		{ 
			return (!operator<(rtm)); 
		};
		/** 等于比较
		@return 等于返回true
		*/
		bool operator==(const xmRTime& rtm ) const
		{
			if (IsNegative() != rtm.IsNegative()|| 
				m_uDay       != rtm.Day()		||
				m_uHour      != rtm.Hour()		||
				m_uMinute    != rtm.Minute()	|| 
				m_uSecond    != rtm.Second()	|| 
				m_uMillisecond != rtm.Millisecond() )
			{
				return false;
			}
			return true;
		}
		/** 不等比较
		@return 不等返回true
		*/
		bool operator!=(const xmRTime& rtm ) const
		{ 
			return (!operator==(rtm)); 
		};

		/**	@} ************************************************************************/

	private:
		unsigned int m_uDay;
		unsigned int m_uHour;
		unsigned int m_uMinute;
		unsigned int m_uSecond;
		unsigned int m_uMillisecond;
		bool m_bNegative;
	};




	/******************************************************************************/
	/** @class xmATime
	@brief
		处理绝对时间（Absolute Time）
	@details
		绝对时间包括年、月、日、时、分、秒、毫秒，表达方式与墙时一致。绝对时间最早由
		1970.1.1 08:00:00开始，在x86平台下，最晚到2038.1.19 08:14:07，在x64平台下，
		最晚到3000.12.31 23:59:59。月、日均由1开始，绝对时间不包括正负符号。
			- 年：1970≤Y≤2037
			- 月：1≤M≤12
			- 日：1≤D≤31
			- 时：0≤H＜24
			- 分：0≤M＜60
			- 秒：0≤S＜60
			- 毫秒：0≤MS＜1000

	@par 多线程安全
		否
	*******************************************************************************/
	class xmBASE_EXPORT xmATime
	{
	public:
		/******************************************************************************/
		/** @name 构造函数与析构函数
		@{
		*/

		/** 创建默认绝对时间
		@param [in] bUpdateNow
			是否设置为当前墙时。ture使用当前墙时构造，false则初始化为：1970.1.1 08:00:00。
		*/
		xmATime(bool bUpdateNow = false)
		{
			if(bUpdateNow)
			{
				UpdateWallClock();
			}
			else
			{
				SetATime(0.0);
			}
		}
		/** 使用 struct tm 结构创建绝对时间
		@param [in] tmTime
			输入参数。
		*/
		xmATime(const struct tm& tmTime)
		{
			m_uMillisecond = 0;
			SetATime(tmTime);
		}
		/** 使用年、月、日、时、分、秒、毫秒设置绝对时间
		@param [in] uYear
			年，不得小于1970，32位程序不得超过2038，64位程序不得超过3000。
		@param [in] uMonth
			月，1～12。
		@param [in] uDay
			日，1～31。
		@param [in] uHour
			时，超过23的部分将被转换为天数。
		@param [in] uMinute
			分，超过59的部分将被转换为小时。
		@param [in] uSecond
			秒，超过59的部分将被转换为分。
		@param [in] uMillisecond
			毫秒，超过999的部分将被转换为秒。
		@exception xmExIllegalInput
			如果创建失败时超出。
		*/
		xmATime(unsigned int uYear, unsigned int uMonth, unsigned int uDay, unsigned int uHour, unsigned int uMinute, unsigned int uSecond, unsigned int uMillisecond)
		{
			if(SetATime(uYear, uMonth, uDay, uHour, uMinute, uSecond, uMillisecond) != xmE_SUCCESS)
			{
				throw xmExIllegalInput();
			}
		}
		/** 使用秒创建绝对时间
		@param [in] fSecond
			使用距离1970.1.1 08:00:00的秒数创建绝对时间，小数部分被设置为毫秒，小数点后第4位起的数据被丢掉。
		*/
		xmATime(double fSecond)
		{
			SetATime(fSecond);
		}
		/** 拷贝构造函数
		@param [in] atm
			另一个绝对时间对象。
		*/
		xmATime(const xmATime& atm)
		{
			m_uMillisecond = atm.Millisecond();
			m_tmTime = atm.ToTMTime();
		}
		/** 析构函数
		*/
		~xmATime(void)
		{};

		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 设置类函数
		@{
		*/

		/** 使用 struct tm 结构设置绝对时间
		@param [in] tmTime
			输入参数。
		@return xmE_SUCCESS
		*/
		inline int SetATime(const struct tm& tmTime);
		/** 使用年、月、日、时、分、秒、毫秒设置绝对时间
		@param [in] uYear
			年，不得小于1970，32位程序不得超过2038，64位程序不得超过3000。
		@param [in] uMonth
			月，1～12。
		@param [in] uDay
			日，1～31。
		@param [in] uHour
			时，超过23的部分将被转换为天数。
		@param [in] uMinute
			分，超过59的部分将被转换为小时。
		@param [in] uSecond
			秒，超过59的部分将被转换为分。
		@param [in] uMillisecond
			毫秒，超过999的部分将被转换为秒。
		@retval  xmE_SUCCESS
			设置成功。
		@retval  xmE_ILLEGAL_INPUT
			参数输入错误。
		*/
		inline int SetATime(unsigned int uYear, unsigned int uMonth, unsigned int uDay, unsigned int uHour, unsigned int uMinute, unsigned int uSecond, unsigned int uMillisecond);
		/** 使用秒创建绝对时间
		@param [in] fSecond
			使用距离1970.1.1 08:00:00的秒数创建绝对时间，小数部分被设置为毫秒，小数点后第4位起的数据被丢掉。
		*/
		inline int SetATime(double fSecond);

		/** 使用当前墙时设置绝对时间。
		*/
		inline void UpdateWallClock(void);

		/**	@} ************************************************************************/




		/******************************************************************************/
		/** @name 基本使用函数
		@{
		*/

		/*==============================================================================
		函 数 名：Year、Month、Day、Hour、Minute、Second
		说    明：获得绝对时间的年、月、日、时、分、秒、毫秒
		==============================================================================*/
		/** 获取年
		@return 年，范围：
		*/
		inline int Year() const
		{
			return m_tmTime.tm_year + 1900;
		};
		/** 获取月
		@return 月，范围：
		*/
		inline int Month() const
		{
			return m_tmTime.tm_mon + 1;
		};
		/** 获取日
		@return 日，范围：
		*/
		inline int Day() const
		{
			return m_tmTime.tm_mday;
		};
		/** 获取时
		@return 时，范围：
		*/
		inline int Hour() const
		{
			return m_tmTime.tm_hour;
		};
		/** 获取分
		@return 分，范围：
		*/
		inline int Minute() const
		{
			return m_tmTime.tm_min;
		};
		/** 获取秒
		@return 秒，范围：
		*/
		inline int Second() const
		{
			return m_tmTime.tm_sec;
		};
		/** 获取毫秒
		@return 毫秒，范围：
		*/
		inline int Millisecond() const
		{
			return m_uMillisecond;
		};
		/** 获得当前日期是本周的星期几，
		@return 星期，范围：周日为0，周一～周六为1～6
		*/
		inline int WeekDay() const
		{
			return m_tmTime.tm_wday;
		};
		/** 获得当前日期是本年的第几天，
		@return 天，范围：0～364（平年）、365（闰年）
		*/
		inline int YearDay() const
		{
			return m_tmTime.tm_yday;
		};
		/** 判断当前年是否是闰年
		@return 是返回true
		*/
		inline bool IsLeap(void) const
		{
			int nYear = m_tmTime.tm_year + 1900;
			return ((nYear%4==0) && (nYear%100!=0) || (nYear%400==0));
		}

		/** 获得当前时间距离1970.1.1 08:00:00有多少秒，小数点后是毫秒
		@return 秒
		*/
		inline double ToSecTime(void) const
		{ 
			return (double)mktime((struct tm*)&m_tmTime) + double(m_uMillisecond)/1000.0; 
		};

		/** 获得用 struct tm 结构表示的当前时间。
		@return struct tm
		*/
		inline struct tm ToTMTime(void) const
		{ 
			return m_tmTime;
		};

		/** 将绝对时间以 xmETimeFormat 描述的格式打印成字符串
		@return 时间字符串。
		*/
		xmString Print(xmETimeFormat eFormat = xmTF_SYM_MS) const;


		/** 获得当前墙时
		@param [out] tmNow
			使用 struct tm 结构表示的当前时间
		@param [out] uMS
			当前的毫秒。
		*/
		static void GetWallClock(struct tm& tmNow, unsigned int& uMS);
		/** 以 xmETimeFormat 描述的格式打印当前墙时
		@return 时间字符串。
		*/
		static xmString PrintWallClock(xmETimeFormat eFormat = xmTF_SYM_MS)
		{
			xmATime aTime(true);
			return aTime.Print(eFormat);
		}

		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 绝对时间的运算符重载
		@{
		*/

		/** 赋值
		@param [in] atm
			绝对时间
		@return 自身的引用。
		*/
		xmATime& operator=(const xmATime& atm)
		{
			m_uMillisecond = atm.Millisecond();
			m_tmTime = atm.ToTMTime();
			return *this;
		}

		/** 时间差计算，绝对时间 - 相对时间 = 绝对时间
		@param [in] rtm
			相对时间
		@return 绝对时间。
		*/
		xmATime operator-(const xmRTime& rtm) const
		{
			return xmATime(ToSecTime() - rtm.ToSecTime());
		}
		/** 时间差计算，绝对时间 - 绝对时间 = 相对时间
		@param [in] atm
			绝对时间
		@return 相对时间。
		*/
		xmRTime	operator-(const xmATime& atm) const
		{
			return xmRTime(ToSecTime() - atm.ToSecTime());
		}
		/** 时间差赋值，绝对时间 - 相对时间 = 绝对时间
		@param [in] rtm
			相对时间
		@return 自身引用。
		*/
		xmATime& operator-=(const xmRTime& rtm)
		{
			SetATime(ToSecTime() - rtm.ToSecTime());
			return *this;
		}
		/** 时间加计算，绝对时间 + 相对时间 = 绝对时间
		@param [in] rtm
			相对时间
		@return 绝对时间。
		*/
		xmATime operator+(const xmRTime& rtm)
		{
			return xmATime(ToSecTime() + rtm.ToSecTime());
		}
		/** 时间加赋值，绝对时间 + 相对时间 = 绝对时间
		@param [in] rtm
			相对时间
		@return 自身引用。
		*/
		xmATime& operator+=(const xmRTime& rtm)
		{
			SetATime(ToSecTime() + rtm.ToSecTime());
			return *this;
		}

		/** 绝对时间小于比较，过去的时间小于现在的时间，现在的时间小于未来的时间。
		*/
		bool operator<(const xmATime& atm )  const
		{ 
			return (ToSecTime() <  atm.ToSecTime()); 
		};
		/** 绝对时间大于比较
		@see operator<()
		*/
		bool operator>(const xmATime& atm )  const
		{ 
			return (ToSecTime() >  atm.ToSecTime()); 
		};
		/** 绝对时间小于等于比较
		@see operator<()
		*/
		bool operator<=(const xmATime& atm) const
		{	
			return (!operator>(atm)); 
		};
		/** 绝对时间大于等于比较
		@see operator<()
		*/
		bool operator>=(const xmATime& atm) const
		{	
			return (!operator<(atm)); 
		};
		/** 绝对时间等于比较
		@see operator<()
		*/
		bool operator==(const xmATime& atm) const
		{
			if (m_uMillisecond != atm.Millisecond())
			{
				return false;
			}
			if (mktime((struct tm*)&m_tmTime) != mktime((struct tm*)&atm.ToTMTime()))
			{
				return false;
			}
			return true;
		};
		/** 绝对时间不等于比较
		@see operator<()
		*/
		bool operator!=(const xmATime& atm) const
		{	
			return (!operator==(atm)); 
		};

		/**	@} ************************************************************************/


	private:
		struct tm m_tmTime;
		unsigned int m_uMillisecond;
	};
}

#endif // __SSA_BASE_TIMEUTIL_H
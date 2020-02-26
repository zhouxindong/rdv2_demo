
#ifndef  __SSA_BASE_SOCKET_H
#define  __SSA_BASE_SOCKET_H
//#pragma once

#include "environment.h"

#include <string>
#include <sstream>

#if defined(xmOS_WINDOWS)
#include <winsock2.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <windows.h>
#undef _WINSOCKAPI_
#else
#include <windows.h>
#endif
#include <ws2ipdef.h>
typedef int socklen_t;
typedef char raw_type;       // Type used for raw data on this platform
#pragma comment(lib,"wsock32")
#pragma comment(lib,"ws2_32.lib")

#elif defined(xmOS_LINUX)
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
typedef void raw_type;       // Type used for raw data on this platform

#else
#error "Unsupported OS"
#endif

#include "singleton.h"
#include "exceptions.h"
#include "waitset.h"
#include "errors.h"
#include "thread.h"
//#include "types.h"

namespace ssa{

	class xmBASE_EXPORT xmSocketEnvironment/* : public xmSingleton<xmSocketEnvironment>*/{
	public:
		//DECLARE_SINGLETON(xmSocketEnvironment);
		void Startup();
		void Cleanup();
	};

	class xmBASE_EXPORT xmSocket : public xmWaitable{
	protected:
#ifdef xmOS_WINDOWS
		typedef SOCKET T_Socket;

		inline static T_Socket DInvalidSocket(){
			return INVALID_SOCKET;
		}

		inline static int DSocketError(){
			return SOCKET_ERROR;
		}

		inline static int DEIntr(){
			return WSAEINTR;
		}

		inline static int DEAgain(){
			return WSAEWOULDBLOCK;
		}

		inline static int DEInProgress(){
			return WSAEWOULDBLOCK;
		}
#elif defined(xmOS_LINUX)
		typedef int T_Socket;

		inline static T_Socket DInvalidSocket(){
			return -1;
		}

		inline static T_Socket DSocketError(){
			return -1;
		}

		inline static int DEIntr(){
			return EINTR;
		}

		inline static int DEAgain(){
			return EAGAIN;
		}

		inline static int DEInProgress(){
			return EINPROGRESS;
		}
#else
#error "Unsupported OS"
#endif

#ifdef xmOS_WINDOWS
		WSAEVENT m_EventForWaitable;
#endif

		T_Socket m_Socket;

		xmSocket();

	public:
		void DisableNaggle();

		void ReuseAddr();

		void SetNonBlockingMode();

		void SetSendBuffSize(int nSendBuf);

		void SetRecvBuffSize(int nRecvBuf);

	public:

		class Exception : public xmException{
		public:
			Exception(const std::string& message = std::string()) :
			  xmException(0,(std::string("[Socket::Exception] ") + message).c_str())
			  {}
		};


		/*******************************************************************************
		Name		: FillAddr
		Purpose		: Function to fill in address structure given an address and port
		*******************************************************************************/
		static sockaddr_in  FillAddr(const std::string &strAddress, short uPort);

		xmSocket(const xmSocket& s);

		virtual ~xmSocket();

		inline bool IsValid()const;
		inline bool IsNotValid()const;
		void Close();
		unsigned short GetLocalPort();

#ifdef xmOS_WINDOWS
		HANDLE GetHandle() const;
		bool CheckSignalled() const;

	protected:
		inline void CreateEventForWaitable();
		inline void CloseEventForWaitable();
		inline void SetWaitingEventsForWindows(long flags);

#else
		//override
		int GetHandle()const;
#endif
	};

		class xmBASE_EXPORT xmIPAddress{
	public:
		unsigned int   host;///< IP address
		unsigned short port;///< IP port number

		inline xmIPAddress() :
		host(0),
			port(0)
		{}

		inline xmIPAddress(unsigned int h, unsigned short p) :
		host(h),
			port(p)
		{}

		/**
		* IPAddress ip(127, 0, 0, 1, 80); //"127.0.0.1" port 80
		* @endcode
		* @param h1 - 1st triplet of IP address.
		* @param h2 - 2nd triplet of IP address.
		* @param h3 - 3rd triplet of IP address.
		* @param h4 - 4th triplet of IP address.
		* @param p - IP port number.
		*/
		inline xmIPAddress(unsigned char h1, unsigned char h2, unsigned char h3, unsigned char h4, unsigned short p) :
		host(((unsigned int)(h1) << 24) + ((unsigned int)(h2) << 16) + ((unsigned int)(h3) << 8) + (unsigned int)(h4)),
			port(p)
		{}

		inline xmIPAddress(const char* ip, unsigned short p) :
		host(xmIPAddress::ParseString(ip)),
			port(p)
		{}

		inline bool operator==(const xmIPAddress& ip);

		static xmIPAddress GetHostByName(const char *hostName, unsigned short port);

	private:
		static inline void ThrowInvalidIP();

		static unsigned int ParseString(const char* ip);
	};

	class xmBASE_EXPORT xmTCPSocket : public xmSocket{
		friend class xmTCPServerSocket;
	public:
  
		xmTCPSocket();

		xmTCPSocket(const xmTCPSocket& s) :
		xmSocket(s)
		{
			//		TRACE(<< "TCPSocket::TCPSocket(copy): invoked " << this << std::endl)
		}

		void Open(const xmIPAddress& ip, bool disableNaggle = false);

		unsigned int Send(char* buf, unsigned uSize);

		unsigned int Recv(char* buf, unsigned uSize);

		xmIPAddress GetLocalAddress();

		xmIPAddress GetRemoteAddress();

#ifdef xmOS_WINDOWS
		void SetWaitingEvents(unsigned int flagsToWaitFor);
#endif

	};

	class xmBASE_EXPORT xmTCPServerSocket : public xmSocket{
		  bool m_bDisableNaggle;//this flag indicates if accepted sockets should be created with disabled Naggle
	public:
		  xmTCPServerSocket();

		  xmTCPServerSocket(const xmTCPServerSocket& s) :
		  xmSocket(s),
			  m_bDisableNaggle(s.m_bDisableNaggle)
		  {}

		  void Open(unsigned short port, bool disableNaggle = false, unsigned short queueLength = 50);

		  xmTCPSocket* Accept();

#ifdef xmOS_WINDOWS
		void SetWaitingEvents(unsigned int flagsToWaitFor);
#endif
	};

	class xmBASE_EXPORT xmUDPSocket : public xmSocket{
	public:
		xmUDPSocket();

		xmUDPSocket(const xmUDPSocket& s) :
		xmSocket(s)
		{}

		void Open(unsigned short port = 0);

		void SetLoop(int flag = 1);

		unsigned Send(char* buf, unsigned int uSize, const xmIPAddress& destinationIP);

		unsigned Recv(char* buf, unsigned int uSize, xmIPAddress &out_SenderIP);

		void JoinGroup(const std::string &multicastGroup);

		void LeaveGroup(const std::string &multicastGroup);

		void SetMulticastTTL(unsigned char multicastTTL);

#ifdef xmOS_WINDOWS
		void SetWaitingEvents(unsigned int flagsToWaitFor);
#endif
	};
}

#endif
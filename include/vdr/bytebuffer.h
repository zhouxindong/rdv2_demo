#ifndef __SSA_VDR_BUFFER_H
#define __SSA_VDR_BUFFER_H

#include <cstddef>   // std::size_t
#include <cstring>   // std::memcpy, std::memcmp, std::memset, std::memchr
#include <stdexcept> // std::out_of_range, std::invalid_argument

#pragma warning(disable: 4251)

namespace ssa
{
	/**
	*  动态缓冲区访问接口，动态调整缓冲区大小，用于有不定长的内存区域的需求。
	*/
	class xmByteBuffer
	{
	public:
		typedef std::size_t size_type;

		static const size_type npos = static_cast<size_type> (-1);

		~xmByteBuffer();

		explicit xmByteBuffer(size_type size = 0);
		xmByteBuffer(size_type size, size_type capacity);
		xmByteBuffer(const void* data, size_type size);
		xmByteBuffer(const void* data, size_type size, size_type capacity);
		xmByteBuffer(void* data, size_type size, size_type capacity,
			bool assume_ownership);

		xmByteBuffer(const xmByteBuffer&);
		xmByteBuffer& operator= (const xmByteBuffer&);

		void  Swap(xmByteBuffer&);
		char* Detach();

		void  Assign(const void* data, size_type size);
		void  Assign(void* data, size_type size, size_type capacity,
			bool assume_ownership);
		void  Append(const xmByteBuffer&);
		void  Append(const void* data, size_type size);
		void  Fill(char value = 0);

		size_type Size() const;
		bool Size(size_type);
		size_type Capacity() const;
		bool Capacity(size_type);
		bool Empty() const;
		void Clear();

		char* Data();
		const char* Data() const;

		char& operator[] (size_type);
		char operator[] (size_type) const;
		char& at(size_type);
		char  at(size_type) const;

		size_type Find(char, size_type pos = 0) const;
		size_type RFind(char, size_type pos = npos) const;

		template<class T>
		size_t Read(const T& Value)
		{
			return Read((char*)&Value, 0, sizeof(T));
		}

		size_t    Read(char *des, size_t offset, size_t len)
		{
			::memcpy((void*)(des), m_pData + offset, len);
			return len;
		}

		template<class T>
		size_t Write(const T& Value)
		{
			return Write((char*)&Value, 0, sizeof(T));
		}

		size_t    Write(const char *src, size_t offset, size_t len)
		{
			Size(len);
			::memcpy(m_pData + offset, (void*)(src), len);
			return len;
		}

	private:
		char*     m_pData;
		size_type m_nSize;
		size_type m_nCapacity;
		bool      m_bFree;
	};

	bool operator== (const xmByteBuffer&, const xmByteBuffer&);
	bool operator!= (const xmByteBuffer&, const xmByteBuffer&);

	//
	// Implementation.
	//
	inline xmByteBuffer::~xmByteBuffer()
	{
		if (m_bFree)
			delete[] m_pData;
	}

	inline xmByteBuffer::xmByteBuffer(size_type s)
		: m_bFree(true)
	{
		m_pData = (s != 0 ? new char[s] : 0);
		m_nSize = m_nCapacity = s;
	}

	inline xmByteBuffer::xmByteBuffer(size_type s, size_type c)
		: m_bFree(true)
	{
		if (s > c)
			throw std::invalid_argument("size greater than capacity");

		m_pData = (c != 0 ? new char[c] : 0);
		m_nSize = s;
		m_nCapacity = c;
	}

	inline xmByteBuffer::xmByteBuffer(const void* d, size_type s)
		: m_bFree(true)
	{
		if (s != 0)
		{
			m_pData = new char[s];
			std::memcpy(m_pData, d, s);
		}
		else
			m_pData = 0;

		m_nSize = m_nCapacity = s;
	}

	inline xmByteBuffer::xmByteBuffer(const void* d, size_type s, size_type c)
		: m_bFree(true)
	{
		if (s > c)
			throw std::invalid_argument("size greater than capacity");

		if (c != 0)
		{
			m_pData = new char[c];

			if (s != 0)
				std::memcpy(m_pData, d, s);
		}
		else
			m_pData = 0;

		m_nSize = s;
		m_nCapacity = c;
	}

	inline xmByteBuffer::xmByteBuffer(void* d, size_type s, size_type c, bool own)
		: m_pData(static_cast<char*> (d)), m_nSize(s), m_nCapacity(c), m_bFree(own)
	{
		if (s > c)
			throw std::invalid_argument("size greater than capacity");
	}

	inline xmByteBuffer::xmByteBuffer(const xmByteBuffer& x)
		: m_bFree(true)
	{
		if (x.m_nCapacity != 0)
		{
			m_pData = new char[x.m_nCapacity];

			if (x.m_nSize != 0)
				std::memcpy(m_pData, x.m_pData, x.m_nSize);
		}
		else
			m_pData = 0;

		m_nSize = x.m_nSize;
		m_nCapacity = x.m_nCapacity;
	}

	inline xmByteBuffer& xmByteBuffer::operator= (const xmByteBuffer& x)
	{
		if (&x != this)
		{
			if (x.m_nSize > m_nCapacity)
			{
				if (m_bFree)
					delete[] m_pData;

				m_pData = new char[x.m_nCapacity];
				m_nCapacity = x.m_nCapacity;
				m_bFree = true;
			}

			if (x.m_nSize != 0)
				std::memcpy(m_pData, x.m_pData, x.m_nSize);

			m_nSize = x.m_nSize;
		}

		return *this;
	}

	inline void xmByteBuffer::Swap(xmByteBuffer& x)
	{
		char* d(x.m_pData);
		size_type s(x.m_nSize);
		size_type c(x.m_nCapacity);
		bool f(x.m_bFree);

		x.m_pData = m_pData;
		x.m_nSize = m_nSize;
		x.m_nCapacity = m_nCapacity;
		x.m_bFree = m_bFree;

		m_pData = d;
		m_nSize = s;
		m_nCapacity = c;
		m_bFree = f;
	}

	inline char* xmByteBuffer::Detach()
	{
		char* r(m_pData);

		m_pData = 0;
		m_nSize = 0;
		m_nCapacity = 0;

		return r;
	}

	inline void xmByteBuffer::Assign(const void* d, size_type s)
	{
		if (s > m_nCapacity)
		{
			if (m_bFree)
				delete[] m_pData;

			m_pData = new char[s];
			m_nCapacity = s;
			m_bFree = true;
		}

		if (s != 0)
			std::memcpy(m_pData, d, s);

		m_nSize = s;
	}

	inline void xmByteBuffer::Assign(void* d, size_type s, size_type c, bool own)
	{
		if (m_bFree)
			delete[] m_pData;

		m_pData = static_cast<char*> (d);
		m_nSize = s;
		m_nCapacity = c;
		m_bFree = own;
	}

	inline void xmByteBuffer::Append(const xmByteBuffer& b)
	{
		Append(b.Data(), b.Size());
	}

	inline void xmByteBuffer::Append(const void* d, size_type s)
	{
		if (s != 0)
		{
			size_type ns(m_nSize + s);

			if (m_nCapacity < ns)
				Capacity(ns);

			std::memcpy(m_pData + m_nSize, d, s);
			m_nSize = ns;
		}
	}

	inline void xmByteBuffer::Fill(char v)
	{
		if (m_nSize > 0)
			std::memset(m_pData, v, m_nSize);
	}

	inline xmByteBuffer::size_type xmByteBuffer::Size() const
	{
		return m_nSize;
	}

	inline bool xmByteBuffer::Size(size_type s)
	{
		bool r(false);

		if (m_nCapacity < s)
			r = Capacity(s);

		m_nSize = s;
		return r;
	}

	inline xmByteBuffer::size_type xmByteBuffer::Capacity() const
	{
		return m_nCapacity;
	}

	inline bool xmByteBuffer::Capacity(size_type c)
	{
		// Ignore capacity decrease requests.
		//
		if (m_nCapacity >= c)
			return false;

		char* d(new char[c]);

		if (m_nSize != 0)
			std::memcpy(d, m_pData, m_nSize);

		if (m_bFree)
			delete[] m_pData;

		m_pData = d;
		m_nCapacity = c;
		m_bFree = true;

		return true;
	}

	inline bool xmByteBuffer::Empty() const
	{
		return m_nSize == 0;
	}

	inline void xmByteBuffer::Clear()
	{
		m_nSize = 0;
	}

	inline char* xmByteBuffer::Data()
	{
		return m_pData;
	}

	inline const char* xmByteBuffer::Data() const
	{
		return m_pData;
	}

	inline char& xmByteBuffer::operator[] (size_type i)
	{
		return m_pData[i];
	}

	inline char xmByteBuffer::operator[] (size_type i) const
	{
		return m_pData[i];
	}

	inline char& xmByteBuffer::at(size_type i)
	{
		if (i >= m_nSize)
			throw std::out_of_range("index out of range");

		return m_pData[i];
	}

	inline char xmByteBuffer::at(size_type i) const
	{
		if (i >= m_nSize)
			throw std::out_of_range("index out of range");

		return m_pData[i];
	}

	inline xmByteBuffer::size_type xmByteBuffer::Find(char v, size_type pos) const
	{
		if (m_nSize == 0 || pos >= m_nSize)
			return npos;

		char* p(static_cast<char*> (std::memchr(m_pData + pos, v, m_nSize - pos)));
		return p != 0 ? static_cast<size_type> (p - m_pData) : npos;
	}

	inline xmByteBuffer::size_type xmByteBuffer::RFind(char v, size_type pos) const
	{
		// memrchr() is not standard.
		//
		if (m_nSize != 0)
		{
			size_type n(m_nSize);

			if (--n > pos)
				n = pos;

			for (++n; n-- != 0;)
			if (m_pData[n] == v)
				return n;
		}

		return npos;
	}

	inline bool operator== (const xmByteBuffer& a, const xmByteBuffer& b)
	{
		return a.Size() == b.Size() &&
			std::memcmp(a.Data(), b.Data(), a.Size()) == 0;
	}

	inline bool operator!= (const xmByteBuffer& a, const xmByteBuffer& b)
	{
		return !(a == b);
	}
}

#endif //
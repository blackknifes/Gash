#ifndef __GSCOPEPOINTER_H__
#define __GSCOPEPOINTER_H__

#include <set>

#define GREFPTR_DEF(type) class type; typedef GScopePointer<type> type##Ptr

template<typename _Ty>
class GScopePointer
{
public:
	GScopePointer() : m_pData(nullptr) {};
	~GScopePointer() { Release(); }

	GScopePointer(_Ty* pData) : m_pData(pData) { AddRef(); };

	GScopePointer(const GScopePointer& pData)
		: m_pData(pData.m_pData)
	{
		AddRef();
	};
	GScopePointer(GScopePointer&& pData)
		:m_pData(pData.m_pData)
	{
		pData.m_pData = nullptr;
	}

	template<typename _castTy>
	GScopePointer(const GScopePointer<_castTy>& pData) 
		: m_pData(pData.m_pData) 
	{ 
		AddRef(); 
	};
	template<typename _castTy>
	GScopePointer(GScopePointer<_castTy>&& pData)
		:m_pData(pData.m_pData)
	{
		pData.m_pData = nullptr;
	}

	bool operator!() const
	{
		return m_pData == nullptr;
	}

	const GScopePointer& operator=(_Ty* pData)
	{
		Release();
		m_pData = pData;
		AddRef();
		return *this;
	}

	template<typename _castTy>
	const GScopePointer& operator=(const GScopePointer<_castTy>& pData)
	{
		Release();
		m_pData = pData.m_pData;
		AddRef();
		return *this;
	}

	template<typename _castTy>
	const GScopePointer& operator=(GScopePointer<_castTy>&& pData)
	{
		Release();
		m_pData = pData.m_pData;
		pData.m_pData = nullptr;
		return *this;
	}

	const GScopePointer& operator=(const GScopePointer& pData)
	{
		Release();
		m_pData = pData.m_pData;
		AddRef();
		return *this;
	}

	const GScopePointer& operator=(GScopePointer&& pData)
	{
		Release();
		m_pData = pData.m_pData;
		pData.m_pData = nullptr;
		return *this;
	}

	bool operator==(const GScopePointer& pData) const
	{
		return m_pData == pData.m_pData;
	}
	bool operator==(const _Ty* pData) const
	{
		return m_pData == pData;
	}
	bool operator==(std::nullptr_t)
	{
		return m_pData == nullptr;
	}
	bool operator!=(const GScopePointer& pData) const
	{
		return m_pData != pData.m_pData;
	}
	bool operator!=(const _Ty* pData) const
	{
		return m_pData != pData;
	}
	bool operator!=(std::nullptr_t)
	{
		return m_pData != nullptr;
	}

	_Ty& operator*()
	{
		return *m_pData;
	}
	const _Ty& operator*() const
	{
		return *m_pData;
	}

	_Ty* operator->() const
	{
		return m_pData;
	}

	_Ty* get() const
	{
		return m_pData;
	}

	void AddRef()
	{
		if(m_pData)
			m_pData->AddRef();
	}

	bool Release()
	{
		bool result = false;
		if (m_pData)
		{
			result = m_pData->Release();
			if (result)
				m_pData = nullptr;
		}
		return result;
	}

	template<typename _valTy>
	GScopePointer& operator<<(_valTy val)
	{
		m_pData->operator<<(val);
		return *this;
	}

	template<typename _valTy>
	const GScopePointer& operator>>(_valTy& val)
	{
		m_pData->operator>>(val);
		return *this;
	}

	friend class std::set<GScopePointer<_Ty>>;
	template<typename _Ty> friend class GScopePointer;
private:
	_Ty* m_pData;

	bool operator<(const GScopePointer& pData) const
	{
		return m_pData < pData.m_pData;
	}
	bool operator<=(const GScopePointer& pData) const
	{
		return m_pData <= pData.m_pData;
	}
	bool operator>(const GScopePointer& pData) const
	{
		return m_pData > pData.m_pData;
	}
	bool operator>=(const GScopePointer& pData) const
	{
		return m_pData >= pData.m_pData;
	}
};
#endif

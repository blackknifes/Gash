#ifndef __GARRAY_H__
#define __GARRAY_H__
#include <stdexcept>
#include <initializer_list>

template<typename _Ty>
class GArray
{
public:
	GArray(size_t size)
		:m_pArrayData(new _Ty[size]), m_size(size)
	{

	}
	GArray(std::initializer_list<_Ty> vals)
		:m_pArrayData(new _Ty[vals.size()]), m_size(vals.size())
	{
		for (size_t i = 0; i < m_size; ++i)
			m_pArrayData[i] = *(vals.begin() + i);
	}
	GArray(_Ty* pArray, size_t size)
		::m_pArrayData(pArray), m_size(size)
	{

	}
	~GArray()
	{
		if(m_pArrayData)
			delete[] m_pArrayData;
	}

	GArray(const GArray& _array)
		:m_pArrayData(new _Ty[_array.m_size]), m_size(_array.m_size)
	{
		for (size_t i = 0; i < m_size; ++i)
			m_pArrayData[i] = _array[i];
	}
	GArray(GArray&& _array)
		:m_pArrayData(_array.m_pArrayData), m_size(_array.m_size)
	{
		_array.m_pArrayData = nullptr;
		_array.m_size = 0;
	}

	_Ty& operator[](size_t index)
	{
		while (index >= m_size)
			throw std::out_of_range("");
		return m_pArrayData[index];
	}
	const _Ty& operator[](size_t index) const
	{
		while (index >= m_size)
			throw std::out_of_range("");
		return m_pArrayData[index];
	}
	_Ty* begin()
	{
		return m_pArrayData;
	}
	_Ty* end()
	{
		return m_pArrayData + m_size;
	}
	const _Ty* begin() const
	{
		return m_pArrayData;
	}
	const _Ty* end() const
	{
		return m_pArrayData + m_size;
	}

	size_t size() const
	{
		return m_size;
	}

	size_t length() const
	{
		return m_size;
	}

	bool empty() const
	{
		return m_size == 0;
	}
private:
	_Ty* m_pArrayData;
	size_t m_size;
};
#endif
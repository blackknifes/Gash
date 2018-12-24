#ifndef __GENUMS_H__
#define __GENUMS_H__

#define GEnumsDef(_enum) typedef GEnums<_enum> _enum##s;

template<typename _enumTy>
class GEnums
{
public:
	GEnums()
		:m_value(0)
	{}
	GEnums(_enumTy val)
		:m_value(val)
	{

	}
	GEnums(unsigned int val)
		:m_value(val)
	{

	}

	operator unsigned int&()
	{
		return m_value;
	}

	operator _enumTy() const
	{
		return (_enumTy)m_value;
	}
	operator unsigned int() const
	{
		return m_value;
	}
	explicit operator unsigned long() const
	{
		return m_value;
	}
private:
	unsigned int m_value;
};
#endif
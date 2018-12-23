#include "GStringPieceIterator.h"
#include <stdexcept>
#include <assert.h>

GStringPieceIterator::GStringPieceIterator(const GStringPieceIterator& itor)
	:m_type(itor.m_type)
{
	switch (itor.m_type)
	{
	case Type::Iterator:
		new (&m_iterator) const_iterator(itor.m_iterator);
		break;
	case Type::CharPointer:
		m_pCharPointer = itor.m_pCharPointer;
		break;
	default:
		break;
	}
}

GStringPieceIterator::GStringPieceIterator(const_iterator itor)
	: m_iterator(itor), m_type(Type::Iterator)
{

}

GStringPieceIterator::GStringPieceIterator(const wchar_t* _begin, const wchar_t* _end)
	: m_type(Type::CharPointer)
{
	m_pCharPointer.pCurrent = m_pCharPointer.pBegin = _begin;
	m_pCharPointer.pEnd = _end;
}

GStringPieceIterator::GStringPieceIterator(const wchar_t* _begin, size_t size)
	: m_type(Type::CharPointer)
{
	m_pCharPointer.pCurrent = m_pCharPointer.pBegin = _begin;
	m_pCharPointer.pEnd = _begin + size;
}

GStringPieceIterator::GStringPieceIterator(const wchar_t* _begin, const wchar_t* _end, const wchar_t* _cur)
	: m_type(Type::CharPointer)
{
	m_pCharPointer.pCurrent = _cur;
	m_pCharPointer.pBegin = _begin;
	m_pCharPointer.pEnd = _end;
}

GStringPieceIterator::GStringPieceIterator()
{
	reset();
}

bool GStringPieceIterator::checkCompatiable(const GStringPieceIterator& itor) const
{
	if (m_type != itor.m_type && m_type != Unknown)
	{
		throw std::runtime_error("iterator is not compatiable.");
		return false;
	}
	return true;
}

GStringPieceIterator::~GStringPieceIterator()
{
	reset();
}

wchar_t GStringPieceIterator::operator*() const
{
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		return *m_iterator;
	case GStringPieceIterator::CharPointer:
		return *m_pCharPointer.pCurrent;
	default:
		break;
	}
	return 0;
}

GStringPieceIterator& GStringPieceIterator::operator--()
{
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		--m_iterator;
		break;
	case GStringPieceIterator::CharPointer:
		if (m_pCharPointer.pCurrent == m_pCharPointer.pBegin)
			throw std::range_error("out of range");
		else
			--m_pCharPointer.pCurrent;
		break;
	default:
		break;
	}
	return *this;
}

GStringPieceIterator GStringPieceIterator::operator--(int)
{
	return GStringPieceIterator::operator--();
}

const GStringPieceIterator& GStringPieceIterator::operator-=(size_t offset)
{
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		m_iterator -= offset;
		break;
	case GStringPieceIterator::CharPointer:
		if (m_pCharPointer.pCurrent < m_pCharPointer.pBegin + offset)
		{
			throw std::range_error("out of range");
			m_pCharPointer.pCurrent = m_pCharPointer.pBegin;
		}
		else
			m_pCharPointer.pCurrent -= offset;
		break;
	default:
		break;
	}
	return *this;
}

GStringPieceIterator GStringPieceIterator::operator-(size_t offset) const
{
	GStringPieceIterator itor = *this;
	return itor -= offset;
}

size_t GStringPieceIterator::operator-(const GStringPieceIterator& itor) const
{
	if (!checkCompatiable(itor))
		return 0;
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		return m_iterator - itor.m_iterator;
	case GStringPieceIterator::CharPointer:
		return m_pCharPointer.pCurrent - itor.m_pCharPointer.pCurrent;
	default:
		break;
	}
	return 0;
}

void GStringPieceIterator::reset()
{
	if (m_type == Type::Iterator)
		m_iterator.~const_iterator();
	m_pCharPointer.pBegin = m_pCharPointer.pEnd = m_pCharPointer.pCurrent = nullptr;
	m_type = Type::Unknown;
}

GStringPieceIterator::const_iterator GStringPieceIterator::toStdIterator() const
{
	return m_iterator;
}

const wchar_t* GStringPieceIterator::toCharPointer() const
{
	assert(m_type == Type::CharPointer);
	return m_pCharPointer.pCurrent;
}

GStringPieceIterator& GStringPieceIterator::operator++()
{
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		++m_iterator;
		break;
	case GStringPieceIterator::CharPointer:
		if (m_pCharPointer.pCurrent == m_pCharPointer.pEnd)
			throw std::range_error("out of range");
		else
			++m_pCharPointer.pCurrent;
		break;
	default:
		break;
	}
	return *this;
}

GStringPieceIterator GStringPieceIterator::operator++(int)
{
	return GStringPieceIterator::operator++();
}

const GStringPieceIterator& GStringPieceIterator::operator+=(size_t offset)
{
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		m_iterator += offset;
		break;
	case GStringPieceIterator::CharPointer:
		if (m_pCharPointer.pCurrent + offset > m_pCharPointer.pEnd)
		{
			throw std::range_error("out of range");
			m_pCharPointer.pCurrent = m_pCharPointer.pEnd;
		}
		else
			m_pCharPointer.pCurrent += offset;
		break;
	default:
		break;
	}
	return *this;
}

GStringPieceIterator GStringPieceIterator::operator+(size_t offset) const
{
	GStringPieceIterator itor = *this;
	return itor += offset;
}

bool GStringPieceIterator::operator!=(const GStringPieceIterator& itor) const
{
	return !GStringPieceIterator::operator==(itor);
}

bool GStringPieceIterator::operator==(const GStringPieceIterator& itor) const
{
	if (!checkCompatiable(itor))
		return false;
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		return m_iterator == itor.m_iterator;
	case GStringPieceIterator::CharPointer:
		return m_pCharPointer.pCurrent == itor.m_pCharPointer.pCurrent;
	default:
		break;
	}
	return false;
}

const GStringPieceIterator& GStringPieceIterator::operator=(const GStringPieceIterator& itor)
{
	reset();
	if (!checkCompatiable(itor))
		return *this;
	if (m_type == Type::Unknown)
		m_type = itor.m_type;
	switch (m_type)
	{
	case GStringPieceIterator::Iterator:
		m_iterator = itor.m_iterator;
		break;
	case GStringPieceIterator::CharPointer:
		m_pCharPointer = itor.m_pCharPointer;
		break;
	default:
		break;
	}
	return *this;
}

#ifndef __GSTRINGPIECEITERATOR_H__
#define __GSTRINGPIECEITERATOR_H__
#include <string>

class GStringPiece;

class GStringPieceIterator
{
public:
	typedef typename std::wstring::const_iterator const_iterator;

	GStringPieceIterator(const GStringPieceIterator& itor);
	~GStringPieceIterator();

	const GStringPieceIterator& operator=(const GStringPieceIterator& itor);
	bool operator==(const GStringPieceIterator& itor) const;
	bool operator!=(const GStringPieceIterator& itor) const;
	wchar_t operator*() const;

	const GStringPieceIterator& operator+=(size_t offset);
	const GStringPieceIterator& operator-=(size_t offset);
	GStringPieceIterator operator+(size_t offset) const;
	GStringPieceIterator operator-(size_t offset) const;
	GStringPieceIterator& operator++();
	GStringPieceIterator operator++(int);
	GStringPieceIterator& operator--();
	GStringPieceIterator operator--(int);

	size_t operator-(const GStringPieceIterator& itor) const;

	void reset();

	const_iterator toStdIterator() const;
	const wchar_t* toCharPointer() const;

	friend class GStringPiece;
private:
	GStringPieceIterator();
	GStringPieceIterator(const_iterator itor);
	GStringPieceIterator(const wchar_t* _begin, const wchar_t* _end);
	GStringPieceIterator(const wchar_t* _begin, size_t size);
	GStringPieceIterator(const wchar_t* _begin, const wchar_t* _end, const wchar_t* _cur);
	bool checkCompatiable(const GStringPieceIterator& itor) const;

	enum Type
	{
		Unknown = 0,
		Iterator,
		CharPointer
	};
	union
	{
		const_iterator m_iterator;
		struct
		{
			const wchar_t* pCurrent;
			const wchar_t* pBegin;
			const wchar_t* pEnd;
		} m_pCharPointer;
	};
	Type m_type;
};
#endif

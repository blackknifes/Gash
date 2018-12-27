#ifndef __GHASHCONST_H__
#define __GHASHCONST_H__

typedef unsigned __int64 hash_t;

static constexpr hash_t prime = 0x100000001B3ull;
static constexpr hash_t basis = 0xCBF29CE484222325ull;

namespace internal
{
	static constexpr hash_t ghash_(const char* str, hash_t last_value = basis)
	{
		return *str ? ghash_(str + 1, (*str ^ last_value) * prime) : last_value;
	}
}

static hash_t GHashString(const char* str)
{
	hash_t ret{ basis };

	while (*str) {
		ret ^= *str;
		ret *= prime;
		str++;
	}

	return ret;
}

static constexpr hash_t operator "" _hash(char const* p, size_t)
{
	return internal::ghash_(p);
}
#endif
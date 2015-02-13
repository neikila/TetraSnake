#include "NonClassFunction.h"

size_t CountSym(const char *string, char Sym)
{
	size_t count = 0;
	while (*string)
		if (*string++ == Sym)
			++count;
	return count;
}

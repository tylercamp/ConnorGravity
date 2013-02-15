
#ifndef _ORGUI_HASH_H_
#define _ORGUI_HASH_H_

#include <ORE/Types.h>


/* Generic hashing algorithms; they're not very fancy or secure, but they don't really need to be */


namespace gui
{
	inline ore::t_UInt64 Hash (ore::t_Byte * data, int size);
	inline ore::t_UInt64 Hash (ore::t_String text);
}



namespace gui
{
	inline ore::t_UInt64 Hash (ore::t_Byte * data, int size)
	{
		int result = 0;
		for (int i = 0; i < size; i++)
		{
			result ^= ((int)data [i]) << ((i%4)*8);
		}
		return result;
	}

	inline ore::t_UInt64 Hash (ore::t_String text)
	{
		return Hash ((ore::t_Byte *)text.c_str (), text.size ());
	}
}

#endif
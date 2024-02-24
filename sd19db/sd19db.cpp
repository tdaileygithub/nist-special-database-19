#include "sd19db.h"

namespace sdb19db
{	
	int IHead::Create() const
	{
		return SQLITE_OK;
	}

	int IHead::Insert(const tables::ihead& table) const
	{
		return 10;
	}
}
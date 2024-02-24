#include "dbmanager.h"

namespace sdb19db
{
	void DbManager::Setup() {
		_ihead->Create();
	}

	int DbManager::Insert(const tables::ihead& table) const
	{
		return _ihead->Insert(table);
	}
}
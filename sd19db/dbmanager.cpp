#include "dbmanager.h"

namespace sdb19db
{
	void DbManager::Setup() {
		_ihead->Create();
		_hsfpage->Create();
	}

	int DbManager::Insert(const tables::ihead& table) const
	{
		return _ihead->Insert(table);
	}

	int DbManager::Insert(const tables::hsfpage& table) const
	{
		return _hsfpage->Insert(table);
	}
}
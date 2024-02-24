#pragma once

#include <memory>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "ihead.h"

namespace sdb19db
{
	class DbManager
	{
	private:
		const std::string _dbname;
		std::unique_ptr<IHead> _ihead;

	public:
		DbManager(const std::string& dbname) : _dbname(dbname) {
			_ihead = std::make_unique<IHead>(dbname);
		}
		void Setup();

		int Insert(const tables::ihead& table) const;
	};
}
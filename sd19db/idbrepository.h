#pragma once

#include <memory>
#include <string>

#include "../sqlite3/sqlite3.h"

namespace sdb19db
{
	template<class T>
	class IDbRepository
	{
	protected:
		std::shared_ptr<sqlite3> db;
		const std::string _lastRowSql = "SELECT last_insert_rowid();";

		//std::unique_ptr<sqlite3, sqlite3_deleter> _db;
		sqlite3* _dbPtr = NULL;
	public:
		IDbRepository(const std::string& dbname) {
			sqlite3_open(dbname.c_str(), &_dbPtr);
			db.reset(_dbPtr, sqlite3_close);
		}
		//virtual ~IBaseDb() = default;
		virtual int Create() const = 0;
		virtual int Insert(const T& table) const = 0;
		virtual int LastRowId() const = 0;
	};

}
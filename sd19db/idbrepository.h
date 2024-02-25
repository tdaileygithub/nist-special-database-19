#pragma once

#include <memory>
#include <string>
#include <iostream>

#include "../sqlite3/sqlite3.h"

namespace sdb19db
{
	template<class T>
	class IDbRepository
	{
	protected:
		sqlite3_stmt* _insertStatement;

		std::shared_ptr<sqlite3> _db;

		const std::string _lastRowSql = "SELECT last_insert_rowid();";
		
		sqlite3* _dbPtr = NULL;
	public:
		IDbRepository(
			const std::string& dbname, 
			const std::string createSql,
			const std::string insertSql)
		{
			sqlite3_open(dbname.c_str(), &_dbPtr);
			_db.reset(_dbPtr, sqlite3_close);

			char* messaggeError;
			if (SQLITE_OK != sqlite3_exec(_dbPtr, createSql.c_str(), NULL, 0, &messaggeError)) {
				std::cerr << "Error Create Table" << std::endl;
				sqlite3_free(messaggeError);
			}
			if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, insertSql.c_str(), insertSql.size(), &_insertStatement, nullptr)) {
				std::cerr << insertSql << std::endl;
				sqlite3_close(_dbPtr);			
				exit(1);
			}
		}		
		virtual int Insert(const T& table) const = 0;
		virtual int LastRowId() const = 0;
	};

}
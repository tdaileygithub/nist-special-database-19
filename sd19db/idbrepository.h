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
		sqlite3_stmt* _lastRowStatement;

		std::shared_ptr<sqlite3> _db;
		
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

			const std::string lastRowSql("SELECT last_insert_rowid();");
			if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, lastRowSql.c_str(), lastRowSql.size(), &_lastRowStatement, nullptr)) {
				std::cerr << lastRowSql << std::endl;
				sqlite3_close(_dbPtr);
				exit(1);
			}
			if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, insertSql.c_str(), insertSql.size(), &_insertStatement, nullptr)) {
				std::cerr << insertSql << std::endl;
				sqlite3_close(_dbPtr);			
				exit(1);
			}
		}

		virtual int Insert(const T& table) const = 0;
		
		int LastRowId() const 
		{			
			int rc = sqlite3_step(_lastRowStatement);
			if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
				std::string errmsg(sqlite3_errmsg(_dbPtr));
				sqlite3_finalize(_lastRowStatement);
				std::cerr << errmsg;
				exit(1);
			}
			//completed but there was no row
			if (rc == SQLITE_DONE) {
				sqlite3_reset(_lastRowStatement);
				return -1;
			}
			rc = sqlite3_column_int(_lastRowStatement, 0);
			sqlite3_reset(_lastRowStatement);
			return rc;
		};
	};

}
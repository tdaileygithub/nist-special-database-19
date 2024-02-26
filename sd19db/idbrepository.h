#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
				sqlite3_close(_dbPtr);
				exit(1);
			}
			if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, insertSql.c_str(), insertSql.size(), &_insertStatement, nullptr)) {
				sqlite3_close(_dbPtr);
				exit(1);
			}
		}

		virtual int Insert(const T& table) const = 0;

		virtual std::vector<std::vector<std::string>> Query(const std::string sql) const
		{
			sqlite3_stmt* sqlstatement;			
			if (SQLITE_OK != sqlite3_prepare(_dbPtr, sql.c_str(), -1, &sqlstatement, NULL)) {
				sqlite3_close(_dbPtr);
				exit(1);
			}
			std::vector<std::vector<std::string>> rows(0, std::vector<std::string>(0));
			int ncols = sqlite3_column_count(sqlstatement);
			while (sqlite3_step(sqlstatement) == SQLITE_ROW)
			{
				std::vector<std::string> cols;
				for (int i = 0; i < ncols; i++)
				{
					cols.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(sqlstatement, i))));
				}
				rows.push_back(cols);
			}
			sqlite3_finalize(sqlstatement);
			return rows;
		};
		
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
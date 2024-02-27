#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../sqlite3/sqlite3.h"

#include "../ulog/ulog.h"

namespace sdb19db
{
	template<class T>
	class IDbRepository
	{
	protected:
		sqlite3_stmt* _insertStatement;
		sqlite3_stmt* _lastRowStatement;

		//std::shared_ptr<sqlite3> _db;
		
		sqlite3* _dbPtr = NULL;

		void HandleSqliteError(const int ResultCode) const {
			sqlite3_close(_dbPtr);
			std::cerr << std::endl << "ResultCode: " << ResultCode << " " << sqlite3_errstr(ResultCode) << std::endl;
			exit(ResultCode);
		}

		void Log(const std::string& msg) const {
			//ulog.val("IDbRepository:", msg);
		}

	public:
		IDbRepository(
			sqlite3* db,
			const std::string createSql,
			const std::string insertSql)
			: _dbPtr(db)
		{
			//int rc = sqlite3_open(dbname.c_str(), &_dbPtr);
			//if (SQLITE_OK != rc) {
			//	HandleSqliteError(rc);
			//}
			//_db.reset(_dbPtr, sqlite3_close);

			char* messaggeError;
			int rc = sqlite3_exec(_dbPtr, createSql.c_str(), NULL, 0, &messaggeError);
			if (SQLITE_OK != rc) {
				exit(9999);
				//std::cerr << "Error Create Table" << std::endl;
				sqlite3_free(messaggeError);
			}
			
			const std::string lastRowSql("SELECT last_insert_rowid();");
			rc = sqlite3_prepare_v2(_dbPtr, lastRowSql.c_str(), lastRowSql.size(), &_lastRowStatement, nullptr);
			if (SQLITE_OK != rc) {								
				HandleSqliteError(rc);
			}
			rc = sqlite3_prepare_v2(_dbPtr, insertSql.c_str(), insertSql.size(), &_insertStatement, nullptr);
			if (SQLITE_OK != rc) {
				HandleSqliteError(rc);
			}
		}

		virtual int Insert(const T& table) const = 0;

		virtual std::vector<std::vector<std::string>> Query(const std::string sql) const
		{
			sqlite3_stmt* sqlstatement;			
			int rc = sqlite3_prepare_v2(_dbPtr, sql.c_str(), sql.size(), &sqlstatement, nullptr);
			if (SQLITE_OK != rc) {
				
				std::cerr << sql << " rc = " << rc << std::endl;				
				//HandleSqliteError(rc);
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
			rc = sqlite3_finalize(sqlstatement);
			if (SQLITE_OK != rc) {				
				HandleSqliteError(rc);
			}
			return rows;
		};
		
		int LastRowId() const 
		{			
			int rc = sqlite3_step(_lastRowStatement);
			if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
				HandleSqliteError(rc);
			}
			//completed but there was no row
			if (rc == SQLITE_DONE) {
				sqlite3_reset(_lastRowStatement);
				return -1;
			}
			int ret = sqlite3_column_int(_lastRowStatement, 0);
			rc = sqlite3_reset(_lastRowStatement);
			if (SQLITE_OK != rc) {
				HandleSqliteError(rc);
			}
			return ret;
		};
	};

}
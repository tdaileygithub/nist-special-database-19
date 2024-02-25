#include <iostream>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "hsfpage.h"

namespace sdb19db
{
	int HsfPage::LastRowId() const {

		sqlite3_stmt* stmt = nullptr;
		if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, _lastRowSql.c_str(), _lastRowSql.size(), &stmt, nullptr)) {
			sqlite3_close(_dbPtr);
			exit(1);
		}
		int rc = sqlite3_step(stmt);

		if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
			std::string errmsg(sqlite3_errmsg(_dbPtr));
			sqlite3_finalize(stmt);
			std::cerr << errmsg;
			exit(1);
		}
		//completed but there was no row
		if (rc == SQLITE_DONE) {
			sqlite3_finalize(stmt);
			return -1;
		}
		return  sqlite3_column_int(stmt, 0);
	}

	int HsfPage::Create() const {

		const std::string sql = R"SQL(
			CREATE TABLE "hsf_page" (
				"id"				INTEGER NOT NULL,
				"hsf_num"			INTEGER NOT NULL,
				"ihead_id"			INTEGER NOT NULL,
				"writer_num"		INTEGER NOT NULL,
				"template_num"		INTEGER NOT NULL,
				"jpeg"				BLOB NOT NULL,
				FOREIGN KEY (ihead_id) REFERENCES ihead (id),
				PRIMARY KEY("id" AUTOINCREMENT)
			);
		)SQL";

		char* messaggeError;
		if (SQLITE_OK != sqlite3_exec(_dbPtr, sql.c_str(), NULL, 0, &messaggeError)) {
			std::cerr << "Error Create Table" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table created Successfully" << std::endl;

		return SQLITE_OK;
	}

	int HsfPage::Insert(const tables::hsfpage& table) const {
		const std::string sql = R"SQL(
			INSERT INTO hsf_page(hsf_num,ihead_id,writer_num,template_num,jpeg) VALUES (?,?,?,?,?);
		)SQL";

		sqlite3_stmt* stmt = nullptr;
		if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, sql.c_str(), sql.size(), &stmt, nullptr)) {
			sqlite3_close(_dbPtr);
			std::cerr << "yo";
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 1, table.hsf_num)) {
			std::cerr << "yo1";
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 2, table.ihead_id)) {
			std::cerr << "yo2";
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 3, table.writer_num)) {
			std::cerr << "yo3";
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 4, table.template_num)) {
			std::cerr << "yo4";
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_blob(stmt, 5, table.buffer, table.buffer_len_bytes, SQLITE_STATIC)) {
			std::cerr << "yo5";
			exit(1);
		}
		if (SQLITE_DONE != sqlite3_step(stmt)) {
			std::cerr << "yo6";
			exit(1);
		}
		//std::cerr << "ppp" <<std::endl;
		sqlite3_finalize(stmt);
		//std::cerr << "qqq" << std::endl;
		//sqlite3_reset(stmt);
		
		return LastRowId();
	}
}

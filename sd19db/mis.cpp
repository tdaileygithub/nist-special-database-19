#include <iostream>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "mis.h"

namespace sdb19db
{
	int Mis::LastRowId() const {

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
		rc = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		return rc;
	}

	int Mis::Create() const {

		const std::string sql = R"SQL(
			CREATE TABLE "mis" (
				"id"				INTEGER NOT NULL,
				"hsf_num"			INTEGER NOT NULL,
				"ihead_id"			INTEGER NOT NULL,
				"writer_num"		INTEGER NOT NULL,
				"template_num"		INTEGER NOT NULL,
				"character"			TEXT NOT NULL,
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
		return SQLITE_OK;
	}

	int Mis::Insert(const tables::mis& table) const {
		const std::string sql = R"SQL(
			INSERT INTO mis(hsf_num,ihead_id,writer_num,template_num,character,jpeg) VALUES (?,?,?,?,?,?);
		)SQL";

		sqlite3_stmt* stmt = nullptr;
		if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, sql.c_str(), sql.size(), &stmt, nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 1, table.hsf_num)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 2, table.ihead_id)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 3, table.writer_num)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 4, table.template_num)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_text(stmt, 5, table.character.c_str(), table.character.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_blob(stmt, 6, table.jpeg, table.jpeg_len_bytes, SQLITE_STATIC)) {
			exit(1);
		}
		if (SQLITE_DONE != sqlite3_step(stmt)) {
			exit(1);
		}
		sqlite3_finalize(stmt);

		return LastRowId();
	}
}

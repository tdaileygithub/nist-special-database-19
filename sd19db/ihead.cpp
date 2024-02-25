#include <iostream>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "ihead.h"

namespace sdb19db
{
	int IHead::LastRowId() const {

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

	int IHead::Create() const {

		const std::string sql = R"SQL(
			CREATE TABLE "ihead" (
				"id"			INTEGER NOT NULL,
				"created"		TEXT NOT NULL,
				"width"			INTEGER NOT NULL,
				"height"		INTEGER NOT NULL,
				"depth"			INTEGER NOT NULL,
				"density"		INTEGER NOT NULL,
				"compress"		INTEGER NOT NULL,
				"complen"		INTEGER NOT NULL,
				"align"			INTEGER NOT NULL,
				"unitsize"		INTEGER NOT NULL,
				"sigbit"		INTEGER NOT NULL,
				"byte_order"	INTEGER NOT NULL,
				"pix_offset"	INTEGER NOT NULL,
				"whitepix"		INTEGER NOT NULL,
				"issigned"		INTEGER NOT NULL,
				"rm_cm"			INTEGER NOT NULL,
				"tb_bt"			INTEGER NOT NULL,
				"lr_rl"			INTEGER NOT NULL,
				"parent"		TEXT NOT NULL,
				"par_x"			INTEGER NOT NULL,
				"par_y"			INTEGER NOT NULL,
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

	int IHead::Insert(const tables::ihead& table) const {
		const std::string sql = R"SQL(
			INSERT INTO ihead(created,width,height,depth,density,compress,complen,align,unitsize,sigbit,byte_order,pix_offset,whitepix,issigned,rm_cm,tb_bt,lr_rl,parent,par_x,par_y) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);
		)SQL";

		sqlite3_stmt* stmt = nullptr;		
		if (SQLITE_OK != sqlite3_prepare_v2(_dbPtr, sql.c_str(), sql.size(), &stmt, nullptr)) {
			sqlite3_close(_dbPtr);			
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_text(stmt, 1, table.created.c_str(), table.created.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 2, table.width)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 3, table.height)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 4, table.depth)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 5, table.density)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 6, table.compress)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 7, table.complen)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 8, table.align)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 9, table.unitsize)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 10, table.sigbit)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 11, table.byte_order)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 12, table.pix_offset)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 13, table.whitepix)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 14, table.issigned)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 15, table.rm_cm)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 16, table.tb_bt)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 17, table.lr_rl)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_text(stmt, 18, table.parent.c_str(), table.parent.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 19, table.par_x)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(stmt, 20, table.par_y)) {
			exit(1);
		}
		if (SQLITE_DONE != sqlite3_step(stmt)) {
			exit(1);
		}
		sqlite3_finalize(stmt);

		return LastRowId();
	}
}

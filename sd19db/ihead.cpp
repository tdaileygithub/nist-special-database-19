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

	int IHead::Insert(const tables::ihead& table) const {
		if (SQLITE_OK != sqlite3_bind_text(_insertStatement, 1, table.created.c_str(), table.created.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 2, table.width)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 3, table.height)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 4, table.depth)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 5, table.density)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 6, table.compress)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 7, table.complen)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 8, table.align)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 9, table.unitsize)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 10, table.sigbit)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 11, table.byte_order)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 12, table.pix_offset)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 13, table.whitepix)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 14, table.issigned)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 15, table.rm_cm)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 16, table.tb_bt)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 17, table.lr_rl)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_text(_insertStatement, 18, table.parent.c_str(), table.parent.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 19, table.par_x)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 20, table.par_y)) {
			exit(1);
		}
		if (SQLITE_DONE != sqlite3_step(_insertStatement)) {
			exit(1);
		}
		sqlite3_reset(_insertStatement);		

		return LastRowId();
	}
}

#include <iostream>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "hsfpage.h"

namespace sdb19db
{
	int HsfPage::Insert(const tables::hsfpage& table) const {
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 1, table.hsf_num)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 2, table.ihead_id)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 3, table.writer_num)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_int(_insertStatement, 4, table.template_num)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_blob(_insertStatement, 5, table.buffer, table.buffer_len_bytes, SQLITE_STATIC)) {
			exit(1);
		}
		if (SQLITE_DONE != sqlite3_step(_insertStatement)) {
			exit(1);
		}
		sqlite3_reset(_insertStatement);
		
		return LastRowId();
	}
}

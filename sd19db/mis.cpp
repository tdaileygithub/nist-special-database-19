#include <iostream>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "mis.h"

namespace sdb19db
{
	int Mis::Insert(const tables::mis& table) const {
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
		if (SQLITE_OK != sqlite3_bind_text(_insertStatement, 5, table.character.c_str(), table.character.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_blob(_insertStatement, 6, table.image, table.image_len_bytes, SQLITE_STATIC)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_text(_insertStatement, 7, table.image_sha256.c_str(), table.image_sha256.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_OK != sqlite3_bind_text(_insertStatement, 8, table.mis_sha256.c_str(), table.mis_sha256.size(), nullptr)) {
			exit(1);
		}
		if (SQLITE_DONE != sqlite3_step(_insertStatement)) {
			exit(1);
		}
		sqlite3_reset(_insertStatement);

		return LastRowId();
	}
}

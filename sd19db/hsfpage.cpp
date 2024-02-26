#include <iostream>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "hsfpage.h"

namespace sdb19db
{
	int HsfPage::Insert(const tables::hsfpage& table) const {
		int rc = sqlite3_bind_int(_insertStatement, 1, table.hsf_num);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 2, table.ihead_id);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 3, table.writer_num);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 4, table.template_num);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_blob(_insertStatement, 5, table.image, table.image_len_bytes, SQLITE_STATIC);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_text(_insertStatement, 6, table.image_sha256.c_str(), table.image_sha256.size(), nullptr);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_text(_insertStatement, 7, table.hsf_page_sha256.c_str(), table.hsf_page_sha256.size(), nullptr);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_step(_insertStatement);
		if (SQLITE_DONE != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_reset(_insertStatement);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		return LastRowId();
	}
}

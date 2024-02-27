#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "../sqlite3/sqlite3.h"

#include "ihead.h"

namespace sdb19db
{
	int IHead::Insert(const tables::ihead& table) const {

		int rc = sqlite3_bind_text(_insertStatement, 1, table.created.c_str(), table.created.size(), nullptr);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}		
		rc = sqlite3_bind_int(_insertStatement, 2, table.width);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 3, table.height);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 4, table.depth);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 5, table.density);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 6, table.compress);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 7, table.complen);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 8, table.align);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 9, table.unitsize);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 10, table.sigbit);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 11, table.byte_order);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 12, table.pix_offset);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 13, table.whitepix);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 14, table.issigned);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 15, table.rm_cm);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 16, table.tb_bt);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 17, table.lr_rl);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_text(_insertStatement, 18, table.parent.c_str(), table.parent.size(), nullptr);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 19, table.par_x);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		rc = sqlite3_bind_int(_insertStatement, 20, table.par_y);
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

	std::ostream& operator<<(std::ostream& out, const tables::ihead& ihead_row)
	{	
		out << std::endl
			<< std::setw(20) << "created:"	<< std::setw(65) << ihead_row.created	<< std::endl
			<< std::setw(20) << "width:"	<< std::setw(65) << ihead_row.width		<< std::endl
			<< std::setw(20) << "height:"	<< std::setw(65) << ihead_row.height	<< std::endl;
		return out;
	}

	std::string to_string(const tables::ihead& h)
	{
		std::ostringstream ss;
		ss << h;
		return ss.str();
	}
}

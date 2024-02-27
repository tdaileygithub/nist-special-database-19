#include <format>

#include "dbmanager.h"

namespace sdb19db
{
	int DbManager::Insert(const tables::ihead& table) const
	{
		return _ihead->Insert(table);
	}

	int DbManager::Insert(const tables::hsfpage& table) const
	{
		return _hsfpage->Insert(table);
	}

	int DbManager::Insert(const tables::mis& table) const
	{
		return _mis->Insert(table);
	}

	bool DbManager::HsfPageProcessed(const std::string sha256) const {
		const std::string sql(std::format("select id from hsf_page where hsf_page_sha256 = '{}';", sha256));
		const auto ret = _hsfpage->Query(sql);
		//for (auto it = ret.begin(); it < ret.end(); it++) {
		//	for (auto colsit = it->begin(); colsit < it->end(); colsit++) {
		//		std::cerr << *colsit << std::endl;
		//	}
		//}
		return (1 == ret.size());
	}

	bool DbManager::MisProcessed(const std::string sha256) const {
		const std::string sql(std::format("select id from mis where mis_sha256 = '{}';", sha256));
		const auto ret = _hsfpage->Query(sql);
		//multiple mis images are present inside each mis file
		return (ret.size() >= 1);
	}

	void DbManager::WriteInMemoryToDisk(const std::string backupFilename) const {
		int rc;
		sqlite3* pFile;
		sqlite3_backup* pBackup;

		rc = sqlite3_open(backupFilename.c_str(), &pFile);
		if (rc == SQLITE_OK) {
			pBackup = sqlite3_backup_init(pFile, "main", _dbPtr, "main");
			if (pBackup) {
				(void)sqlite3_backup_step(pBackup, -1);
				(void)sqlite3_backup_finish(pBackup);
			}
		}
		(void)sqlite3_close(pFile);
	}

	int DbManager::GetMisCharacterCount(std::string character) const {
		const std::string sql(std::format("select count(*) from mis where character='{}';", character));
		const auto ret = _hsfpage->Query(sql);		
		return std::stoi(ret.at(0).at(0));
	}

	int DbManager::GetMisCharacterCount(int field_type, int hsf_num) const {
		const std::string sql(std::format("select count(*) from mis where field_type={} and hsf_num={};", field_type, hsf_num));
		const auto ret = _hsfpage->Query(sql);
		return std::stoi(ret.at(0).at(0));
	}
}
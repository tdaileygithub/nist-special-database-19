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
		return (1 == ret.size());
	}
}
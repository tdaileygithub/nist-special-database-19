#pragma once

#include <memory>
#include <string>

#include "hsfpage.h"
#include "ihead.h"
#include "mis.h"

namespace sdb19db
{
	class DbManager
	{
	private:
		const std::string _dbname;
		std::unique_ptr<IHead> _ihead;
		std::unique_ptr<HsfPage> _hsfpage;
		std::unique_ptr<Mis> _mis;

	public:
		DbManager(const std::string& dbname) : _dbname(dbname) {
			_ihead = std::make_unique<IHead>(dbname);
			_hsfpage = std::make_unique<HsfPage>(dbname);
			_mis = std::make_unique<Mis>(dbname);
		}

		int Insert(const tables::ihead& table) const;
		int Insert(const tables::hsfpage& table) const;
		int Insert(const tables::mis& table) const;

		bool HsfPageProcessed(const std::string sha256) const;

		bool MisProcessed(const std::string sha256) const;
	};
}
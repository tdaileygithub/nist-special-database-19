#pragma once

#include <memory>
#include <string>

#include "hsfpage.h"
#include "ihead.h"

namespace sdb19db
{
	class DbManager
	{
	private:
		const std::string _dbname;
		std::unique_ptr<IHead> _ihead;
		std::unique_ptr<HsfPage> _hsfpage;		

	public:
		DbManager(const std::string& dbname) : _dbname(dbname) {
			_ihead = std::make_unique<IHead>(dbname);
			_hsfpage = std::make_unique<HsfPage>(dbname);
		}
		void Setup();

		int Insert(const tables::ihead& table) const;
		int Insert(const tables::hsfpage& table) const;
	};
}
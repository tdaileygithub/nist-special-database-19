#pragma once

#include <memory>
#include <string>

#include "../sqlite3/sqlite3.h"

namespace sdb19db 
{
	namespace tables {
		class ihead {
		public:
			int id;
		};
	}

	struct sqlite3_deleter {
		void operator()(sqlite3* db) {
			sqlite3_close_v2(db);
		}
	};

	template<class T>
	class IDbRepository 
	{
	private:
		std::shared_ptr<sqlite3> db;

		//{
		//	sqlite3* dbPtr = NULL;
		//	sqlite3_open(dbname.c_str(), &dbPtr);
		//	db.reset(dpPtr, sqlite3_close);
		//}

		//std::unique_ptr<sqlite3, sqlite3_deleter> _db;
		sqlite3* _dbPtr = NULL;
	public:
		IDbRepository(const std::string_view dbname) 
		{
			sqlite3_open(std::string(dbname).c_str(), &_dbPtr);
			db.reset(_dbPtr, sqlite3_close);
		}
		//virtual ~IBaseDb() = default;
		virtual int Create() const = 0;
		virtual int Insert(const T& table) const = 0;
	};
	
	class IHead : public IDbRepository<tables::ihead>	
	{
	public:
		int _a = 2;
		//IHead(const IHead&) = default;
		//IHead& operator=(const IHead&) = default;

		~IHead() {}
		IHead(const std::string_view dbname) : IDbRepository(dbname) {}
		
		int Create() const;
		int Insert(const tables::ihead& table) const;
	};

	template<class T>
	class DbFacade
	{
	public:
		//int Insert(const tables::ihead& table) const;
		//IHead IHead;
	};

}
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "hsfpage.h"
#include "ihead.h"
#include "mis.h"

#include "../sqlite3/sqlite3.h"

namespace sdb19db
{
	struct RawImageInfo {
		unsigned char* PixelData;
		unsigned int Width;
		unsigned int Height;
	};

	struct BlobData {
		unsigned char* Data;
		int Length;		
	};

	class DbManager
	{
	private:
		const std::string			_dbname;

		std::unique_ptr<IHead>		_ihead;
		std::unique_ptr<HsfPage>	_hsfpage;
		std::unique_ptr<Mis>		_mis;

		sqlite3* _dbPtr				= NULL;

		void HandleSqliteError(const int ResultCode) const {
			sqlite3_close(_dbPtr);
			std::cerr << std::endl << "ResultCode: " << ResultCode << " " << sqlite3_errstr(ResultCode) << std::endl;
			exit(ResultCode);
		}
	public:
		DbManager(const std::string& dbname) 
			: _dbname(dbname) 
		{
			int rc = sqlite3_open(_dbname.c_str(), &_dbPtr);
			if (SQLITE_OK != rc) {
				HandleSqliteError(rc);
			}
			_ihead	 = std::make_unique<IHead>(_dbPtr);
			_hsfpage = std::make_unique<HsfPage>(_dbPtr);
			_mis	 = std::make_unique<Mis>(_dbPtr);
		}

		int Insert(const tables::ihead& table) const;
		int Insert(const tables::hsfpage& table) const;
		int Insert(const tables::mis& table) const;

		bool HsfPageProcessed(const std::string sha256) const;

		bool MisProcessed(const std::string sha256) const;

		void WriteInMemoryToDisk(const std::string backupFilename) const;

		int GetMisCharacterCount(const std::string character) const;

		int GetMisCharacterCount(int field_type, int hsf_num) const;

		BlobData GetBlobData(const int id, const std::string table, std::string blob_column) const;

		std::vector<int> GetMisIds(const std::string character) const;
		
		RawImageInfo GetImageAsPng(const int id, const std::string table, std::string blob_column) const;
	};

	std::ostream& operator<<(std::ostream& out, const RawImageInfo& config);

	std::string to_string(const RawImageInfo& config);
}
#include <cstring>
#include <format>
#include <iomanip>
#include <sstream>
#include <string>

#include "dbmanager.h"

#include "../lodepng/lodepng.h"

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

	int DbManager::GetMisCharacterCount(const std::string character) const {
		const std::string sql(std::format("select count(*) from mis where character='{}';", character));
		const auto ret = _hsfpage->Query(sql);		
		return std::stoi(ret.at(0).at(0));
	}

	int DbManager::GetMisCharacterCount(int field_type, int hsf_num) const {
		const std::string sql(std::format("select count(*) from mis where field_type={} and hsf_num={};", field_type, hsf_num));
		const auto ret = _hsfpage->Query(sql);
		return std::stoi(ret.at(0).at(0));
	}

	std::vector<int> DbManager::GetMisIds(const std::string character) const {
		std::vector<int> ids;
		const std::string sql(std::format("select id from mis where character = '{}';", character));
		const auto ret = _hsfpage->Query(sql);
		for (auto it = ret.begin(); it < ret.end(); it++) {
			//return first column
			ids.push_back(std::stoi(it->at(0)));
		}
		return ids;
	}

	BlobData DbManager::GetBlobData(const int id, const std::string table, std::string blob_column) const
	{
		BlobData bd;
		bd.Length = 0;
		bd.Data = nullptr;

		sqlite3_stmt* sqlstatement;		
		const std::string sql(std::format("select id, LENGTH({}), {} from {} where id = '{}';", blob_column, blob_column, table, id));

		int rc = sqlite3_prepare_v2(_dbPtr, sql.c_str(), sql.size(), &sqlstatement, nullptr);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}		
		while (sqlite3_step(sqlstatement) == SQLITE_ROW)
		{
			auto id			= sqlite3_column_text(sqlstatement, 0);
			auto len		= sqlite3_column_int(sqlstatement, 1);
			auto imgdata	= (unsigned char*)sqlite3_column_blob(sqlstatement, 2);		
			bd.Length		= len;			
			bd.Data			= new unsigned char[len];			
			std::memcpy(bd.Data, imgdata, len);
		}
		rc = sqlite3_finalize(sqlstatement);
		if (SQLITE_OK != rc) {
			HandleSqliteError(rc);
		}
		//exit(55);
		return bd;
	};

	RawImageInfo DbManager::GetImageAsPng(const int id, const std::string table, std::string blob_column) const 
	{	
		RawImageInfo rai;
		rai.Id = id;
		BlobData bd = GetBlobData(id, table, blob_column);

		unsigned int width, height, error;
		unsigned char* imagepixels = nullptr;
		
		error = lodepng_decode_memory(&imagepixels, &rai.Width, &rai.Height, bd.Data, bd.Length, LCT_GREY, 8);
		if (error) printf("zzz error %u: %s\n", error, lodepng_error_text(error));
		//exit(33);		
		free(bd.Data);
		
		rai.PixelData = imagepixels;
		std::memcpy(rai.PixelData, imagepixels, bd.Length);

		return rai;
	}

	std::ostream& operator<<(std::ostream& out, const RawImageInfo& rai) 
	{
		out << "              Id= " << rai.Id << std::endl;
		for (int i = 0; i < rai.Height; i++)
		{
			out << std::setw(3) << i << std::setw(3) << " | ";
			for (int j = 0; j < rai.Width; j++)
			{
				int offset = rai.Width * i + j;
				if (rai.PixelData[offset] == 255) {
					out << "*";					
				}
				else {
					out << " ";					
				}				
			}
			out << std::endl;			
		}
		return out;
	}

	std::string to_string(const RawImageInfo& config) 
	{
		std::ostringstream ss;
		ss << config;
		return ss.str();
	}
}
#include <ostream>

#include "idbrepository.h"

namespace sdb19db
{
	namespace tables {
		class mis {
		public:
			int id = 0;
			std::string mis_sha256 = "";
			int field_type = 0;
			int hsf_num = 0;
			int ihead_id = 0;
			int writer_num = 0;
			int template_num = 0;
			std::string character = "";
			char* image = nullptr;
			int image_len_bytes = 0;
			std::string image_sha256 = "";
		};
	}

	class Mis : public IDbRepository<tables::mis>
	{
	public:
		~Mis() {}
		Mis(sqlite3* db) : IDbRepository(
			db,
			R"SQL(
				CREATE TABLE "mis" (
					"id"				INTEGER NOT NULL,
					"field_type"		INTEGER NOT NULL,
					"mis_sha256"		TEXT NOT NULL,
					"hsf_num"			INTEGER NOT NULL,
					"ihead_id"			INTEGER NOT NULL,
					"writer_num"		INTEGER NOT NULL,
					"template_num"		INTEGER NOT NULL,
					"character"			TEXT NOT NULL,
					"image_sha256"		TEXT NOT NULL,
					"image"				BLOB NOT NULL,
					FOREIGN KEY (ihead_id) REFERENCES ihead (id),
					PRIMARY KEY("id" AUTOINCREMENT)
				);
			)SQL",
			R"SQL(
				INSERT INTO mis(hsf_num,ihead_id,writer_num,template_num,character,image,image_sha256,mis_sha256,field_type) VALUES (?,?,?,?,?,?,?,?,?);
			)SQL")
		{
		}

		int Insert(const tables::mis& table) const;
	};

	std::ostream& operator<<(std::ostream& out, const tables::mis& ihead_row);

	std::string to_string(const tables::mis& h);
}
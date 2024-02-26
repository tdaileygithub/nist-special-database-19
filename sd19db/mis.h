#include "idbrepository.h"

namespace sdb19db
{
	namespace tables {
		class mis {
		public:
			int id = 0;
			int hsf_num = 0;
			int ihead_id = 0;
			int writer_num = 0;
			int template_num = 0;
			std::string character = "";
			char* image = nullptr;
			int image_len_bytes = 0;
		};
	}

	class Mis : public IDbRepository<tables::mis>
	{
	public:
		~Mis() {}
		Mis(const std::string& dbname) : 
			IDbRepository(dbname,
				R"SQL(
					CREATE TABLE "mis" (
						"id"				INTEGER NOT NULL,
						"hsf_num"			INTEGER NOT NULL,
						"ihead_id"			INTEGER NOT NULL,
						"writer_num"		INTEGER NOT NULL,
						"template_num"		INTEGER NOT NULL,
						"character"			TEXT NOT NULL,
						"image"				BLOB NOT NULL,
						FOREIGN KEY (ihead_id) REFERENCES ihead (id),
						PRIMARY KEY("id" AUTOINCREMENT)
					);
				)SQL",
				R"SQL(
					INSERT INTO mis(hsf_num,ihead_id,writer_num,template_num,character,image) VALUES (?,?,?,?,?,?);
				)SQL")
		{
		}

		int Insert(const tables::mis& table) const;
	};
}
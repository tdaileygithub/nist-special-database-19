#include "idbrepository.h"

namespace sdb19db
{
	namespace tables {
		class hsfpage {
		public:
			int id = 0;
			int hsf_num = 0;
			int ihead_id = 0;
			int writer_num = 0;
			int template_num = 0;
			char* buffer = nullptr;
			int buffer_len_bytes = 0;
		};
	}

	class HsfPage : public IDbRepository<tables::hsfpage>
	{
	public:
		~HsfPage() {}
		HsfPage(const std::string& dbname) : IDbRepository(
			dbname,
			R"SQL(
				CREATE TABLE "hsf_page" (
					"id"				INTEGER NOT NULL,
					"hsf_num"			INTEGER NOT NULL,
					"ihead_id"			INTEGER NOT NULL,
					"writer_num"		INTEGER NOT NULL,
					"template_num"		INTEGER NOT NULL,
					"jpeg"				BLOB NOT NULL,
					FOREIGN KEY (ihead_id) REFERENCES ihead (id),
					PRIMARY KEY("id" AUTOINCREMENT)
				);
			)SQL",
			R"SQL(
				INSERT INTO hsf_page(hsf_num,ihead_id,writer_num,template_num,jpeg) VALUES (?,?,?,?,?);
			)SQL"
		)
		{
		}
		int Insert(const tables::hsfpage& table) const;
		int LastRowId() const;
	};
}
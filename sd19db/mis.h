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
			char* jpeg = nullptr;
			int jpeg_len_bytes = 0;
		};
	}

	class Mis : public IDbRepository<tables::mis>
	{
	public:
		~Mis() {}
		Mis(const std::string& dbname) : IDbRepository(dbname) {}

		int Create() const;
		int Insert(const tables::mis& table) const;
		int LastRowId() const;
	};
}
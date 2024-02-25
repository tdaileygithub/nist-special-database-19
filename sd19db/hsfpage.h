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
		HsfPage(const std::string& dbname) : IDbRepository(dbname) {}

		int Create() const;
		int Insert(const tables::hsfpage& table) const;
		int LastRowId() const;
	};
}
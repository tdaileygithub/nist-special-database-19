#include "idbrepository.h"

namespace sdb19db
{
	namespace tables {
		class hsfpage {
		public:
			int id = 0;
			int ihead_id;
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
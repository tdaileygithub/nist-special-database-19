#include "idbrepository.h"

namespace sdb19db
{
	namespace tables {
		class ihead {
		public:
			int id = 0;
			std::string created = "";
			int width = 0;
			int height = 0;
			int depth = 0;
			int density = 0;
			int compress = 0;
			int complen = 0;
			int align = 0;
			int unitsize = 0;
			int sigbit = 0;
			int byte_order = 0;
			int pix_offset = 0;
			int whitepix = 0;
			int issigned = 0;
			int rm_cm = 0;
			int tb_bt = 0;
			int lr_rl = 0;
			std::string parent = "";
			int par_x = 0;
			int par_y = 0;
		};
	}

	class IHead : public IDbRepository<tables::ihead>
	{
	public:
		int _a = 2;
		//IHead(const IHead&) = default;
		//IHead& operator=(const IHead&) = default;

		~IHead() {}
		IHead(const std::string& dbname) : IDbRepository(dbname) {}

		int Create() const;
		int Insert(const tables::ihead& table) const;
		int LastRowId() const;
	};
}
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
		~IHead() {}
		IHead(const std::string& dbname) : IDbRepository(
			dbname,
			R"SQL(
				CREATE TABLE "ihead" (
					"id"			INTEGER NOT NULL,
					"created"		TEXT NOT NULL,
					"width"			INTEGER NOT NULL,
					"height"		INTEGER NOT NULL,
					"depth"			INTEGER NOT NULL,
					"density"		INTEGER NOT NULL,
					"compress"		INTEGER NOT NULL,
					"complen"		INTEGER NOT NULL,
					"align"			INTEGER NOT NULL,
					"unitsize"		INTEGER NOT NULL,
					"sigbit"		INTEGER NOT NULL,
					"byte_order"	INTEGER NOT NULL,
					"pix_offset"	INTEGER NOT NULL,
					"whitepix"		INTEGER NOT NULL,
					"issigned"		INTEGER NOT NULL,
					"rm_cm"			INTEGER NOT NULL,
					"tb_bt"			INTEGER NOT NULL,
					"lr_rl"			INTEGER NOT NULL,
					"parent"		TEXT NOT NULL,
					"par_x"			INTEGER NOT NULL,
					"par_y"			INTEGER NOT NULL,
					PRIMARY KEY("id" AUTOINCREMENT)
				);
			)SQL",
			R"SQL(
				INSERT INTO ihead(created,width,height,depth,density,compress,complen,align,unitsize,sigbit,byte_order,pix_offset,whitepix,issigned,rm_cm,tb_bt,lr_rl,parent,par_x,par_y) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);
			)SQL"
		)
		{
		}

		int Insert(const tables::ihead& table) const;		
	};
}
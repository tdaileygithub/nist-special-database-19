#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include "pugixml/pugixml.hpp"

using namespace std;

class Sd19Config 
{
private:
    std::string _nistSd19Folder;
    std::string _sqliteDbName;

public:
    Sd19Config() {
        pugi::xml_document doc;

        if (!doc.load_file("config.xml"))
            exit(1);

        _nistSd19Folder = std::string((pugi::xpath_query("/Sd19Db/Configs/Config[@Name='NIST_Special_Database_19']").evaluate_node_set(doc)[0]).node().text().get());
        _sqliteDbName   = std::string((pugi::xpath_query("/Sd19Db/Configs/Config[@Name='Sqlite_Db_Name']").evaluate_node_set(doc)[0]).node().text().get());        
    }

    std::string GetNistSd19Folder() const {
        return _nistSd19Folder;
    }
    std::string GetSqliteDbName() const {
        return _sqliteDbName;
    }
};

int main()
{
    Sd19Config config;

    std::cout << setw(30) << "NIST_Special_Database_19: " << setw(60) << config.GetNistSd19Folder() << std::endl;
    std::cout << setw(30) << "Sqlite_Db_Name: "           << setw(60) << config.GetNistSd19Folder() << std::endl;

	return 0;
}

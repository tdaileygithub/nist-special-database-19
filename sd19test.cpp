#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "doctest/doctest.h"

#include "sqlite_orm/sqlite_orm.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

struct User {
	int id;
	std::string name;
	std::vector<char> hash;  //  binary format
};

TEST_CASE("testing the factorial function")
{
	CHECK(factorial(1) == 1);
	CHECK(factorial(2) == 2);
	CHECK(factorial(3) == 6);
	CHECK(factorial(10) == 3628800);
}

TEST_CASE("sqlite3 open close")
{
    using std::cout;
    using std::endl;
    using namespace sqlite_orm;
    auto storage = make_storage("blob.sqlite",
        make_table("users",
            make_column("id", &User::id, primary_key()),
            make_column("name", &User::name),
            make_column("hash", &User::hash)));
    storage.sync_schema();
    storage.remove_all<User>();

    User alex{
        0,
        "Alex",
        {0x10, 0x20, 0x30, 0x40},
    };
    alex.id = storage.insert(alex);

    cout << "users count = " << storage.count<User>() << endl;

    cout << "alex = " << storage.dump(storage.get<User>(alex.id)) << endl;

    auto hash = storage.get<User>(alex.id).hash;
    
    CHECK(hash.size() == 4);
    CHECK(hash[0] == 0x10);
    CHECK(hash[1] == 0x20);
    CHECK(hash[2] == 0x30);
    CHECK(hash[3] == 0x40);
}
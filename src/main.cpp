#include <iostream>
#include <cstdio>
#include <string>
#include <SQLiteCpp/SQLiteCpp.h>
#include "Factory.h"

int main(int, char**){
    std::cout << "Hello, from main!\n";
    
    try {
        const char* path = "../data/recipes.sqlite";
        SQLite::Database db(path);
        SQLite::Statement query(db, "SELECT * FROM resources");

        while (query.executeStep()) {
            int id = query.getColumn(0);
            const char* name = query.getColumn(1);

            std::cout << "row: " << id << ", " << name << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
};
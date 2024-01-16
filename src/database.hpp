#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

class Database {
public:
    Database(const std::string& dbPath);
    ~Database();

    int open();
    void close();
    bool exists();
    int initialize();

private:
    std::string dbPath;
    sqlite3* db;
};

#endif

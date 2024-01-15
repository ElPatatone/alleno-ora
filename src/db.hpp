#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

class Database {
public:
    Database(std::string_view dbPath);
    ~Database();

    int open();
    void close();
    bool exists();
    int initialize();
    std::string getDBPath(std::string_view configPath);

private:
    std::string dbPath;
    sqlite3* db;
};

#endif // DEBUG

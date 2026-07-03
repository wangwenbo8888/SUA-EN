

#include "acl_cpp/db/db_sqlite.hpp"

#define DATA_SOURCE "E:\\USgHIFU\\MainFrame\\build-HIFUMainframeWithWifi-MinGW-Release\\release\\database.db"

class SqliteDatabase
{
public:
    SqliteDatabase();

    ~SqliteDatabase();

    bool initial();

    bool query(const char* command,std::string& json);

    bool query(const char* command0, const char* command1, std::string& json);

    bool checkUserPassword(const char* command,
                           const std::string& password);

    bool update(const char* command);

private:

    std::string resultToJson(acl::db_rows* result);

    std::string combineResult(acl::db_rows* result0, acl::db_rows* result1);

    acl::db_sqlite* m_db;

};
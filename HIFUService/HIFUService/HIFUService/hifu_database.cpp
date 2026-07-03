

#include "include/hifu_database.h"
#include "json/reader.h"

SqliteDatabase::SqliteDatabase()
{
    m_db = NULL;
}

SqliteDatabase::~SqliteDatabase()
{
}

bool SqliteDatabase::initial()
{

    m_db = new acl::db_sqlite(DATA_SOURCE);

    return true;
}

bool SqliteDatabase::update(const char* command)
{
    if (m_db == NULL)
    {
        return false;
    }
    else
    {
        if (!m_db->open())
        {
            return false;
        }

        if (m_db->sql_update(command))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool SqliteDatabase::checkUserPassword(const char* command,
                                       const std::string& password)
{
    if (m_db == NULL)
    {
        return false;
    }
    else
    {
        acl::db_rows result;

        if (!m_db->open())
        {
            return false;
        }

        if (m_db->sql_select(command, &result))
        {
            const std::vector<acl::db_row*>& rows = result.get_rows();
            for (size_t i = 0; i < rows.size(); ++i)
            {
                const acl::db_row* row = rows[i];
                for (size_t j = 0; j < row->length(); ++j)
                {
                    const char* value = row->field_value(j);
                    if (value != NULL && value == password)
                    {
                        return true;
                    }
                }
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}

bool SqliteDatabase::query(const char* command, std::string& json)
{
    if (m_db==NULL)
    {
        return false;
    }
    else
    {
        acl::db_rows result;
        if (!m_db->open())
        {
            return false;
        }

        if (m_db->sql_select(command, &result))
        {
            json = resultToJson(&result);
            return true;
        }
        else
        {
            return false;
        }

    }

}

bool SqliteDatabase::query(const char* command0, const char* command1, std::string& json)
{
    if (m_db == NULL)
    {
        return false;
    }
    else
    {
        acl::db_rows result0;
        acl::db_rows result1;
        if (!m_db->open())
        {
            return false;
        }

        if (m_db->sql_select(command0, &result0) && m_db->sql_select(command1, &result1))
        {
            json = combineResult(&result0, &result1);
            return true;
        }
        else
        {
            return false;
        }

    }
}

std::string SqliteDatabase::combineResult(acl::db_rows* result0, acl::db_rows* result1)
{
    if (result0 == NULL || result1==NULL)
    {
        return "";
    }

    Json::Value total;
    const std::vector<acl::db_row*>& rows0 = result0->get_rows();
    for (size_t i = 0; i < rows0.size(); ++i)
    {
        Json::Value temp;
        const acl::db_row* row = rows0[i];
        for (size_t j = 0; j < row->length(); ++j)
        {
            const char* name = row->field_name(j);
            const char* value = row->field_value(j);
            if (value == NULL)
            {
                temp[row->field_name(j)] = "NULL";
            }
            else
            {
                temp[row->field_name(j)] = row->field_value(j);
            }
        }

        total["BaseInfo"] = temp;
    }

    Json::Value ReportInfo;
    const std::vector<acl::db_row*>& rows1 = result1->get_rows();
    for (size_t i = 0; i < rows1.size(); ++i)
    {
        Json::Value temp;
        const acl::db_row* row = rows1[i];
        for (size_t j = 0; j < row->length(); ++j)
        {
            const char* name = row->field_name(j);
            const char* value = row->field_value(j);
            if (value == NULL)
            {
                temp[row->field_name(j)] = "NULL";
            }
            else
            {
                temp[row->field_name(j)] = row->field_value(j);
            }
        }

        ReportInfo[i] = temp;
    }
    total["ReportInfo"] = ReportInfo;

    if (total.empty())
    {
        return "";
    }
    else
    {
        return total.toStyledString();
    }
}

std::string SqliteDatabase::resultToJson(acl::db_rows* result)
{
    if (result==NULL)
    {
        return "";
    }

    Json::Value total;
    const std::vector<acl::db_row*>& rows = result->get_rows();
    for (size_t i = 0; i < rows.size(); ++i)
    {
         Json::Value temp;
         const acl::db_row* row = rows[i];
         for (size_t j = 0; j < row->length(); ++j)
         {
             const char* name = row->field_name(j);
             const char* value = row->field_value(j);
             if (value==NULL)
             {
                 temp[row->field_name(j)] = "NULL";
             }
             else
             {
                 temp[row->field_name(j)] = row->field_value(j);
             }
         }

         total[i] = temp;
    }

    if (total.empty())
    {
        return "";
    }
    else
    {
       return total.toStyledString();
    }

}
include(sql/private/CMakeLists.txt)
set(akicore_sql_SRCS
    sql/database.cxx
    sql/query.cxx
    ${akicore_sqlprivate_SRCS}
)
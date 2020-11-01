D:
cd D:\CppBaseLib\SQL\SQLite\sqlite-3330000\tools
sqlite3 ggg.db %创建数据库%
%.quit                   退出%
%sqlite3 testDB.db       路径存在？打开数据库：创建数据库%
%.databases              检查当前数据库%
%sqlite3 testDB.db .dump > testDB.sql 导出数据库至asc文件%
%sqlite3 testDB.db < testDB.sql       从文件还原数据库%
%ATTACH DATABASE 'ggg.db' as 'gg';    附加数据库,并定义名称gg(main 和 temp 被保留用于主数据库和存储临时)%
pause
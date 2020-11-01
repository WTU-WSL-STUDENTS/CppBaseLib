D:
cd D:\CppBaseLib\SQL\SQLite\sqlite-3330000\tools
sqlite3 ggg.db %创建数据库%
%.quit                   退出%
%sqlite3 testDB.db       路径存在？打开数据库：创建数据库%
%.databases              检查当前数据库%
%sqlite3 testDB.db .dump > testDB.sql 导出数据库至asc文件%
%sqlite3 testDB.db < testDB.sql       从文件还原数据库%
%ATTACH DATABASE 'ggg.db' as 'gg';    附加数据库,并定义名称gg(main 和 temp 被保留用于主数据库和存储临时)%
%DETACH DATABASE 'gg';                分离数据库%
%.tables                              查询当前所有的表名%
goto start
CREATE TABLE COMPANY(                 %定义一个名为COMPANY的表%
      ID INT PRIMARY KEY NOT NULL,    %定义一个int类型的ID作为私有主键%
      NAME          TEXT NOT NULL,    %定义一个TEXT类型的NAME%
      AGE            INT NOT NULL,
      ADDRESS        CHAR(50),
      SALARY         REAL
      );
:start %多行注释%
%.schema COMPANY                      查询表的结构%
% DROP TABLE TEMP                     删除指定表%
%INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (1, 'Paul', 32, 'California', 20000.00 );%
%INSERT INTO COMPANY VALUES (7, 'James', 24, 'Houston', 10000.00 ); 添加一行数据%
%.header on .mode column              设置数据显示格式%           
%SELECT * FROM table_name;            在指定表中按指定规则查找数据%
%SELECT ID, NAME, SALARY FROM COMPANY;查找指定列%
%.width 10, 20, 10                    设置前三列显示的宽度%
pause
D:
cd D:\CppBaseLib\SQL\SQLite\sqlite-3330000\tools
sqlite3 ggg.db %�������ݿ�%
%.quit                   �˳�%
%sqlite3 testDB.db       ·�����ڣ������ݿ⣺�������ݿ�%
%.databases              ��鵱ǰ���ݿ�%
%sqlite3 testDB.db .dump > testDB.sql �������ݿ���asc�ļ�%
%sqlite3 testDB.db < testDB.sql       ���ļ���ԭ���ݿ�%
%ATTACH DATABASE 'ggg.db' as 'gg';    �������ݿ�,����������gg(main �� temp ���������������ݿ�ʹ洢��ʱ)%
%DETACH DATABASE 'gg';                �������ݿ�%
%.tables                              ��ѯ��ǰ���еı���%
goto start
CREATE TABLE COMPANY(                 %����һ����ΪCOMPANY�ı�%
      ID INT PRIMARY KEY NOT NULL,    %����һ��int���͵�ID��Ϊ˽������%
      NAME          TEXT NOT NULL,    %����һ��TEXT���͵�NAME%
      AGE            INT NOT NULL,
      ADDRESS        CHAR(50),
      SALARY         REAL
      );
:start %����ע��%
%.schema COMPANY                      ��ѯ��Ľṹ%
% DROP TABLE TEMP                     ɾ��ָ����%
%INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (1, 'Paul', 32, 'California', 20000.00 );%
%INSERT INTO COMPANY VALUES (7, 'James', 24, 'Houston', 10000.00 ); ���һ������%
%.header on .mode column              ����������ʾ��ʽ%           
%SELECT * FROM table_name;            ��ָ�����а�ָ�������������%
%SELECT ID, NAME, SALARY FROM COMPANY;����ָ����%
%.width 10, 20, 10                    ����ǰ������ʾ�Ŀ��%
pause
docker run --name mysql-test -e MYSQL_ROOT_PASSWORD=123456@E -d mysql 
mysql -uroot -p123456@E 
create table stu (id int PRIMARY KEY, name VARCHAR(20), eng INT(20), chi INT(20), math INT(20));
insert into stu(id,name,eng,chi,math) values (2, "zhao", 99, 91, 92);
查询数学成绩的前4名，并按英语成绩排序
select * from (select * from stu order by math desc limit 4)t order by t.eng desc;

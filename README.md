# LCCD
[![Build Status](https://travis-ci.org/iLCSoft/LCCD.svg?branch=master)](https://travis-ci.org/iLCSoft/LCCD)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/12357/badge.svg)](https://scan.coverity.com/projects/ilcsoft-lccd)

Linear Collider Conditions Data framework

LCCD is distributed under the [GPLv3 License](http://www.gnu.org/licenses/gpl-3.0.en.html)

[![License](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)


## Build instructions
In order to build LCCD you need LCIO, ILCUTIL, CMake and optionally CondDBMySQL

if you have access to /cvmfs/ilc.desy.de/ then you will find all this packages under:
```
/cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/
```

if you do NOT have acces to /cvmfs/ilc.desy.de/ or you do not have an SL6 machine the easiest way is to use ilcinstall (http://ilcsoft.desy.de/portal/software_packages/ilcinstall/).

Once the dependencies are resolved, issue the following commands:
```
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake ..
make install
make tests    # optional step to build LCCD tests
```

## Start test database

0. select the mysql version you want to use, e.g.
```
export PATH=/opt/products/mysql/5.0.26/bin:$PATH
```

1. create a data directory
```
mkdir /tmp/mysqlDB
```
2. init the database
```
mysql_install_db --datadir=/tmp/mysqlDB
```
3. start the database
```
mysqld_safe --datadir=/tmp/mysqlDB &
```
or:
```
mysqld_safe --datadir=/tmp/mysqlDB --socket=/tmp/mysql.sock --log-error=mysql.log --pid-file=mysql.pid  &
```
3. b. you can stop the database  - no need to do it now ]
```
mysqladmin -u root -p shutdown  
```
4. set root password (and remember it)
```
mysqladmin -u root password <mypassword>
```
5. create users for LCCD [NOTE: THIS IS FOR TESTING ONLY - BE CAREFUL WITH USER RIGHTS/PRIVILEGES !!!!!]
```
mysql -u root -p 
grant all privileges on *.* to calvin@localhost identified by 'hobbes' with grant option ;
grant all privileges on *.* to calvin@"%" identified by 'hobbes' with grant option ; 
quit
```

## Run examples
If you built against CondDBMysql and built the tests, you can run them as follows:
```
cd ../test
./createcalfile hcal.slcio hcal
./readcalfile hcal.slcio hcal

./calfiletodb hcal.slcio hcal myhcal 100 200
./calfiletodb hcal.slcio hcal myhcal 200 300
./calfiletodb hcal.slcio hcal myhcal 300 400
./calfiletodb hcal.slcio hcal myhcal 400 500

./tagdbfolder /lccd/myhcal mytag01 "just a test tag"

./calfiletodb hcal.slcio hcal myhcal 0 500

./calfilefromdb myhcal 120 fromdb.slcio mytag01

./calfilefromdb myhcal 20 fromdb.slcio 
```


It might be instructive to read the code in parallel and inspect the MySQL database, e.g. 
```
mysql -u calvin -p
```
Enter password:
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 16 to server version: 5.0.26

Type 'help;' or '\h' for help. Type '\c' to clear the buffer.

```
mysql> show databases ;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| lccd_test          |
| mysql              |
| test               |
+--------------------+
4 rows in set (0.00 sec)
```
```
mysql> use lccd_test
```
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed

```
mysql> show tables ;
+-----------------------+
| Tables_in_lccd_test   |
+-----------------------+
| data_tbl_3_1          |
| databases_tbl         |
| folders_tbl           |
| object_key_tbl_3      |
| object_key_tbl_head_3 |
| partition_tbl_3       |
| tag2folder_tbl        |
| tag2obj_tbl_3         |
| tags_tbl              |
+-----------------------+
9 rows in set (0.01 sec)
```
```
mysql> select * from tags_tbl ;
+--------+---------------------+---------+---------+------------------------+
| tag_id | insert_t            | tname   | tattr   | tdesc                  |
+--------+---------------------+---------+---------+------------------------+
|      1 | 2009-10-07 15:16:50 | HEAD    | NO ATTR | Latest objects version |
|      2 | 2009-10-07 15:17:14 | mytag01 | NULL    | just a test tag        |
+--------+---------------------+---------+---------+------------------------+
2 rows in set (0.00 sec)
```



## License and Copyright
Copyright (C), LCCD Authors

LCCD is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License long with this program.  If not, see <http://www.gnu.org/licenses/>.

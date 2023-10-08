#!/bin/sh

mysql -e "create table flag(flag varchar(256)); insert into flag values('$GZCTF_FLAG');" -uroot -proot WHUsubject
unset GZCTF_FLAG
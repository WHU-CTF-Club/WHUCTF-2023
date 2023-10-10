FROM dasctfbase/web_php73_apache_mysql

COPY src /var/www/html

# 如需自定义 FLAG 请将自定义脚本覆盖到 /flag.sh
COPY ./conf/init.sh /flag.sh

# 如需操作数据库请将 sql 文件拷贝到 /db.sql
COPY ./conf/db.sql /db.sql

# 如有上传文件等操作请务必将权限设置正确！
# RUN chown www-data:www-data /var/www/html/uploads/

EXPOSE 80
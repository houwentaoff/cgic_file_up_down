cgic_file_up_down
=================

cgic库配合boa服务器实现上传下载文件功能
1. 编译boa,cp boa && 编译cgic
    `cd boa && ./configure &&make ; cd ../cgic205/ make`
   `install -D -v etc/boa/boa.conf /etc/boa/boa.conf;
    install -D -v boa/src/boa_indexer /var/www/cgi-bin`
2. 编译cgic205,copy upload.cgi到/var/www/cgi-bin/ 新建cgi目录 /var/www/cgi-bin ; 
3. 创建/var/www/cgi-bin/uploads目录供上传使用'chmod' `chmod 777 /var/www/cgi-bin/ -R`


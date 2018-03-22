FROM ctf_base:latest

RUN apt-get install -y php7.0 php7.0-curl php-memcached nginx memcached php7.0-fpm netcat-traditional

RUN mkdir /var/www/proxy
ADD index.php index_src.php login.php login_src.php /var/www/proxy/
ADD default /etc/nginx/sites-available/
ADD init_mem.sh /

EXPOSE 80
CMD service memcached start && service php7.0-fpm start && service nginx start && /init_mem.sh& tail -f /var/log/nginx/access.log

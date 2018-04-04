FROM ctf_base:latest

RUN apt-get install -y php7.0 php-sqlite3 nginx php7.0-fpm

RUN mkdir /var/www/inject_me
ADD index.php db.db /var/www/inject_me/
ADD default /etc/nginx/sites-available/
RUN chown -R www-data:www-data /var/www/inject_me

EXPOSE 80
CMD service php7.0-fpm start && service nginx start && tail -f /var/log/nginx/access.log

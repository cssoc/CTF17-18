FROM ctf_base:latest

RUN apt-get install -y php7.0 php-sqlite3 nginx php7.0-fpm

RUN mkdir /var/www/blind
ADD index.php register.php db.db /var/www/blind/
ADD default /etc/nginx/sites-available/
RUN chown -R www-data:www-data /var/www/blind

EXPOSE 80
CMD service php7.0-fpm start && service nginx start && tail -f /var/log/nginx/access.log

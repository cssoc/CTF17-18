FROM ctf_base:latest

RUN apt-get install -y php7.0 php-sqlite3 nginx php7.0-fpm chromedriver python-selenium

RUN mkdir /var/www/cross
ADD index.php logout.php login.php register.php db.db /var/www/cross/
ADD default /etc/nginx/sites-available/
ADD admin.py /
RUN chown -R www-data:www-data /var/www/cross

EXPOSE 80
CMD service php7.0-fpm start && service nginx start && ./admin.py

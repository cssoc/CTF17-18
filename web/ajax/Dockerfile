FROM ctf_base:latest

RUN apt-get install -y python-flask nginx uwsgi uwsgi-plugin-python

ADD  ajax.py /
ADD templates /templates
ADD db.sqlite /db/db.sqlite
ADD default /etc/nginx/sites-available/

RUN chown -R www-data:www-data /db

EXPOSE 80
CMD service nginx start && uwsgi -s /tmp/ajax.sock --manage-script-name --mount /=ajax:app --uid www-data --gid www-data --plugin python

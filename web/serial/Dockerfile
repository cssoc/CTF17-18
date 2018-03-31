FROM ctf_base:latest

RUN apt-get install -y python-flask nginx uwsgi uwsgi-plugin-python

ADD flag.txt serial.py /
ADD templates /templates
ADD default /etc/nginx/sites-available/

EXPOSE 80
CMD service nginx start && uwsgi -s /tmp/serial.sock --manage-script-name --mount /=serial:app --uid www-data --gid www-data --plugin python

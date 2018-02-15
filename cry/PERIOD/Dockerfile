FROM ctf_base:latest

RUN apt-get install -y xinetd python3 python3-crypto

ADD flag.py lsfr.py period.py /home/challenge/

ADD xinetd.conf /etc/

EXPOSE 1337

CMD xinetd -dontfork

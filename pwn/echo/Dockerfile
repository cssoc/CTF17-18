FROM ctf_base:latest

RUN apt-get install -y xinetd

ADD flag.txt echo /home/challenge/

ADD xinetd.conf /etc/
EXPOSE 3338

CMD xinetd -dontfork

FROM ctf_base:latest

RUN apt-get install -y xinetd

ADD guess /home/challenge/
ADD flag.txt /

ADD xinetd.conf /etc/
EXPOSE 3338

CMD xinetd -dontfork

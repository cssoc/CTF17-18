FROM ctf_base:latest

RUN apt-get install -y xinetd procps

ADD flag.txt ROPe /home/challenge/

ADD xinetd.conf /etc/
EXPOSE 3338

CMD xinetd -dontfork

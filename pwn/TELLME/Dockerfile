FROM ctf_base:latest

RUN apt-get install -y xinetd

ADD flag.txt TELLME /home/challenge/

ADD xinetd.conf /etc/
EXPOSE 3338

CMD xinetd -dontfork

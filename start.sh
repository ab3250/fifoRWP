#!/usr/bin/env bash
./stop.sh
./gwsocket --addr=127.0.0.1 -p 8080 --pipein=/tmp/fifoOut --pipeout=/tmp/fifoIn &
#./gwsocket-modified &
#export NGINX_ROOT="$(pwd)/www/"
#sudo /usr/local/nginx/sbin/nginx -c $(pwd)/nginx.conf
#sudo /usr/local/openresty/bin/openresty -c $(pwd)/nginx.conf
sudo /usr/local/nginx/sbin/nginx -c $(pwd)/nginx.conf
chibi-scheme -r fifoRWP.scm &
google-chrome-stable 127.0.0.1/index.html &
disown


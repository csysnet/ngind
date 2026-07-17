gcc -g -O0 -Icore -Itransport -Ihttp -o serv serv.c core/*c transport/*.c http/*.c && gdb ./ser
gcc -Icore -Itransport -Ihttp -o serv serv.c core/*c transport/*.c http/*.c && ./serv
printf "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\nContent-Length: 123456\r\n\r\n" | nc localhost 8080
printf "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\nTransfer-Encoding: chunked\r\n\r\n" | nc localhost 8080

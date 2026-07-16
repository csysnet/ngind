gcc -Icore -Itransport -Ihttp -o serv serv.c core/*c transport/*.c http/*.c && ./serv
gcc -g -O0 -Icore -Itransport -Ihttp -o serv serv.c core/*c transport/*.c http/*.c && gdb ./ser
printf "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\nName: Nhi\r\n\r\n" | nc localhost 8080

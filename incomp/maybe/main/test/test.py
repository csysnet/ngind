import socket
import time

HOST = "localhost"
PORT = 8080

with socket.create_connection((HOST, PORT)) as s:
    # Send only part of the headers
    s.sendall(b"GET /api HTTP/1.1\r\n")
    s.sendall(b"Host: localhost:8080\r\n")

    print("Partial headers sent")

    # Wait 10 seconds
    time.sleep(10)

    # Finish the headers (blank line ends the header section)
    s.sendall(
        b"User-Agent: test-client\r\n"
        b"Accept: */*\r\n"
        b"Connection: close\r\n"
        b"\r\n"
    )

    print("Remaining headers sent")

    # Read the response
    response = b""
    while True:
        data = s.recv(4096)
        if not data:
            break
        response += data

print(response.decode(errors="replace"))

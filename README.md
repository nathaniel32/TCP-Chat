# **TCP Socket Chat**

### **Aufruf**
- **Console**
    1. **Compiler**
        - `gcc -o Client client.c -lws2_32`
        - `gcc -o server server.c -lws2_32`
    2. **Programmausführung**
        - `server < ipv6 > < port > < snummer >`
        - `client < ipv6 > < port > < snummer >`

- **Visual Studio**
    - Eigenschaftenseiten --> Debugging --> Befehlsargumente : ***< ipv6 > < port > < snummer >***
    - Eigenschaftenseiten --> Linker --> Eingabe --> Zusätzliche Abhängigkeiten : ***Ws2_32.lib***

### **Quellen**
- [**addrinfo**](https://learn.microsoft.com/de-de/windows/win32/api/ws2def/ns-ws2def-addrinfoa)
- [**beispiel Socket**](https://learn.microsoft.com/de-de/windows/win32/winsock/complete-server-code)
- [**kbhit**](https://learn.microsoft.com/de-de/cpp/c-runtime-library/reference/kbhit?view=msvc-170)
- [**select**](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select)
- [**SOCKADDR_IN**](https://learn.microsoft.com/de-de/windows/win32/api/winsock/ns-winsock-sockaddr_in)
- [**ioctlsocket**](https://learn.microsoft.com/de-de/windows/win32/api/winsock/nf-winsock-ioctlsocket)
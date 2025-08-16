if not exist bin mkdir bin

gcc src/*.c -o bin\app -lws2_32

cd bin
app ::1 5000 s83823
if not exist bin mkdir bin

gcc src/*.c ..\common\src\*.c -o bin\app.exe -lws2_32

cd bin
app ::1 5000 s83823
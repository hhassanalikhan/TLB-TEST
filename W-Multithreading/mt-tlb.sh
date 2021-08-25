gcc -pthread -o multithreadingTest multithreadingTest.c -lm
for i in {1..100}; do ./multithreadingTest; done
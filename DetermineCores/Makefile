all: rc rjava rpython

rc: c_compile 
	echo "---C Program---"
	./main 1> c_output.csv 2> c_runtime.txt
	echo "---End---------\n"

c_compile: main.c
	gcc main.c -o main -lm -lpthread

rjava: Main.java
	echo "---Java Program---"
	java Main.java 1> java_output.csv 2> j_runtime.txt
	echo "---End------------\n"

rpython: main.py
	echo "---Python Program---"
	python3 main.py 1> python_output.csv 2> p_runtime.txt
	echo "---End--------------\n"
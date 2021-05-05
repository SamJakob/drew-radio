run: drew-radio
	./out/drew-radio

debug: drew-radio-debug
	./out/drew-radio

drew-radio: main.cpp
	g++ -o ./out/drew-radio main.cpp -lbluetooth -std=c++17

drew-radio-debug: main.cpp
	g++ -g -o ./out/drew-radio main.cpp -lbluetooth -std=c++17

clean:
	rm -rf ./out
	mkdir ./out

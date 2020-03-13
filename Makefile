app: app.o board.o
	g++ -lsfml-graphics -lsfml-window -lsfml-system bin/app.o bin/board.o -o baduk.run

app.o: cli/app.cpp
	g++ -c cli/app.cpp -o bin/app.o

# use -lcurl
server.o: server/server.cpp
	g++ -c server/server.cpp -o bin/server.o

board.o: game_files/board.cpp game_files/board.h
	g++ -c game_files/board.cpp -o bin/board.o

clean:
	rm -f bin/*.o baduk.run

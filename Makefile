quikdiff:  main.o quikdiff.o memoryhandler.o indexer.o filehandler.o errorhandler.o
	/usr/bin/gcc -g -o quikdiff main.o quikdiff.o memoryhandler.o indexer.o filehandler.o errorhandler.o


main.o: main.c
	/usr/bin/gcc -g -c main.c qdglobal.h
	
quikdiff.o: quikdiff.c quikdiff.h  qdglobal.h
	/usr/bin/gcc -g -c quikdiff.c 

indexer.o: indexer.c indexer.h qdglobal.h
	/usr/bin/gcc -g -c indexer.c

filehandler.o: filehandler.c filehandler.h qdglobal.h
	/usr/bin/gcc -g -c filehandler.c

errorhandler.o: errorhandler.c errorhandler.h qdglobal.h
	/usr/bin/gcc -g -c errorhandler.c

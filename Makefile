includePath = ./include/
srcPath = ./src
binPath = ./bin
cFlags = -g

ExtraCFiles = $(srcPath)/array_2d/array_2d.c $(srcPath)/dlist/dlist.c


all: bin gcc_comp

bin:
	mkdir $(binPath)

gcc_comp:
<<<<<<< HEAD
	gcc -Wall -I $(includePath) -o $(binPath)/graph $(srcPath)/is_connected.c $(srcPath)/graph/graph.c $(srcPath)/dlist/dlist.c $(cFlags)
	gcc -Wall -I $(includePath) -o $(binPath)/graph2 $(srcPath)/is_connected.c $(srcPath)/graph2/graph2.c $(srcPath)/array_2d/array_2d.c $(cFlags)
=======
	gcc -Wall -I $(includePath) -o $(binPath)/graph_test $(srcPath)/graph_test.c $(srcPath)/graph/graph.c $(srcPath)/dlist/dlist.c $(cFlags)
	#gcc -Wall -I $(includePath) -o $(binPath)/graph $(srcPath)/is_connected.c $(srcPath)/graph/graph.c $(srcPath)/dlist/dlist.c $(cFlags)
	#gcc -Wall -I $(includePath) -o $(binPath)/graph2 $(srcPath)/is_connected.c $(srcPath)/graph2/graph2.c $(srcPath)/array_2d/array_2d.c $(cFlags)
>>>>>>> 720fcd8 (e)

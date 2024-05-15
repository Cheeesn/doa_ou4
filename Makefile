includePath = ./include/
srcPath = ./src
binPath = ./bin
cFlags = -g

ExtraCFiles = $(srcPath)/array_2d/array_2d.c $(srcPath)/dlist/dlist.c $(srcPath)/array_1d/array_1d.c


all: bin gcc_comp

bin:
	mkdir $(binPath)

gcc_comp:
	#gcc -Wall -I $(includePath) -o $(binPath)/graph_test $(srcPath)/graph_test.c $(srcPath)/graph/graph.c $(ExtraCFiles) $(cFlags)
	gcc -Wall -I $(includePath) -o $(binPath)/graph $(srcPath)/is_connected/is_connected.c $(srcPath)/graph/graph.c $(ExtraCFiles) $(cFlags)
	#gcc -Wall -I $(includePath) -o $(binPath)/graph2 $(srcPath)/is_connected/is_connected.c $(srcPath)/graph2/graph2.c $(ExtraCFiles) $(cFlags)

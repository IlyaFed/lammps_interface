all:
	g++ -std=c++11 -ggdb lib/density.cpp lib/areas.cpp lib/atom.cpp lib/box.cpp main.cpp -o p
	./p -xyz_collect source_for_test/hydrogen_dens.xyz 12 -convchg visualisation/CHGCAR
	rm p
	rm -r p.dSYM

test:
	g++ -std=c++11 -ggdb lib/density.cpp lib/areas.cpp lib/atom.cpp lib/box.cpp test.cpp -o test_dft_analysis
	./test_dft_analysis
	gnuplot plot.txt
	rm test_dft_analysis
	rm -r test_dft_analysis.dSYM

clean:
	rm -f dft_analysis
	rm -f test_dft_analysis

plot:
	gnuplot plot.txt

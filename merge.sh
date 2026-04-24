for (( c=1; c<10; c++))
do
	cd Jobs/cen$c
	cp ../../combineResults_simple.pl .
	perl combineResults_simple.pl
	root -b -q myCombine.C++
	mv ntuple.merge.root ../../cen$c.piplus.root
	cd ../../
done

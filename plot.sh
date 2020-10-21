PATH_TO_DATA=$1

gnuplot -persist <<-EOFMarker
	reset
	set contour base
	set cntrparam level incremental -3, 0.5, 3
	unset surface
	set table 'cont.dat'
	splot '$PATH_TO_DATA'
	unset table
	reset
	unset key
	set xrange [0:1]
	set yrange [0:1]
	set palette rgbformulae 33,13,10
	p '$PATH_TO_DATA' with image, 'cont.dat' w l lt -1 lw 1.5
EOFMarker


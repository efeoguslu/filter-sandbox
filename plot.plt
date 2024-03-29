
        # Set plot title and axis labels
        set title "Sensor Data Plot"
        set xlabel "Sample Index"
        set ylabel "Values"

        # Set data file
        set datafile separator ','

        # Set y-axis limits
        # set yrange [0:2]

        # Plot data from file as a line graph
        plot "extractedAzLogFile.txt" with lines title "rotatedAzLogFile", \
             "iirFilter.txt" with lines title "iirFilterOutput", \
             "firFilter.txt" with lines title "firFilterOutput"
    
set arrow from 1014, graph 0 to 1014, graph 1 nohead lc rgb 'red' lw 1
set arrow from 2782, graph 0 to 2782, graph 1 nohead lc rgb 'red' lw 1
set arrow from 3548, graph 0 to 3548, graph 1 nohead lc rgb 'red' lw 1
set arrow from 3657, graph 0 to 3657, graph 1 nohead lc rgb 'red' lw 1
set arrow from 4656, graph 0 to 4656, graph 1 nohead lc rgb 'red' lw 1
set arrow from 6032, graph 0 to 6032, graph 1 nohead lc rgb 'red' lw 1
set arrow from 7336, graph 0 to 7336, graph 1 nohead lc rgb 'red' lw 1
set arrow from 7706, graph 0 to 7706, graph 1 nohead lc rgb 'red' lw 1
set arrow from 8377, graph 0 to 8377, graph 1 nohead lc rgb 'red' lw 1
set arrow from 8498, graph 0 to 8498, graph 1 nohead lc rgb 'red' lw 1
set arrow from 9034, graph 0 to 9034, graph 1 nohead lc rgb 'red' lw 1
set arrow from 10448, graph 0 to 10448, graph 1 nohead lc rgb 'red' lw 1
set arrow from 10579, graph 0 to 10579, graph 1 nohead lc rgb 'red' lw 1
set arrow from 11086, graph 0 to 11086, graph 1 nohead lc rgb 'red' lw 1
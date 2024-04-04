
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
             "lowThresholdLogFile.txt" with lines title "lowThres", \
             "userInputLogFile.txt" using 0:2 lines title "user"
    
set arrow from 3011, graph 0 to 3011, graph 1 nohead lc rgb 'red' lw 1
set arrow from 3523, graph 0 to 3523, graph 1 nohead lc rgb 'red' lw 1
set arrow from 3636, graph 0 to 3636, graph 1 nohead lc rgb 'red' lw 1
set arrow from 8698, graph 0 to 8698, graph 1 nohead lc rgb 'red' lw 1
set arrow from 8927, graph 0 to 8927, graph 1 nohead lc rgb 'red' lw 1
set arrow from 9422, graph 0 to 9422, graph 1 nohead lc rgb 'red' lw 1
set arrow from 9533, graph 0 to 9533, graph 1 nohead lc rgb 'red' lw 1
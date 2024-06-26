#include "logging.h"
#include "queue.h"

auto t1 = std::chrono::high_resolution_clock::now();

// Function to calculate elapsed time in hours:minutes:seconds format
std::string getElapsedTime() {
    // Calculate elapsed time since t1
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(now - t1).count();

    // Calculate hours, minutes, and remaining seconds
    int hours = elapsed_seconds / 3600;
    int minutes = (elapsed_seconds % 3600) / 60;
    int seconds = elapsed_seconds % 60;

    // Format elapsed time as hours:minutes:seconds
    std::ostringstream elapsed_time;
    elapsed_time << std::setfill('0') << std::setw(2) << hours << ":"; // Hours
    elapsed_time << std::setfill('0') << std::setw(2) << minutes << ":"; // Minutes
    elapsed_time << std::setfill('0') << std::setw(2) << seconds; // Seconds

    return elapsed_time.str();
}


void logData(std::ofstream &logfile, float ax, float ay, float az, float gr, float gp, float gy) {
    if (logfile.is_open()) {
        // Get elapsed time in hours:minutes:seconds format
        std::string elapsed_time = getElapsedTime();

        // Write elapsed time and sensor data
        logfile << elapsed_time << ", ";
        
        logfile << std::setprecision(6) << std::fixed; // Set precision for all subsequent data
        logfile << ax << ", " << ay << ", " << az << ", ";
        logfile << gr << ", " << gp << ", " << gy << std::endl;
        
    }
}


void logData(std::ofstream &logfile, float data) {
    if (logfile.is_open()) {
        // Get elapsed time in hours:minutes:seconds format
        std::string elapsed_time = getElapsedTime();

        // Write elapsed time and sensor data
        logfile << elapsed_time << ", ";
        
        logfile << std::setprecision(6) << std::fixed; // Set precision for all subsequent data
        logfile << data << std::endl;
        
    }
}

void logData(std::ofstream &logfile, float compoundAccelVector, float compoundGyroVector) {
    if (logfile.is_open()) {
        // Get elapsed time in hours:minutes:seconds format
        std::string elapsed_time = getElapsedTime();

        // Write elapsed time and sensor data
        logfile << elapsed_time << ", ";
        
        logfile << std::setprecision(6) << std::fixed; // Set precision for all subsequent data
        logfile << compoundAccelVector << "," << compoundGyroVector << std::endl;
    }
}

void logBump(std::ofstream &logfile, queue* q){
    if(logfile.is_open()){
        // Get elapsed time in hours:minutes:seconds format
        std::string elapsed_time = getElapsedTime();

        // Write elapsed time and sensor data
        logfile << elapsed_time << ", ";

        logfile << std::setprecision(6) << std::fixed; // Set precision for all subsequent data
        logfile << q->samples_processed << ", " << q->bump_counter << std::endl;
    }
}


void logAngles(std::ofstream &logfile, float roll, float pitch) {
    if (logfile.is_open()) {
        // Get elapsed time in hours:minutes:seconds format
        std::string elapsed_time = getElapsedTime();

        // Write elapsed time and sensor data
        logfile << elapsed_time << ", ";
        
        logfile << std::setprecision(6) << std::fixed; // Set precision for all subsequent data
        logfile << roll << ", " << pitch << std::endl;
    }
}


void extractSensorData(std::ifstream& inputFile, std::ofstream& outputFile) {
    if (!inputFile.is_open()) {
        std::cerr << "Input file is not open." << std::endl;
        return;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Output file is not open." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string timestamp;
        double sensorData;

        if (iss >> timestamp >> sensorData) {
            outputFile << sensorData << std::endl;
        }
    }
}

// Function to create the .plt file
bool createPlotScript(const std::string& directoryPath, const std::vector<int>& verticalLineIndices) {
    std::stringstream plotScript;

    // Add Gnuplot script header
    plotScript << R"(
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
             "firFilter.txt" with lines title "firFilterOutput", \
             "lowThresholdLogFile.txt" with lines title "lowThres"
    )";

    // Add commands for vertical lines using arrows
    for (int index : verticalLineIndices) {
        plotScript << "\nset arrow from " << index << ", graph 0 to " << index << ", graph 1 nohead lc rgb 'red' lw 1";
    }

    std::string plotScriptPath = directoryPath + "plot.plt";
    std::ofstream plotScriptFile(plotScriptPath);

    if (!plotScriptFile.is_open()) {
        std::cerr << "Error: Unable to create plot script file." << std::endl;
        return false;
    }

    plotScriptFile << plotScript.str();
    plotScriptFile.close();

    return true;
}



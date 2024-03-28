#include "filters.h"
#include "logging.h"
#include "queue.h"

FirstOrderIIR iirFilter;
FIRFilter firFilter;

float filterAlpha{ 0.5f };

std::vector<float> readSensorData(const std::string& filePath) {
    std::vector<float> sensorData;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return sensorData;
    }

    float value;
    while (file >> value) {
        sensorData.push_back(value);
    }

    file.close();
    return sensorData;
}

int main(){

    // Initialize Filters
    FirstOrderIIR_Init(&iirFilter, filterAlpha);
    FIRFilter_Init(&firFilter);

    float iirFilterOutput{ 0.0f };
    float firFilterOutput{ 0.0f };
    float std_dev{ 0.0f };

    queue q1, q2;
    
    int bufferSize{ 10 };
    init_queue(&q1, bufferSize);
    init_queue(&q2, bufferSize);

    std::ifstream rotatedAzLogFile("rotatedAzLogFile.txt");
    std::ofstream extractedAzLogFile("extractedAzLogFile.txt");
    

    // Extract Data

    extractSensorData(rotatedAzLogFile, extractedAzLogFile);
    extractedAzLogFile.close();

    std::vector<float> sensorData = readSensorData("extractedAzLogFile.txt");

    // Open output files for filtered data
    std::ofstream iirFilterLogFile("iirFilter.txt");
    std::ofstream firFilterLogFile("firFilter.txt");

    std::ofstream stdDevLogFile("standardDeviation.txt");

    // Apply filters and bump detection
    for (float value : sensorData) {

        // Apply your filters here
        float filteredValue1 = FirstOrderIIR_Update(&iirFilter, value);
        float filteredValue2 = FIRFilter_Update(&firFilter, value);

        // Write filtered values to output files
        iirFilterLogFile << filteredValue1 << std::endl;
        firFilterLogFile << filteredValue2 << std::endl;
        stdDevLogFile << calculate_std_dev(&q2, calculate_mean(&q2)) << std::endl;

        // Use the filtered value for bump detection
        enqueue(&q1, filteredValue1); // or filteredValue2, filteredValue3, depending on which filter's output you want to use

        /*
        if(q1.bump_detected){
            std::cout << "Bump detected at sample " << q1.samples_processed << " with IIR filtering, Count: " << q1.bump_counter << std::endl;
            q1.bump_detected = false;
        }
        */
        

        enqueue(&q2, filteredValue2);

        if(q2.bump_detected){
            std::cout << "Bump detected at sample " << q2.samples_processed << " Sec: " << float(q2.samples_processed) / 75 << " with FIR filtering, Count: " << q2.bump_counter << std::endl;
            q2.bump_detected = false;
        }


    }

    // Output results
    std::cout << "Bumps detected with IIR filtering: " << q1.bump_counter << std::endl;
    std::cout << "Bumps detected with FIR filtering: " << q2.bump_counter << std::endl;


    return 0;
}
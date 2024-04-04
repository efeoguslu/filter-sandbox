#include "filters.h"
#include "logging.h"
#include "queue.h"

FirstOrderIIR iirFilter;
FIRFilter firFilter;

float filterAlpha{ 0.25f };

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


void lowThresholdUpdate(float* output, float x){
    if(x >= 0.8f && x <= 1.2f){
        *output = 0.0f;
    }
    else if(x < 0.8f){
        *output = x - 1.0f; // This will return a negative value for inputs smaller than 0.8
    }
    else{
        *output = x - 1.0f; // This will return the same output for inputs larger than 1.2
    }
}

int main(){

    // Initialize Filters
    FirstOrderIIR_Init(&iirFilter, filterAlpha);
    // FIRFilter_Init(&firFilter);

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

    std::ofstream lowThresholdLogFile("lowThresholdLogFile.txt");

    std::vector<int> verticalLineIndices;

    auto directoryPath = "/home/efeoguslu/Desktop/filterSandbox/";
    float lowThres{ 0 };


    // Apply filters and bump detection
    for (float value : sensorData) {

        // Apply your filters here
        float IIRfilteredValue = FirstOrderIIR_Update(&iirFilter, value);
        float filteredValue2 = FIRFilter_Update(&firFilter, value);

        lowThresholdUpdate(&lowThres, value);

        lowThresholdLogFile << lowThres << std::endl;

        // Write filtered values to output files
        iirFilterLogFile << IIRfilteredValue << std::endl;
        firFilterLogFile << filteredValue2 << std::endl;
        stdDevLogFile << calculate_std_dev(&q2, calculate_mean(&q2)) << std::endl;

        // Use the filtered value for bump detection
        enqueue(&q1, IIRfilteredValue); // or filteredValue2, filteredValue3, depending on which filter's output you want to use

        
        if(queue_full(&q1)){
            if(q1.bump_detected){
            std::cout << "Bump detected at sample " << q1.samples_processed << " with IIR filtering, Count: " << q1.bump_counter << std::endl;
            q1.bump_detected = false;
        }
        }
        
        
        enqueue(&q2, filteredValue2);

        /*
        if(q2.bump_detected){
            std::cout << "Bump detected at sample " << q2.samples_processed << " Sec: " << float(q2.samples_processed) / 75 << " with First Order IIR filtering, Count: " << q2.bump_counter << std::endl;
            // Add the sample index to the vector
            verticalLineIndices.push_back(q2.samples_processed);
            q2.bump_detected = false;
        }
        */
        


        // // Sleep for the remaining time to achieve 75 Hz sampling rate
        // static auto lastTime = std::chrono::high_resolution_clock::now();
        // auto currentTime = std::chrono::high_resolution_clock::now();
        // auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count();
        // auto desiredElapsed = 1000000 / 75; // 1 second / 75 Hz = 13333 microseconds
        // if (elapsed < desiredElapsed) {
        //     std::this_thread::sleep_for(std::chrono::microseconds(desiredElapsed - elapsed));
        // }
        // lastTime = std::chrono::high_resolution_clock::now();


    }

    // Create plot script with vertical lines
    // createPlotScript(directoryPath, verticalLineIndices);

    // Output results
    std::cout << "Bumps detected with IIR filtering: " << q1.bump_counter << std::endl;
    std::cout << "Bumps detected with FIR filtering: " << q2.bump_counter << std::endl;


    return 0;
}


#pragma once // Ensures this header file is included only once during compilation

#include "CommonObject.h" // Include the common objects header for shared data
#include <string>         // For using std::string
#include <httplib.h>      // For making HTTP requests
#include <nlohmann/json.hpp> // For handling JSON data

class DownloadThread
{
public:
    // Operator function to run the thread, downloading weather data as needed
    void operator()(CommonObjects& common);

    // Function to set the base URL for the API requests
    void SetUrl(const std::string& base_url);

private:
    std::string _base_url; // Stores the base URL for the API
};

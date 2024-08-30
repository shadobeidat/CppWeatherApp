#include "DownloadThread.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT // Enables SSL support in httplib (ssl is used for secure communication over the internet)
#include "httplib.h"  // Includes the httplib library for making HTTP requests
#include "nlohmann/json.hpp" // Includes the nlohmann::json library for handling JSON data

// This function runs the download thread, which periodically checks for download requests.
void DownloadThread::operator()(CommonObjects& common)
{
    while (!common.exit_flag.load(std::memory_order_acquire)) // Check if the application should exit (load:reads the value of the atomic flag)
    {
        if (common.start_download.load(std::memory_order_acquire)) // Check if a download should start
        {
            httplib::Client cli("http://api.openweathermap.org"); // Create an HTTP (instanse) client for the OpenWeatherMap API
            //there is another parameter=port =443 by default

// Construct the full URL for the API request
            std::string complete_url = _base_url + common.city + "&appid=" + common.api_key;

            // Send a GET request to the constructed URL
            auto res = cli.Get(complete_url.c_str());

            if (res && res->status == 200) // Check if the request was successful (the res is smart pointer)
            {
                // Parse the JSON response and store it in the common object
                auto json_result = nlohmann::json::parse(res->body);
                common.weather_data = json_result;

                // Signal that the data is ready for processing
                common.data_ready.store(true, std::memory_order_release);
            }
            else {
                std::cerr << "Failed to fetch data. Status code: " << (res ? res->status : 0) << std::endl;
            }

            // Reset the start_download flag after processing
            common.start_download.store(false, std::memory_order_release);
        }
        else
        {
            // Sleep for a short duration to prevent busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }



    }



}

// This function sets the base URL for the API requests
void DownloadThread::SetUrl(const std::string& base_url)
{
    _base_url = base_url; // Store the base URL
}

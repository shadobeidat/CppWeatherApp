#pragma once // Ensures this header file is included only once during compilation

#include <atomic>      // For using atomic types for thread-safe operations
#include <string>      // For using the std::string type
#include <vector>      // For using the std::vector type
#include <algorithm>   // For using algorithms like std::find
#include <nlohmann/json.hpp> // For handling JSON data

// This structure holds common data and states shared between threads.
struct CommonObjects
{
    // Atomic flags to manage thread-safe operations
    std::atomic_bool exit_flag = false;         // Signals when the application should exit
    std::atomic_bool start_download = false;    // Indicates when to start downloading weather data
    std::atomic_bool data_ready = false;        // Indicates when the weather data is ready to be processed

    // Strings to store the API endpoint URL, city name, and API key
    std::string url;       // The complete URL for fetching weather data
    std::string city;      // The name of the city for which weather data is fetched
    std::string api_key = "41c16c5061f234d94d129970f4270efa"; // API key for accessing OpenWeatherMap

    // JSON object to store the weather data fetched from the API
    nlohmann::json weather_data;

    // Vectors to store favorite cities and search history
    std::vector<std::string> favorite_cities;  // List of user's favorite cities
    std::vector<std::string> search_history;   // List of the most recent city searches

    // Unit preferences for temperature and wind speed
    bool use_fahrenheit = false; // false for Celsius, true for Fahrenheit
    bool use_miles = false;      // false for kilometers, true for miles

    // Function to set the city and construct the API request URL
    void set_city(const std::string& city_name)
    {
        city = city_name; // Set the city name
        // Construct the full URL using the city name and API key
        url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key;
    }

    // Function to add a city to the list of favorite cities
    void add_to_favorites(const std::string& city_name)
    {
        // Check if the city is not already in the favorites list
        if (std::find(favorite_cities.begin(), favorite_cities.end(), city_name) == favorite_cities.end())
        {
            favorite_cities.push_back(city_name); // Add the city to the favorites list
        }
    }

    // Function to remove a city from the list of favorite cities
    void remove_from_favorites(const std::string& city_name)
    {
        // Find the city in the favorites list
        auto it = std::find(favorite_cities.begin(), favorite_cities.end(), city_name);
        if (it != favorite_cities.end())
        {
            favorite_cities.erase(it); // Remove the city from the favorites list
        }
    }

    // Function to add a city to the search history
    void add_to_search_history(const std::string& city_name)
    {
        search_history.push_back(city_name); // Add the city to the search history
        if (search_history.size() > 10) // Limit the history to the last 10 searches
        {
            search_history.erase(search_history.begin()); // Remove the oldest entry if history exceeds 10
        }
    }

    // Function to convert temperature from Kelvin to the desired unit (Celsius or Fahrenheit)
    float convert_temperature(float kelvin) const
    {
        if (use_fahrenheit)
        {
            return (kelvin - 273.15f) * 9.0f / 5.0f + 32.0f; // Convert to Fahrenheit
        }
        else
        {
            return kelvin - 273.15f; // Convert to Celsius
        }
    }

    // Function to convert wind speed from meters per second to the desired unit (kilometers per hour or miles per hour)
    float convert_wind_speed(float meters_per_sec) const
    {
        if (use_miles)
        {
            return meters_per_sec * 2.237f; // Convert to miles per hour
        }
        else
        {
            return meters_per_sec; // Keep in meters per second
        }
    }
};

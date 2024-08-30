#include <iostream>              // Includes the standard input-output stream library for printing to the console.
#include <thread>                // Includes the thread library for creating and managing threads.
#include "CommonObject.h"        // Includes the header file for the CommonObjects structure that stores shared data.
#include "DrawThread.h"          // Includes the header file for the DrawThread class that handles the GUI.
#include "DownloadThread.h"      // Includes the header file for the DownloadThread class that handles downloading data.

int main()
{
    // Create an instance of CommonObjects to hold shared data and states.
    CommonObjects common;

    // Create an instance of DrawThread to handle drawing the GUI.
   //DrawThread draw; creates an object that manages the GUI(displaying data and handling user input).
    DrawThread draw;

    // Start a thread for the DrawThread instance, passing the common object by reference.
    // The std::jthread automatically handles thread lifecycle management.
    std::thread draw_th([&] { draw(common); });


    // DownloadThread down; creates an object that handles data fetching(downloading weather data).
    // Create an instance of DownloadThread to handle downloading weather data.
    DownloadThread down;

    // Set the base URL for downloading weather data from the OpenWeatherMap API.
    down.SetUrl("http://api.openweathermap.org/data/2.5/weather?q=");

    // Start a thread for the DownloadThread instance, passing the common object by reference.
    std::thread down_th([&] { down(common); });

    // Print a message to the console indicating that the application is running.
    std::cout << "Running...\n";

    // Wait for the download thread to complete its execution.
    down_th.join();

    // Wait for the draw thread to complete its execution.
    draw_th.join();
}

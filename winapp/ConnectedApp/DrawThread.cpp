#include "DrawThread.h"
#include "GuiMain.h" // Contains the main function to run the GUI.
#include "../../shared/ImGuiSrc/imgui.h" // Includes the ImGui library for GUI rendering
//#include "../../../ConnectedApp/IconsFontAwesome5.h" // Include the FontAwesome icon library

// Function to draw the main application window
void DrawAppWindow(void* common_ptr)
{
    // Cast the void pointer to a CommonObjects pointer
    auto* common = static_cast<CommonObjects*>(common_ptr);

    ImGui::Begin("Weather Management"); // Begin a new ImGui window with the title "Weather Management"


    if (ImGui::BeginTabBar("Weather Tabs")) {
        if (ImGui::BeginTabItem("Weather Info")) {
            ImGui::Text("Enter City");
            ImGui::SameLine();
            static char city_input[128] = ""; // Static buffer to store user input for the city name
            ImGui::PushItemWidth(200); // Set the width of the input field
            ImGui::InputText("##cityinput", city_input, IM_ARRAYSIZE(city_input)); // Input field for the city name
            ImGui::PopItemWidth(); // Reset the item width to default
            ImGui::SameLine();


            if (ImGui::Button("Get Weather")) {
                std::string user_input_city(city_input); // Convert the user input to a std::string
                common->set_city(user_input_city); // Set the city in the common object
                common->start_download.store(true, std::memory_order_release); // Signal the download thread to start downloading
                common->add_to_search_history(user_input_city); // Add the city to the search history
            }

            // Layout adjustments for settings and buttons
            ImGui::SameLine();
            ImGui::Checkbox("Fahrenheit", &common->use_fahrenheit);
            ImGui::SameLine();
            ImGui::Checkbox("Miles/h", &common->use_miles);

            // Styled Button Block
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();


            // Push style for blue buttons with white text
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.47f, 0.87f, 1.0f)); // Button background color: blue
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08f, 0.52f, 0.98f, 1.0f)); // Button on hover: lighter blue
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.37f, 0.69f, 1.0f)); // Button clicked: dark blue
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Text color: white


            if (ImGui::Button("Add to Favorites")) {
                common->add_to_favorites(city_input);
            }
            ImGui::SameLine();
            if (ImGui::Button("Remove from Favorites")) {
                common->remove_from_favorites(city_input);
            }


            // Pop style colors to return to default
            ImGui::PopStyleColor(4); // Pop all 4 styles set earlier

            // Displaying weather data
            if (common->data_ready.load(std::memory_order_acquire)) {
                ImGui::Text("Weather Details:");

                // Applying custom colors
              // City Name
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.27f, 0.54f, 0.70f, 1.0f));
                ImGui::Text("City: %s", common->weather_data["name"].get<std::string>().c_str());
                ImGui::PopStyleColor();

                // Temperature
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.34f, 0.13f, 1.0f));
                ImGui::Text("Temperature: %.2f %s", common->convert_temperature(common->weather_data["main"]["temp"].get<float>()), common->use_fahrenheit ? "F" : "C");
                ImGui::PopStyleColor();

                // Humidity
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.75f, 0.58f, 1.0f));
                ImGui::Text("Humidity: %d%%", common->weather_data["main"]["humidity"].get<int>());
                ImGui::PopStyleColor();

                // Wind Speed (Lilac)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.78f, 0.67f, 0.85f, 1.0f));
                ImGui::Text("Wind Speed: %.2f %s", common->convert_wind_speed(common->weather_data["wind"]["speed"].get<float>()), common->use_miles ? "mph" : "m/s");
                ImGui::PopStyleColor();
            }
            ImGui::EndTabItem();
        }

        /*  if (ImGui::BeginTabItem("Settings")) {
              ImGui::Checkbox("Use Fahrenheit", &common->use_fahrenheit);
              ImGui::Checkbox("Use Miles for Wind Speed", &common->use_miles);
              ImGui::EndTabItem();
          }*/



        if (ImGui::BeginTabItem("Favorite Cities")) {
            if (ImGui::BeginChild("Favorites List", ImVec2(0, 150), true)) {
                for (const auto& city : common->favorite_cities) {
                    if (ImGui::Selectable(city.c_str())) {
                        common->set_city(city);
                        common->start_download.store(true, std::memory_order_release);
                    }
                }
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Search History")) {
            if (ImGui::BeginChild("History List", ImVec2(0, 150), true)) {
                for (const auto& city : common->search_history) {
                    if (ImGui::Selectable(city.c_str())) {
                        common->set_city(city);
                        common->start_download.store(true, std::memory_order_release);
                    }
                }
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }


    ImGui::End(); // End the ImGui window
}

// The operator function to run the DrawThread
void DrawThread::operator()(CommonObjects& common)
{
    // Start the GUI main loop, passing the DrawAppWindow function and common objects 
    // (it runs all the time and update all the window not just somthing specific)
    GuiMain(DrawAppWindow, &common);

    // Once the GUI loop exits, signal the exit flag
    common.exit_flag.store(true, std::memory_order_release);
}
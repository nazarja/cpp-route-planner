#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include <array>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;
using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::string;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if (!is)
        return std::nullopt;

    auto size = is.tellg();
    std::vector<std::byte> contents(size);

    is.seekg(0);
    is.read((char *)contents.data(), size);

    if (contents.empty())
        return std::nullopt;
    return std::move(contents);
}

// checks input is in valid range
bool IsValidInput(float value) 
{
    return (value >= 0 && value <= 100);  
}

// creates a blank line in the console
void NewConsoleLine() {
    cout << "\n" << endl;
}

// main
int main(int argc, const char **argv)
{
    std::string osm_data_file = "";
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
            if (std::string_view{argv[i]} == "-f" && ++i < argc)
                osm_data_file = argv[i];
    }
    else
    {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }

    std::vector<std::byte> osm_data;

    if (osm_data.empty() && !osm_data_file.empty())
    {
        std::cout << "Reading OpenStreetMap data from the following file: " << osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if (!data)
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }

    // blank line
    NewConsoleLine();

    // TODO 1: 
    array<float, 4> points {-1,-1,-1,-1};
    array<string, 4> labels {"start_x", "start_y", "end_x", "end_y"};

    for (int i = 0; i < labels.size(); i++)
    {

        //  loop until valid input submitted
        while(!IsValidInput(points[i]))
        {
            cout << "Enter a value between 0 and 100 for " << labels[i] << ": ";
            cin >> points[i];

            // if input is not a float or is out of range
            if (cin.fail() || !IsValidInput(points[i]))
            {
                // clear cin and remaining stream characters
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                // inform user of error and reassign points[i] from 0 to -1
                // red text hightlights error message
                cout << "\x1B[31m" << "Invalid input, please try again ..." << "\033[0m\t\t" << endl;
                
                points[i] = -1;
            }
        }
    }

    // blank line
    NewConsoleLine();

    // Build Model.
    RouteModel model{osm_data};

    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, points[0], points[1], points[2], points[3]};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface &surface) {
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface &surface) {
        render.Display(surface);
    });
    display.begin_show();
}

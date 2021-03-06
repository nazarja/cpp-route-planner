# C++ Route Planner using IO2D

This application calculates and displays a route / path between two nodes input by the user.  
It uses OpenStreetMap xml data (map.osm) and the 2d graphics library IO2D.  
The algorithm used for calculating the route based on the nodes provided is the A*Search algorithm.
User input is also validated to ensure type correctness and range is valid.
Testing is provided using gtest suite from Google.


`start_node(0,0)` is bottom left corner of map  
`end_node(100,100)` is top right corner of map

![completed route map](img/completed-route.png?raw=true "Completed Route Map")


![test completed](img/tests-complete.png?raw=true "Completed Tests")


# Installing on Fedora Linux

_Instructions for other OS's can be found here:_  
https://github.com/udacity/CppND-Route-Planning-Project

#

Make a new empty directory for the project
````
mkdir cpp-route-planner && cd cpp-route-planner
````

Clone the project from github
````
git clone git clone --recurse-submodules https://github.com/nazarja/cpp-route-planner.git  .
````

## Install Dependencies

- GCC: `sudo dnf groupinstall "Development Tools" && sudo dnf groupinstall "C Development Tools and Libraries"`
- CMake: `sudo dnf install cmake`
- Cairo: `sudo dnf install cairo-devel `
- GraphicsMagick: `sudo dnf install GraphicsMagick-devel`
- liblzma and libtiff: `sudo dnf install xz-devel libtiff-devel`
- libexpat: `sudo dnf install expat-devel`
- libjpeg: `sudo dnf install libjpeg-turbo-devel`
- libpng: `sudo apt install libpng-devel`. 
  - If `usr/include/libpng/png.h` doesn't exist then
    - `sudo mkdir /usr/include/libpng && sudo ln -s /usr/include/libpng16/png.h /usr/include/libpng/png.h`


## Install IO2D

````
cd thirdparty
git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl
cd P0267_RefImpl
mkdir Debug
cd Debug
cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" ..
cmake --build .
sudo make install
````

## Compiling and Running

### Compiling
To compile the project, first, create a `build` directory and change to that directory:
```
mkdir build && cd build
```
From within the `build` directory, then run `cmake` and `make` as follows:
```
cmake ..
make
```
### Running
The executable will be placed in the `build` directory. From within `build`, you can run the project as follows:
```
./OSM_A_star_search
```
Or to specify a map file:
```
./OSM_A_star_search -f ../<your_osm_file.osm>
```

## Testing

The testing executable is also placed in the `build` directory. From within `build`, you can run the unit tests as follows:
```
./test
```


# ProcessScope
This is a Linux system monitor built with C++ and Qt. It's currently about 85% finished. I built it to be a lightweight tool that talks directly to the kernel without a lot of extra dependencies.

## How it works
The app pulls telemetry straight from the /proc filesystem, specifically /proc/stat and /proc/meminfo.
- **OsMetricsProvider:** Handles the raw data retrieval from the filesystem.
- **Controller:** Manages the updateData loop and coordinates stats between the provider and the viewer.
- **Viewer:** The Qt interface. It uses a QSortFilterProxyModel so the process list is searchable and sortable in real-time.

## Features
- **Live Stats:** Displays Total CPU (with model info), Memory (Used vs. Total), and Network bandwidth.
- **Searchable List:** A table of active processes that you can filter by Name or PID via the search bar.
- **PID Details:** Selecting a row pulls extra metadata for that specific process, including thread counts.
- **Modular Design:** Uses standard structs for data passing and the Factory Pattern for UI component setup.

## Technical stack
- **Language:** C++17
- **Framework:** Qt
- **Platform:** Linux
- **Build:** CMake and Docker-ready

## How to build

### Create the docker image
sudo docker build -t process-scope .

#### Allow container to access host screen
xhost +local:

#### Start docker container
sudo docker compose up

#### Stop docker container
sudo docker compose down

## Context
I built this to have a lightweight, dependency-free tool for debugging high-concurrency applications on the ARMv8 and x64 platforms I work with daily.

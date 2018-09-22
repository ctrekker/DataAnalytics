# The DataAnalytics Project Central Repository
Welcome to the DataAnalytics Project's central repository! Here you will find the source code for the DataAnalytics project (v2.0). Instructions for installation can be found below.
## Development Links
2.1 Board: https://gitlab.com/ctrekker/DataAnalytics/boards?scope=all&utf8=%E2%9C%93&state=opened&label_name[]=2.1
## Installation
IMPORTANT: Installation from sources requires the following dependencies to be downloaded and installed first:
* GNU gcc/g++
* git
* node[js]
* npm
### Linux
To clone this repository, run

`git clone https://<username>@gitlab.com/ctrekker/DataAnalytics.git && cd DataAnalytics`

Build the project

`./build.bat`

Initialize the project

`./da2 init`

Install nodejs dependencies

`npm install`

To run DataAnalytics commands, start with

`./da2`

This will give you a help list for options. For usage instructions, visit the wiki in this repository.
### Windows
1. Visit the main repository page
2. Click the "Download" button on the right which is above the "Last Updated" column
3. Select "Download zip"
4. Extract the zip to the desired location
5. Navigate to the root of the source tree in file explorer
6. Execute the build.bat by double-clicking on it
7. Verify that "da2.exe" was created in the root directory
8. Navigate to the root of the source tree in windows CMD
9. Run `da2 init` to initialize the project
10. Install nodejs dependencies with `npm install`
11. To run DataAnalytics commands, start with `da2`. For more help on using DataAnalytics, see this project's wiki
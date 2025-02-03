
# Weather Dashboard

A simple C program that fetches weather data from the [OpenWeatherMap API](https://openweathermap.org/api) and displays it on the console. This program allows the user to input a city name and get the current weather, including temperature, humidity, and weather description.

## Features

- Fetches weather data using the OpenWeatherMap API.
- Displays the weather information in a clean, user-friendly format.
- Allows users to fetch weather for different cities.
- Provides basic error handling for API requests.

## Prerequisites

Before running the program, ensure that you have the following dependencies installed:

- **C Compiler**: Ensure you have a C compiler such as GCC installed.
- **libcurl**: The program uses the `libcurl` library to make HTTP requests. You can install it with the following command:

  ### For Ubuntu:
  ```bash
  sudo apt-get install libcurl4-openssl-dev
  ```

  ### For MacOS (using Homebrew):
  ```bash
  brew install curl
  ```

## How to Run

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/akoraishi2000/Weather-Dashboard.git
    ```

2. **Navigate into the project folder**:
    ```bash
    cd Weather-Dashboard
    ```

3. **Replace the API key**:
   - You need to replace the `API_KEY` in the code with your own API key from [OpenWeatherMap](https://openweathermap.org/api).
   - Update the line:
     ```c
     #define API_KEY "your-api-key-here"
     ```

4. **Compile the Code**:
    Use a C compiler such as `gcc` to compile the code. For example:
    ```bash
    gcc -o weather_dashboard weather_dashboard.c -lcurl
    ```

5. **Run the Program**:
    After compiling, you can run the program:
    ```bash
    ./weather_dashboard
    ```

    Follow the on-screen menu to fetch weather data for a city.

## Usage

Once you run the program, you will be presented with a menu:

1. **Fetch Weather**: Enter the name of the city for which you want to get the weather information.
2. **Exit**: Exit the program.

The program will fetch the weather data from the OpenWeatherMap API and display the weather details, such as:

- City Name
- Weather Description
- Temperature (in Celsius)
- Humidity (percentage)

## Example Output

```
--- Weather Dashboard Menu ---
1. Fetch Weather
2. Exit
Enter your choice: 1
Enter city name: London

--- Weather Dashboard ---
City: London
Description: clear sky
Temperature: 15.3°C
Humidity: 72.0%
-------------------------
```

## Acknowledgments

- [OpenWeatherMap](https://openweathermap.org/) for providing the weather data API.
- [libcurl](https://curl.se/libcurl/) for handling the HTTP requests.

Feel free to modify and improve this project for your own purposes. Contributions are welcome!
```

### How to Customize:
- Replace the link to the repository with the correct one if necessary.
- Adjust the license section if your project uses a different license.
- If you want to add more detailed sections, feel free to elaborate on specific functionality, installation steps, or usage tips.

Let me know if you'd like any more adjustments!

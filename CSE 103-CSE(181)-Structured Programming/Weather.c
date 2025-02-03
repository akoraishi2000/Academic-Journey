#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define API_KEY "e673564b055ccd44e14a866c48acd901" // Replace with your OpenWeatherMap API key
#define BASE_URL "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric"

// Struct to store weather data
typedef struct {
    char city[100];
    char description[100];
    float temperature;
    float humidity;
} WeatherData;

// Callback function to handle API response
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    char *response = (char *)ptr;
    WeatherData *weather = (WeatherData *)userdata;

    // Parse the JSON response (simple parsing for demonstration)
    char *desc = strstr(response, "\"description\":\"");
    if (desc) {
        sscanf(desc, "\"description\":\"%[^\"]", weather->description);
    }

    char *temp = strstr(response, "\"temp\":");
    if (temp) {
        sscanf(temp, "\"temp\":%f", &weather->temperature);
    }

    char *humidity = strstr(response, "\"humidity\":");
    if (humidity) {
        sscanf(humidity, "\"humidity\":%f", &weather->humidity);
    }

    return size * nmemb;
}

// Function to fetch weather data
int fetch_weather(const char *city, WeatherData *weather) {
    CURL *curl;
    CURLcode res;
    char url[256];

    // Construct the API URL
    snprintf(url, sizeof(url), BASE_URL, city, API_KEY);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, weather);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
            return 1; // API request failed
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0; // Success
}

// Function to display weather data
void display_weather(const WeatherData *weather) {
    printf("\n--- Weather Dashboard ---\n");
    printf("City: %s\n", weather->city);
    printf("Description: %s\n", weather->description);
    printf("Temperature: %.1f°C\n", weather->temperature);
    printf("Humidity: %.1f%%\n", weather->humidity);
    printf("-------------------------\n");
}

// Main function
int main() {
    WeatherData weather = {0};
    char city[100];
    int choice;

    while (1) {
        // Display menu
        printf("\n--- Weather Dashboard Menu ---\n");
        printf("1. Fetch Weather\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            // Get city name from user
            printf("Enter city name: ");
            scanf("%s", city);
            strcpy(weather.city, city);

            // Fetch weather data
            if (fetch_weather(city, &weather) == 0) {
                display_weather(&weather);
            } else {
                printf("Failed to fetch weather data for %s.\n", city);
            }
        } else if (choice == 2) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

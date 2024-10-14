#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

// Function to convert string to Unix time in milliseconds with a fractional second part
double stringToUnixTime(const std::string& str) {
    // Parse the string into a time_point
    std::tm tm = {};
    std::istringstream ss(str.substr(0, 19)); // Remove the fractional second part
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse time string");
    }
    // Convert the time_point to Unix time in seconds
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    // Add the fractional second part
    auto fractionalSeconds = std::stod(str.substr(20)); // Get the fractional second part
    double milliseconds = seconds  + fractionalSeconds/1000;
    return milliseconds;
}

int main() {
    std::string str = "2024-09-26T14:54:20.276";
    double unixTimeMs = stringToUnixTime(str);
    std::cout << "Unix time in milliseconds: " << std::fixed ;
    std::cout << unixTimeMs << std::endl;
    return 0;
}

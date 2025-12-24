#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std; 

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config() {}

void Config::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        throw invalid_argument("Could not open the input file");
    }

    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;  // Skip comments and empty lines

        // Remove '=' if it exists and split by space or '='
        size_t pos = line.find('=');
        if (pos != string::npos) {
            line.replace(pos, 1, " "); // Replace '=' with a space
        }

        istringstream iss(line);
        string key;
        double value;
        if (iss >> key >> value) {
            data[key] = value;
        } else {
            cerr << "Warning: Invalid line in config file: " << line << endl;
        }
    }
}

double Config::get(const string& key, double defaultValue) {
    return data.count(key) ? data[key] : defaultValue;
}

string Config::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

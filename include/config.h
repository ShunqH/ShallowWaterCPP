#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>

class Config {
public:
    static Config& getInstance();
    void loadFromFile(const std::string& filename);
    double get(const std::string& key, double defaultValue = 0.0);

private:
    Config();  
    std::map<std::string, double> data;
    static std::string trim(const std::string& str);
};

#endif // CONFIG_H

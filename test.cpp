#include <map>
#include <string>
#include <algorithm>
#include <iostream>

template <typename K, typename V>
V maxElement(std::map<K, V>& map, const V& defaultValue){
    return map.empty()? defaultValue :std::max_element(map.begin(), map.end(),
    [](const auto& a, const auto& b){return a.second < b.second;})->second;
}




int main() {
    std::map<std::string, int> myMap = {};
    int defaultValue = -1; // Значение по умолчанию


    auto maxIter = maxElement(myMap, defaultValue);
    std::cout<<"maxITer"<<maxIter;

    return 0;
}
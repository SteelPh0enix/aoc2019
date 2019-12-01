#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "file_reader.hpp"

unsigned calculateNeededFuel(unsigned weight) {
  return static_cast<unsigned>(
      std::floor((static_cast<double>(weight) / 3.) - 2.));
}

int main() {
  std::fstream data_file("input.txt");
  if (!data_file.is_open()) {
    std::cout << "Couldn't open file with data!\n";
    return 1;
  }

  std::vector<unsigned> weigths{};
  readDataLines(data_file, std::back_inserter(weigths),
                [](std::string const& line) { return std::stoul(line); });

  std::cout << "Read " << weigths.size() << " weights.\n";

  unsigned fuel_sum{0};
  std::for_each(weigths.begin(), weigths.end(), [&fuel_sum](unsigned weight) {
    fuel_sum += calculateNeededFuel(weight);
  });

  std::cout << "Needed fuel: " << fuel_sum << '\n';
}
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
#include <data_reader.hpp>

using WeightT = std::int64_t;

constexpr char const* CSV_FILE = "weights.csv";
constexpr char const* CSV_HEADER =
    "Module weight;Fuel needed;Fuel needed for fuel;Summaric fuel weight;Whole weight\n";
constexpr char const* CSV_FOOTER = "Summary:;;;;\n";

template <typename T>
struct ModuleFullWeight {
  T module_weight;
  T fuel_needed;
  T fuel_needed_for_fuel;
  T summaric_fuel_weight;
  T whole_weight;

  ModuleFullWeight<T>& operator+=(ModuleFullWeight<T> const& right) {
    module_weight += right.module_weight;
    fuel_needed += right.fuel_needed;
    fuel_needed_for_fuel += right.fuel_needed_for_fuel;
    summaric_fuel_weight += right.summaric_fuel_weight;
    whole_weight += right.whole_weight;
    return *this;
  }
};

template <typename T>
void moduleWeightToCSV(ModuleFullWeight<T> const& weight,
                       std::ostream& output) {
  output << std::to_string(weight.module_weight) << ';'
         << std::to_string(weight.fuel_needed) << ';'
         << std::to_string(weight.fuel_needed_for_fuel) << ';'
         << std::to_string(weight.summaric_fuel_weight) << ';'
         << std::to_string(weight.whole_weight) << '\n';
}

template <typename T>
T calculateNeededFuel(T weight) {
  return static_cast<T>(std::floor((static_cast<double>(weight) / 3.) - 2.));
}

template <typename T>
T calculateFuelNeededForFuel(T fuel_weight) {
  T fuel_for_fuel = calculateNeededFuel(fuel_weight);
  if (fuel_for_fuel > 0) {
    return fuel_for_fuel + calculateFuelNeededForFuel(fuel_for_fuel);
  } else {
    return 0;
  }
}

template <typename T>
std::vector<T> readWeightsFromFile(std::string const& filename) {
  std::fstream data_file(filename);
  std::vector<T> weigths{};

  if (!data_file.is_open()) {
    std::cout << "Couldn't open file with data!\n";
    return weigths;
  }

  readDataLines(data_file, std::back_inserter(weigths),
                [](std::string const& line) { return std::stoul(line); });
  return weigths;
}

template <typename T, typename It>
T calculateCompleteFuelWeight(It modules_start, It modules_end) {
  std::fstream csv_output(CSV_FILE, std::ios::out | std::ios::trunc);
  if (csv_output.is_open()) {
    csv_output << CSV_HEADER;
  }

  ModuleFullWeight<T> modules_summary{};

  for (auto module = modules_start; module != modules_end; module++) {
    ModuleFullWeight<T> weight{};
    weight.module_weight = *module;
    weight.fuel_needed = calculateNeededFuel(weight.module_weight);
    weight.fuel_needed_for_fuel = calculateFuelNeededForFuel(weight.fuel_needed);
    weight.summaric_fuel_weight = weight.fuel_needed + weight.fuel_needed_for_fuel;
    weight.whole_weight =
        weight.module_weight + weight.fuel_needed + weight.fuel_needed_for_fuel;

    if (csv_output.is_open()) {
      moduleWeightToCSV(weight, csv_output);
    }
    modules_summary += weight;
  }

  csv_output << CSV_FOOTER;
  moduleWeightToCSV(modules_summary, csv_output);
  return modules_summary.summaric_fuel_weight;
}

int main() {
  auto modules_weight = readWeightsFromFile<WeightT>("input.txt");
  auto fuel_needed = calculateCompleteFuelWeight<WeightT>(
      std::begin(modules_weight), std::end(modules_weight));
  std::cout << "Fuel needed: " << fuel_needed << "\nCheck " << CSV_FILE
            << " for details\n";
}
#include <istream>
#include <string>

template <typename OutputIt, typename ConvertF>
std::size_t readDataLines(std::istream& source,
                          OutputIt output,
                          ConvertF convert) {
  std::size_t lines{0};

  for (std::string line{}; std::getline(source, line);) {
    output = convert(line);
    lines++;
  }

  return lines;
}

template <typename OutputIt, typename ConvertF>
std::size_t readSeparatedData(std::istream& source,
                              char separator,
                              OutputIt output,
                              ConvertF convert) {
  std::size_t data_amount{0};

  for (std::string data_sample{};
       std::getline(source, data_sample, separator);) {
    output = convert(data_sample);
    data_amount++;
  }

  return data_amount;
}
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
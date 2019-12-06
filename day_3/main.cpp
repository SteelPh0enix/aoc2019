/*
  Task: Find Manhattan distance between starting point and closest intersection
  point

  Data: Taxicab paths of two lines, possibly intersecting in several
  points

  Method: Find all interseciton points, calculate distances and pick
  smallest one

  How do i find all intersection points?
  Naive method:
    * Make n*m integer matrix, preferably dynamically resizable
    * Draw paths of lines in matrix
    * While parsing the line we want to check intersections with, check the
      matrix data for points intersecting with other lines
    * Calculate distance from origin for each found intersection point, and put
      it in buffer
    * Sort the buffer and get closest distance

  Better metod:
    * Use std::unordered_map to reduce memory complexity needed from n*m (where
      n and m are farthest points) to n (where n is length of path in points)
    * Make the same steps as in naive method, making computational complexity
      pretty much linear
*/

#include <data_reader.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct PathVector {
  enum class Direction : char { Left = 'L', Right = 'R', Up = 'U', Down = 'D' } direction;
  unsigned long length;

  PathVector(std::string const& str) {
    if (str.length() >= 2) {
      direction = static_cast<Direction>(str[0]);
      length = std::strtoul(&str[1], nullptr, 10);
    }
  }

};

std::vector<PathVector> parsePathInput(std::istream& input) {
  std::vector<PathVector> paths;

  for (std::string path_string{}; std::getline(input, path_string, ',');) {
    paths.emplace_back(path_string);
  }

  return paths;
}

std::vector<std::vector<PathVector>> parseInputData(
    std::string const& input_filename) {
  std::vector<std::vector<PathVector>> paths{};
  std::vector<std::stringstream> lines_streams{};
  std::fstream input_file(input_filename, std::ios::in);

  if (!input_file.is_open()) {
    std::cout << "Couldn't open input file!\n";
    return paths;
  }

  readDataLines(input_file, std::back_inserter(lines_streams),
                [](std::string const& str) { return std::stringstream(str); });

  for (auto& path : lines_streams) {
    paths.push_back(parsePathInput(path));
  }

  return paths;
}

int main() {
  auto paths = parseInputData("input.txt");

  for (auto const& path: paths) {
    std::cout << "Path: ";
    for (auto const& vec: path) {
      std::cout << static_cast<char>(vec.direction) << " " << vec.length << ", ";
    }
    std::cout << "\n\n";
  }
}
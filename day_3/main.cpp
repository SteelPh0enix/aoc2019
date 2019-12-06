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
#include <utility>
#include <vector>

struct Point {
  int x{}, y{};

  bool operator==(Point const& second) const {
    return x == second.x && y == second.y;
  }
};

// Custom std::hash specialisation copied from cppreference :^)
namespace std {
  template<> struct hash<Point> {
    std::size_t operator()(Point const& point) const noexcept {
      std::size_t hash_x = std::hash<int>{}(point.x);
      std::size_t hash_y = std::hash<int>{}(point.y);
      return hash_x ^ (hash_y << 1);
    }
  };
}

using LinesMap = std::unordered_map<Point, int>;
using MoveFunc = Point (*)(Point);

struct PathVector {
  enum class Direction : char {
    Left = 'L',
    Right = 'R',
    Up = 'U',
    Down = 'D'
  } direction{};
  unsigned long length{};

  PathVector(std::string const& str) {
    if (str.length() >= 2) {
      direction = static_cast<Direction>(str[0]);
      length = std::strtoul(&str[1], nullptr, 10);
    }
  }
};

Point moveLeft(Point origin) {
  return {origin.x - 1, origin.y};
}

Point moveRight(Point origin) {
  return {origin.x + 1, origin.y};
}

Point moveUp(Point origin) {
  return {origin.x, origin.y + 1};
}

Point moveDown(Point origin) {
  return {origin.x, origin.y - 1};
}

MoveFunc getMoveFuncForDirection(PathVector::Direction direction) {
  using Dir = PathVector::Direction;
  switch (direction) {
    case Dir::Left:
      return moveLeft;
    case Dir::Right:
      return moveRight;
    case Dir::Up:
      return moveUp;
    case Dir::Down:
      return moveDown;
    default:
      return [](Point p) { return p; };
  }
}

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

void drawPathOnMap(LinesMap& map, std::vector<PathVector> const& vectors) {
  Point origin{0, 0};
  map[origin]++;

  std::cout << "Drawing from " << origin.x << ", " << origin.y << "\n";

  for (auto const& vec : vectors) {
    auto move = getMoveFuncForDirection(vec.direction);
    for (unsigned i = 0; i < vec.length; i++) {
      origin = move(origin);
      map[origin]++;
      std::cout << "Point " << origin.x << ", " << origin.y << " drawed " << map[origin] << " times\n";
    }
  }

  std::cout << "Point amount: " << map.size() << '\n';
}

int main() {
  auto paths = parseInputData("input.txt");
  LinesMap map{};

  drawPathOnMap(map, paths[0]);
}
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

#include <algorithm>
#include <array>
#include <cmath>
#include <data_reader.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <std::size_t N>
struct Intersection {
  int x{}, y{};
  std::array<unsigned, N> line_distances{};

  unsigned manhattanDistanceFromOrigin() const {
    return static_cast<unsigned>(std::abs(x) + std::abs(y));
  }

  unsigned combinedLinesLength() const {
    return std::accumulate(line_distances.cbegin(), line_distances.cend(), 1u);
  }

  bool operator==(Intersection const& second) const {
    return x == second.x && y == second.y;
  }
};

template <std::size_t N>
struct IntersectionHasher {
  std::size_t operator()(Intersection<N> const& val) const noexcept {
    std::size_t h1 = std::hash<int>{}(val.x);
    std::size_t h2 = std::hash<int>{}(val.y);
    return h1 ^ (h2 << 1);
  }
};

template <std::size_t N>
using LinesMap =
    std::unordered_map<Intersection<N>, int, IntersectionHasher<N>>;

template <std::size_t N>
using MoveFunc = Intersection<N> (*)(Intersection<N>);

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

template <std::size_t N>
Intersection<N> moveLeft(Intersection<N> origin) {
  return {origin.x - 1, origin.y};
}

template <std::size_t N>
Intersection<N> moveRight(Intersection<N> origin) {
  return {origin.x + 1, origin.y};
}

template <std::size_t N>
Intersection<N> moveUp(Intersection<N> origin) {
  return {origin.x, origin.y + 1};
}

template <std::size_t N>
Intersection<N> moveDown(Intersection<N> origin) {
  return {origin.x, origin.y - 1};
}

template <std::size_t N>
MoveFunc<N> getMoveFuncForDirection(PathVector::Direction direction) {
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
      return [](Intersection<N> p) { return p; };
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

template <std::size_t N>
void drawPathOnMap(LinesMap<N>& map,
                   std::vector<PathVector> const& vectors,
                   std::size_t line_number) {
  Intersection<N> origin{0, 0};
  map[origin]++;
  unsigned line_length{0};

  for (auto const& vec : vectors) {
    auto move = getMoveFuncForDirection<N>(vec.direction);
    for (unsigned i = 0; i < vec.length; i++) {
      origin = move(origin);
      origin.line_distances[line_number] = line_length++;
      map[origin]++;
    }
  }

  std::cout << "Intersections amount: " << map.size() << '\n';
}

template <std::size_t N>
std::vector<Intersection<N>> getAllCrossings(
    LinesMap<N> const& map,
    std::vector<PathVector> const& vectors,
    std::size_t line_number) {
  std::vector<Intersection<N>> points{};
  Intersection<N> origin{0, 0};
  unsigned line_length{0};

  for (auto const& vec : vectors) {
    auto move = getMoveFuncForDirection<N>(vec.direction);
    for (unsigned i = 0; i < vec.length; i++) {
      origin = move(origin);
      line_length++;
      auto intersection = map.find(origin);
      if (intersection != map.cend()) {
        auto intersection_copy = (*intersection).first;
        intersection_copy.line_distances[line_number] = line_length;
        points.push_back(intersection_copy);
      }
    }
  }

  return points;
}

int main() {
  auto paths = parseInputData("input.txt");
  LinesMap<2> map{};

  drawPathOnMap(map, paths[0], 0);
  auto crossings = getAllCrossings(map, paths[1], 1);
  std::sort(crossings.begin(), crossings.end(),
            [](auto const& first, auto const& second) {
              return first.manhattanDistanceFromOrigin() <
                     second.manhattanDistanceFromOrigin();
            });

  std::cout << "Nearest crossing: (" << crossings[0].x << ", " << crossings[0].y
            << "), with distance " << crossings[0].manhattanDistanceFromOrigin()
            << " and wires length " << crossings[0].combinedLinesLength()
            << "\n";

  std::sort(crossings.begin(), crossings.end(),
            [](auto const& first, auto const& second) {
              return first.combinedLinesLength() < second.combinedLinesLength();
            });

  std::cout << "Shortest crossing: (" << crossings[0].x << ", "
            << crossings[0].y << "), with distance "
            << crossings[0].manhattanDistanceFromOrigin()
            << " and wires length " << crossings[0].combinedLinesLength()
            << "\n";
}
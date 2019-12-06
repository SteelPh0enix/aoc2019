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
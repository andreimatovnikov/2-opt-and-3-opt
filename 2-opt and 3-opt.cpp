#include <iostream>
#include <vector>


struct Point {
    double x;
    double y;
};

double cord_distance(const Point& a, const Point& b) {
    return hypot(a.x - b.x, a.y - b.y);
}

double path_length(std::vector<int>& tour, std::vector<Point>& points, bool cycle) {
    double total = 0.0;
    for (int i = 0; i < tour.size() - 1; ++i)
        total += cord_distance(points[tour[i]], points[tour[i + 1]]);
    if (cycle)
        total += cord_distance(points[tour.back()], points[tour[0]]);
    return total;
}


bool two_opt(std::vector<int>& tour, std::vector<Point>& points) {
    bool improved = false;
    for (int i = 1; i < tour.size() - 2; ++i) {
        for (int j = i + 1; j < tour.size() - 1; ++j) {
            double dist1 = cord_distance(points[tour[i - 1]], points[tour[i]]) +
                cord_distance(points[tour[j]], points[tour[j + 1]]);
            double dist2 = cord_distance(points[tour[i - 1]], points[tour[j]]) +
                cord_distance(points[tour[i]], points[tour[j + 1]]);
            if (dist2 < dist1) {
                reverse(tour.begin() + i, tour.begin() + j + 1);
                improved = true;
            }
        }
    }
    return improved;
}


bool three_opt(std::vector<int>& tour, std::vector<Point>& points) {
    bool improved = false;
    for (int i = 0; i < tour.size() - 5; ++i) {
        for (int j = i + 2; j < tour.size() - 3; ++j) {
            for (int k = j + 2; k < tour.size() - 1; ++k) {
                std::vector<int> cur = 
                { tour[i], tour[i + 1], tour[j], tour[j + 1], tour[k], tour[k + 1] };
                double prev_dist =
                    cord_distance(points[cur[0]], points[cur[1]]) +
                    cord_distance(points[cur[2]], points[cur[3]]) +
                    cord_distance(points[cur[4]], points[cur[5]]);

                std::vector<std::vector<int>> variants = {
                    { cur[0], cur[2], cur[1], cur[4], cur[3], cur[5] },
                    { cur[0], cur[3], cur[4], cur[1], cur[2], cur[5] },
                };

                for (auto& v : variants) {
                    double new_dist =
                        cord_distance(points[v[0]], points[v[1]]) +
                        cord_distance(points[v[2]], points[v[3]]) +
                        cord_distance(points[v[4]], points[v[5]]);
                    if (new_dist < prev_dist) {
                        reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                        reverse(tour.begin() + j + 1, tour.begin() + k + 1);
                        improved = true;
                        return true;  
                    }
                }
            }
        }
    }
    return improved;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i)
        std::cin >> points[i].x >> points[i].y;

    std::vector<int> tour(n);
    for (int i = 0; i < n; ++i)
        tour[i] = i;

   std::vector<bool> visited(n, false);
    tour[0] = 0;
    visited[0] = true;
    for (int i = 1; i < n; ++i) {
        int prev = tour[i - 1];
        double bestDist = 10000000;
        int next = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j]) {
                double d = cord_distance(points[prev], points[j]);
                if (d < bestDist) {
                    bestDist = d;
                    next = j;
                }
            }
        }
        tour[i] = next;
        visited[next] = true;
    }

    bool improved;
    do {
        improved = two_opt(tour, points);
        if (!improved)
            improved = three_opt(tour, points);
    } while (improved);

    double final_length = path_length(tour, points, true);
    std::cout << final_length << 1 << std::endl;  
    for (int i = 0; i < tour.size(); ++i)
        std::cout << tour[i] << " ";
    std::cout << std::endl;

    return 0;
}

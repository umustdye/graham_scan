/* 
 * File:   Original Graham Scan
 * Author: Heidi Dye
 *
 * Created on March 10, 2022, 9:27 PM
 */


/* POSSIBLE OPTIMIZATIONS
 *
 * - Reduce Hull
 *      - The current algorithm takes all points along edges. You really only need points at the
 *        corners of the hull.
 *        If you only take edges at corners, you can ignore all collinear points with shorter
 *        distance because they are guaranteed to not be on the hull
 *
 * - Swap vectors with pointers
 * - Make function calls for cross / dist inline
 */
#include <bits/stdc++.h>
#include "get_time.h"

using namespace std;

typedef long long ll;

struct Point {
    ll x;
    ll y;
    void operator=(const Point &rhs) {
        this->x = rhs.x;
        this->y = rhs.y;
    }
    Point operator-(const Point &rhs) const {
        Point p = {this->x - rhs.x, this->y - rhs.y};
        return p;
    }
};
inline ll cross(Point origin, Point a, Point b);
inline ll dist(Point a, Point b);
void output(const vector<Point> & v);

int main() {
    ios::sync_with_stdio(false);
    
    string file_name;
    cin >> file_name;
    ifstream file(file_name);
    
    if(file.is_open())
    {
        ll n = 0;

        file >> n;
   
        ll begin_time;
        ll end_time;
        // Read in and find bottom left point
        vector<Point> points(n);
        Point lowest = {numeric_limits<ll>::max(), numeric_limits<ll>::max()};
        ll lowest_index = 0;
        for(ll i = 0; i < n; ++i) {
            file >> points[i].x >> points[i].y;
            if(points[i].y < lowest.y || (points[i].y == lowest.y && points[i].x < lowest.x)) {
                lowest = points[i];
                lowest_index = i;
            }
        }
        
        //close the file
        file.close();

        //get the beginning time for timing analysis
        timer t; 
        t.start();

        
        // Sort remaining n - 1 points by polar angle first, distance if collinear, around lowest point
        auto comp = [&](const Point &a, const Point &b) -> bool {
            ll orientation = cross(points[0], a, b);
            return (orientation == 0) ? dist(points[0], a) < dist(points[0], b) : orientation > 0;
        };
        swap(points[0], points[lowest_index]);
        sort(points.begin() + 1, points.end(), comp);

        // Reverse last points collinear around lowest point
        int rev_ind = points.size() - 1;
        while(cross(points[0], points[rev_ind-1], points[rev_ind]) == 0) {
            --rev_ind;
        }
        reverse(points.begin() + rev_ind, points.end());


        // Check if hull possible
        if(points.size() < 3) {
            cout<<"Convex Hull not possible\n";
            return 0;
        }

        // Create convex hull
        vector<Point> hull;
        for(unsigned int i = 0; i < points.size(); ++i) {
            while(hull.size() > 1 && cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) < 0) {
                hull.pop_back();
            }
            hull.push_back(points[i]);
        }
        
        //get the end time
        t.stop();
        double tm = t.get_total();
        
        cout << "Total Time for " << n << " Samples: " << tm << endl;

        // Print contents of hull
        cout<<hull.size()<<endl;
        output(hull);       
    }


    return 0;
}


inline ll cross(Point origin, Point a, Point b) { // Positive = counter clockwise
    Point A = a - origin, B = b - origin;   // Zero = collinear
    return (A.x * B.y) - (B.x * A.y);       // Negative = clockwise
}

inline ll dist(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void output(const vector<Point> & v) {
    for(unsigned int i = 0; i < v.size(); ++i) {
        cout<<v[i].x<<" "<<v[i].y<<"\n";
    }
}
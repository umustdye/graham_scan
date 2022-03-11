/* 
 * File:   Preprocessing Threshold Graham Scan
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
        //cin >> n;
   
        
        ll begin_time;
        ll end_time;
        // Read in and find bottom left point
        vector<Point> points(n);
        Point lowest = {numeric_limits<ll>::max(), numeric_limits<ll>::max()};
        ll lowest_index = 0;
        for(ll i = 0; i < n; ++i) {
            file >> points[i].x >> points[i].y;
            //cin >> points[i].x >> points[i].y;
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

        //get y min to beginning of vector
        swap(points[0], points[lowest_index]);
        int threshold = 1000;
        vector<Point> points_opt;
        if(n > threshold)
        {
            //Preprocess points that will definitely not a part of the convex hull
             Point minU = points[0];
             Point minL = points[0];
             Point maxU = points[0];
             Point maxL = points[0];
             ll minUval = minU.x - minU.y;
             ll minLval = minL.x + minL.y;
             ll maxUval = maxU.x + maxU.y;
             ll maxLval = maxL.x - maxL.y;
             ll xMin = 0;
             ll yMin = 0;
             ll xMax = 0;
             ll yMax = 0;
             vector<Point> points_temp;
             for(int i=0; i<n; i++) {
                 Point v = points[i];
                 ll x = v.x;
                 ll y = v.y;
                 if (!(x > xMin && x < xMax && y > yMin && y < yMax)) {
                     points_temp.push_back(v);
                     ll sum = x + y;
                     ll diff = x - y;
                     if (diff < minUval) minU = v;
                     if (diff > maxLval) maxL = v;
                     if (sum < minLval) minL = v;
                     if (sum > maxUval) maxU = v;
                     minUval = minU.x - minU.y;
                     maxLval = maxL.x - maxL.y;
                     minLval = minL.x + minL.y;
                     maxUval = maxU.x + maxU.y;
                     xMin = max(minU.x, minL.x);
                     yMin = max(minL.y, maxL.y);
                     xMax = min(maxU.x, maxL.x);
                     yMax = min(minU.y, maxU.y);
                 }
             }

             //vector<Point> points_opt;
             //do one more filtering pass on optimized points
             for(int i=0; i<points_temp.size(); i++) {
                 Point v = points_temp[i];
                 ll x = v.x;
                 ll y = v.y;
                 if (!(x > xMin && x < xMax && y > yMin && y < yMax))
                     points_opt.push_back(v);
             }


             //continue with the rest of the convex hull
             cout << "New size of points after preprocessing: "<< points_opt.size() << endl;         
        }
        
        else
        {
            points_opt = points;
        }

        
        
        // Sort remaining n - 1 points by polar angle first, distance if collinear, around lowest point
        auto comp = [&](const Point &a, const Point &b) -> bool {
            ll orientation = cross(points[0], a, b);
            return (orientation == 0) ? dist(points[0], a) < dist(points[0], b) : orientation > 0;
        };
        
        sort(points_opt.begin() + 1, points_opt.end(), comp);

        // Reverse last points collinear around lowest point
        int rev_ind = points_opt.size() - 1;
        while(cross(points_opt[0], points_opt[rev_ind-1], points_opt[rev_ind]) == 0) {
            --rev_ind;
        }
        reverse(points_opt.begin() + rev_ind, points_opt.end());


        // Check if hull possible
        if(points_opt.size() < 3) {
            cout<<"Convex Hull not possible\n";
            return 0;
        }

        // Create convex hull
        vector<Point> hull;
        for(unsigned int i = 0; i < points_opt.size(); ++i) {
            while(hull.size() > 1 && cross(hull[hull.size()-2], hull[hull.size()-1], points_opt[i]) < 0) {
                hull.pop_back();
            }
            hull.push_back(points_opt[i]);
        }
        
        //get the end time
        t.stop();
        double tm = t.get_total();
        
        cout << "Total Time for " << n << " Samples: " << tm << endl;

        /*// Print contents of hull
        cout<<hull.size()<<endl;
        output(hull);*/       
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
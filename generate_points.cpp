/* 
 * File:   Generate Points
 * Author: h1999
 * Purpose: Generate a certain amount of ll 2d coordinates and write to a text file
 * Created on March 10, 2022, 9:31 PM
 */


#include <bits/stdc++.h>


using namespace std;

typedef long long ll;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    srand(static_cast<unsigned int>(time(0)));
    //take in how many points you want
    ll n;
    cin >> n;
    
    ll offset = 1000000;
    ll max = 2000000;
    vector<pair<int, int>> points(n);
    string file_name = "data_points_" + to_string(n) + "_.txt";
    ofstream file(file_name);
    if(file.is_open())
    {
        file << n << endl;
        for(int i=0; i<n; i++)
        {
            points[i].first = (rand() % max) - offset;
            points[i].second = (rand() % max) - offset;

            file << points[i].first << " " << points[i].second << endl;
        }

        file.close();    
    }

    
    
    
    

    return 0;
}


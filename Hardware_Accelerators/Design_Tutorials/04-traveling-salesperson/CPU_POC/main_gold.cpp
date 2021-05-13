/**
 * 
 * Build with:
 *  g++ -O3 main_gold.cpp && ./a.out
 * 
 */


#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <iostream>


struct Coord
{
    float x, y;
};

constexpr long int factorial(const int N)
{
    return ( N == 1 ? 1 : N * factorial( N-1 ));
}

constexpr int N = 13;
uint32_t getShortestDistance(const std::array<uint32_t, N*N>& distances)
{
    uint32_t ret = std::numeric_limits<uint32_t>::max();

    std::array<uint8_t, N> perm{0,1,2,3,4,5,6,7,8,9,10,11,12};

    for( uint64_t i = 0; i < factorial(N) ; ++i )
    {
        uint32_t distance = 0;
        for ( int j = 0; j < N-1; ++j)
            distance += distances[perm[j]*N+perm[j+1]];
    
        ret = std::min(ret, distance);

        std::next_permutation(perm.begin(), perm.end());

        // hacky cheap progress indicator
        if ( i%(factorial(N)/500) == 0) std::cout << i*1000/factorial(N)/10.0 << "%\r" << std::flush;
    }
    return ret;
}


int main()
{
    const std::array<unsigned int, N*N> distances = [](){

        // Use an IIFE to initialize the array of distances between pairs of cities.
        // Scaling may look a bit strange, but this is to ensure consistency  
        // between the software design and the HLS design

        const Coord cities[] = {{40.7127837, -74.0059413 },
                                {34.0522342, -118.2436849},
                                {41.8781136, -87.6297982 },
                                {29.7604267, -95.3698028 },
                                {39.9525839, -75.1652215 },
                                {33.4483771, -112.0740373},
                                {29.4241219, -98.4936282 },
                                {32.715738,  -117.1610838},
                                {32.7766642, -96.7969879 },
                                {37.3382082, -121.8863286},
                                {30.267153,  -97.7430608 },
                                {39.768403,  -86.158068  },
                                {30.3321838, -81.655651  },
                                {37.7749295, -122.4194155},
                                {39.9611755, -82.9987942 }
                                };
        
        float preciseDistances[N*N];

        float maxDist = 0;

        for( short int i = 0; i < N; ++i )
        {
            for( short int j = 0; j < N; ++j )
            {
                float dist = std::sqrt( 
                                        std::pow(cities[i].x-cities[j].x, 2) 
                                    + std::pow(cities[i].y-cities[j].y, 2) 
                                    );

                preciseDistances[i*N+j] = dist;
                maxDist = std::max( maxDist, dist);                                       
            }
        }

        std::array<unsigned int, N*N> ret;

        for( unsigned int i = 0; i < N*N; ++i )
                ret[i] = (preciseDistances[i]/maxDist) * std::numeric_limits<uint16_t>::max();

        return ret;

    }();

    std::cout << getShortestDistance(distances) << std::endl;
}



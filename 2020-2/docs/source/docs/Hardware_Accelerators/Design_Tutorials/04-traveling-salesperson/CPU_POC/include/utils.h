#include <iostream> 
#include <array>

/**
 * Helper class to print std::arrays
 *
 * cout << std::array<int, 5> << std::endl;
 */

template<typename T, unsigned long NN>
std::ostream& operator << (std::ostream& lhs, const std::array<T, NN>& rhs)
{
    for ( const auto& v: rhs )
        lhs << v << " ";
    return lhs;
}

/**
 * Calculate a factorial ( at compile time)
 */

constexpr long int factorial(const int N)
{
    return ( N <= 1 ? 1 : N * factorial( N-1 ));
}

/**
 * Calculate an array of factorials ( at compile time)
 */

template<unsigned long NN>
constexpr auto precalcFactorials()->std::array<long int, NN>
{
        std::array<long int, NN> ret{};
        for (int i = 0; i < NN; ++i ) ret[i] = factorial(i);
        return ret;
}

/**
 * Calculate the i'th permutation of an integer sequence of length N
 */

template<int N>
constexpr std::array<size_t, N> getPermutation(int i)
{
	
      constexpr std::array<long int, N> factorials = precalcFactorials<N>();
	  std::array<size_t, N> ret{0};

	  for ( char n = N-1; n >= 0; --n )
	  {
		for( char ii = 0; ii < n; ++ii )
		{
		  if ( i >= factorials[n] )
		  {
			ret[N-n-1]++;
			i -= factorials[n];
		  }
		}
	  }

	  for (char k = N - 1; k > 0; --k)
		for (char j = k - 1; j >= 0; --j)
			if (ret[j] <= ret[k])
				ret[k]++;
	return ret;
}

template<int N>
constexpr std::array<size_t, N> getPermutationSwap(int nSwap, int i)
{
	
    constexpr std::array<long int, N> factorials = precalcFactorials<N>();

	std::array<size_t, N> perm{0};
	for( int j = 0; j < N; ++j ) 
		perm[j] = N-j-1;

	for( int j = 0; j < nSwap ; ++j )
		std::swap(perm[j], perm[i/factorials[j]%(j+1)]);

	return perm;

}
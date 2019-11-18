#ifndef BINPOTS_HEADER
#define BINPOTS_HEADER

/* Binary : Powers of Two */
/* Some convenient Constants */

namespace rcl{
template <unsigned int N> const unsigned int& pot = pot<N-1>*2;
template <> const unsigned int pot<0> = 1;
template <unsigned int N> const unsigned int& msk = msk<N-1>+pot<N-1>;
template <> const unsigned int msk<0> = 0;
template <unsigned int N> const unsigned int& npot = ~pot<N>;
template <unsigned int N> const unsigned int& nmsk = ~msk<N>;
template <unsigned int x,unsigned int y> const unsigned int& min = x<y ? x : y;
template <unsigned int x,unsigned int y> const unsigned int& minpot = x<y ? pot<x> : pot<y>;
template <unsigned int x,unsigned int y> const unsigned int& minmsk = x<y ? msk<x> : msk<y>;

}
#endif

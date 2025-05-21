#include <cstddef>

#include "../common.h"
#include "rand_util.h"

/*
  Install your PRNG so that PRNG_SEED(src, len), PRNG_FUNCTION() and
  PRNG_MAXIMUM are defined. PRNG_FUNCTION() outputs an integer type whose range
  is covered by the double type. Its range is between 0 and PRNG_MAXIMUM
  inclusive.

  Like ANSI C's RAND_MAX it is assumed that PRNG_MAXIMUM >= 32767 otherwise
  dRand() will not work as intended.
*/

static void xseed(const void* src, size_t len) {
  unsigned int seed = 0;
  if (len > sizeof(seed)) {
    len = sizeof(seed);
  }
  if (len > 0) {
    memcpy(&seed, src, len);
  }
  srand(seed);
}

// What baseline built in random function to use
#define PRNG_FUNCTION() rand()
// Alias for xseed
#define PRNG_SEED(src, len) xseed(src, len)
// Alias for PRNG_MAXIMUM >= 32767
#define PRNG_MAXIMUM RAND_MAX
#define RS_SCALE (1.0 / (1.0 + PRNG_MAXIMUM))

/*
  High quality random number generation functions below this line. Each function introduces no
  accuracy issues beyond inherent double precision accuracy limits. Hence the
  probability distributions should always be considered accurate to within a few ulp.
*/

/* =============================================================================================== */

// Returns 0 or 1 with the probability x for picking 0.
int randutil::RandBiased(double x) {
  for (;;) {
    long double randscale = PRNG_FUNCTION() * RS_SCALE;
    if (randscale >= x) {
      return 0;
    }
    if (randscale + RS_SCALE <= x) {
      return 1;
    }
    /* randscale < x < randscale + RS_SCALE */
    x = (x - randscale) * (1.0 + PRNG_MAXIMUM);
  }
}

// Returns a random number between 0 and n-1 inclusive according to the
// distribution given in slots[]
size_t randutil::RandSlot(const double slots[/* n-1 */], size_t n) {
  long double xhi;

  /* Select a random range [x,x+RS_SCALE) */
  long double x = PRNG_FUNCTION() * RS_SCALE;

  /* Perform binary search to find the intersecting slot */
  size_t hi = n - 2, lo = 0, mi;
  while (hi > lo) {
    mi = AVG_2S_COMPLEMENT(lo, hi);
    if (x >= slots[mi])
      lo = mi + 1;
    else
      hi = mi;
  }

  /* Taking slots[-1]=0.0, this is now true: slots[lo-1] <= x < slots[lo] */

  /* If slots[lo-1] <= x < x+RS_SCALE <= slots[lo] then
     any point in [x,x+RS_SCALE) is in [slots[lo-1],slots[lo]) */

  if ((xhi = x + RS_SCALE) <= slots[lo])
    return lo;

  /* Otherwise x < slots[lo] < x+RS_SCALE */

  for (;;) {
    /* x < slots[lo] < xhi */
    if (RandBiased((slots[lo] - x) / (xhi - x)))
      return lo;
    x = slots[lo];
    lo++;
    if (lo >= n - 1)
      return n - 1;

    if (xhi <= slots[lo]) {
      /* slots[lo-1] = x <= xhi <= slots[lo] */
      return lo;
    }
  }
}

// Returns a (uniformly) randomly chosen number between 0 and n-1 inclusive.
size_t randutil::RandRange(size_t n) {
  long double range = static_cast<long double>(n);
  long double resolution = range * RS_SCALE;
  long double x = resolution * PRNG_FUNCTION(); /* x in [0,n) */
  long double xhi = x + resolution;
  size_t lo = (size_t)floor(x);

  for (;;) {
    lo++;
    if (lo >= xhi || RandBiased((lo - x) / (xhi - x))) {
      return lo - 1;
    }
    if (lo > static_cast<size_t>(std::numeric_limits<long double>::max())) {
      NOTREACHED();
    }
    x = static_cast<long double>(lo);
  }
}

float randutil::fRandRange(float in_range) {
  long double range = static_cast<long double>(in_range);
  long double resolution = range * RS_SCALE;
  long double x = resolution * PRNG_FUNCTION();
  long double xhi = x + resolution;
  long double lo = x;

  for (;;) {
    lo++;
    if (lo >= xhi || RandBiased((lo - x) / (xhi - x))) {
      return static_cast<float>(lo - 1);
    }
    if (lo > std::numeric_limits<float>::max()) {
      NOTREACHED();
    }
    x = lo;
  }
}

long double randutil::dRandRange(double in_range) {
  long double range = static_cast<long double>(in_range);
  long double resolution = range * RS_SCALE;
  long double x = resolution * PRNG_FUNCTION();
  long double xhi = x + resolution;
  long double lo = x;

  for (;;) {
    lo++;
    if (lo >= xhi || RandBiased((lo - x) / (xhi - x))) {
      return lo - 1;
    }
    if (lo > std::numeric_limits<long double>::max()) {
      NOTREACHED();
    }
    x = lo;
  }
}

// Returns random integer within in_range
int randutil::iRandRange(int in_range) {
  long long range = static_cast<long long>(in_range);
  long double resolution = range * RS_SCALE;
  long double x = resolution * PRNG_FUNCTION();
  long double xhi = x + resolution;
  long double lo = floor(x);

  for (;;) {
    lo++;
    if (lo >= xhi || RandBiased((lo - x) / (xhi - x))) {
      return static_cast<int>(lo - 1);
    }
    if (lo > std::numeric_limits<long long>::max()) {
      NOTREACHED();
    }
    x = lo;
  }
}

static unsigned char* seed = (unsigned char*)(&entropy + 1);

// Set the PRNG seed from some sources of entropy.
unsigned int randutil::RandReSeed(void) {
  static unsigned int accSeed = 0;
  if (seed == ((unsigned char*)(&entropy + 1))) {
    switch (entropy.which) {
      case 0:
        entropy.timet.ent += time(NULL);
        accSeed ^= entropy.timet.ent;
        break;
      case 1:
        entropy.clockt.ent += clock();
        break;
      case 2:
        entropy.counter.ent++;
        break;
    }
    entropy.which = (entropy.which + 1) % 3;
    seed = (unsigned char*)&entropy.timet;
  }

  accSeed = ((accSeed * (UCHAR_MAX + 2U)) | 1) + (int)*seed;
  seed++;
  PRNG_SEED(&accSeed, sizeof(accSeed));

  return accSeed;
}

int randutil::dRandIterationsRequired() {
  double m, n = 0;
  int c = -1;
  do {
    m = n;
    n = (n + PRNG_MAXIMUM) * (1.0 / (1.0 + PRNG_MAXIMUM));
    c++;
  } while (n > m); /* Accuracy limitations will eventually force n <= m */
  return c;
}

// Returns a random fraction between 0.0 inclusive and 1.0 exclusive.
// GCC & MinGW
long double randutil::dRandGCC() {
  std::wcout << __FUNCSIG__ << ENDL;
  long double d;
  do {
    d = ((PRNG_FUNCTION() * RS_SCALE) + PRNG_FUNCTION()) * RS_SCALE;
  } while (d >= 1.0); /* In case of round off error. */
  return d;
}

// MSVC, WATCOM, Turbo C, etc
long double randutil::dRandMSVC() {
  std::wcout << __FUNCSIG__ << ENDL;
  long double d;
  do {
    d = ((((PRNG_FUNCTION() * RS_SCALE) + PRNG_FUNCTION()) * RS_SCALE +
          PRNG_FUNCTION()) *
             RS_SCALE +
         PRNG_FUNCTION()) *
        RS_SCALE;
  } while (d >= 1.0); /* In case of round off error. */
  return d;
}

long double randutil::dRandManual() {
  std::wcout << __FUNCSIG__ << ENDL;
  long double d;

  if (dRandIterationsCount == 0) {
    dRandIterationsCount = dRandIterationsRequired();
  }

  do {
    switch (dRandIterationsCount) {
      case 1:
        d = PRNG_FUNCTION() * RS_SCALE;
        break;
      case 2:
        d = (PRNG_FUNCTION() * RS_SCALE + PRNG_FUNCTION()) * RS_SCALE;
        break;
      case 3:
        d = ((PRNG_FUNCTION() * RS_SCALE + PRNG_FUNCTION()) * RS_SCALE +
             PRNG_FUNCTION()) *
            RS_SCALE;
        break;
      case 4:
        d = (((PRNG_FUNCTION() * RS_SCALE + PRNG_FUNCTION()) * RS_SCALE +
              PRNG_FUNCTION()) *
                 RS_SCALE +
             PRNG_FUNCTION()) *
            RS_SCALE;
        break;
      case 5:
        d = ((((PRNG_FUNCTION() * RS_SCALE + PRNG_FUNCTION()) * RS_SCALE +
               PRNG_FUNCTION()) *
                  RS_SCALE +
              PRNG_FUNCTION()) *
                 RS_SCALE +
             PRNG_FUNCTION()) *
            RS_SCALE;
        break;
      default: {
        int i;
        for (d = 0.0, i = 0; i < dRandIterationsCount; i++)
          d = (d + PRNG_FUNCTION()) * RS_SCALE;
        break;
      }
    }
  } while (d >= 1.0); /* In case of round off error. */

  return d;
}

long double randutil::dRand() {
  RandReSeed(); // Needed to avoid repetition

  long double specific_drand;

  if (!force_regular_drand) {
#if (PRNG_MAXIMUM >= 0x7FFFFFFF && DBL_MANT_DIG <= 62) || \
    (PRNG_MAXIMUM >= 0x7FFF && DBL_MANT_DIG <= 30)
  specific_drand = dRandGCC();
#elif PRNG_MAXIMUM < 208064 && DBL_MANT_DIG <= 53
  specific_drand = dRandMSVC();
#else
  specific_drand = dRandManual();
#endif
  } else {
    specific_drand = dRandManual();
  }

  return specific_drand;
}

void randutil::LogRand() {
  std::wcout << L"\n --- rand_util.cc randum number functions test --- \n\n";
  std::wcout << "dispatched dRand function: ";
  long double randnum = dRand();
  const int BiasedRand = RandBiased(0.500);
  const double dRangeRand = dRandRange(100.00);
  const float fRangeRand = fRandRange(32.0f);
  const int iRangeRand = iRandRange(256);
  std::wostringstream logbuff;
  logbuff << std::setprecision(MAX_LOADSTRING)
          << L"dRand() result: " << randnum << WNL
          << L"RandBiased(0.500) result: " << BiasedRand << WNL
          << L"dRandRange(100.00) result: " << dRangeRand << WNL
          << L"fRandRange(32.0f) result: " << fRangeRand << WNL
          << L"iRandRange(256) result: " << iRangeRand << ENDL;
  std::wcout << logbuff.str();
  logbuff.clear();
}

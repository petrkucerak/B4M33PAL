#include <iostream>
#include <vector>

using namespace std;

const int prime_numbers[] = {
    5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,  53,
    59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193,
    197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271,
    277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
    367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443,
    449, 457, 461, 463, 467, 479, 487, 491, 499, 503};
const int prime_numbers_count = 94;

long multiplicative_inverse(long a, long modulus)
{
   long s = 0, r = modulus, old_s = 1, old_r = a;
   while (r != 0) {
      long quotient = old_r / r;
      long temp = r;
      r = old_r - quotient * r;
      old_r = temp;
      temp = s;
      s = old_s - quotient * s;
      old_s = temp;
   }
   return old_s < 0 ? old_s + modulus : old_s;
}

void is_valid(vector<long> &sequnece, long A, long C, long M)
{
   for (int i = 0; i < sequnece.size() - 1; ++i) {
      if ((A * sequnece[i] + C) % M != sequnece[i + 1])
         return;
   }
   printf("%ld %ld %ld\n", A, C, M);
   exit(EXIT_SUCCESS);
}

void a_combination(int &m_max, long p, vector<long> &sequnece)
{
   long M = p * p;
   long A = p + 1;
   for (; A < M; ++A) {
      for (long c = 1; c < M; ++c) {
         is_valid(sequnece, A, c, M);
      }
   }
}

void m_combination(int start, long p, int &m_max, vector<long> &sequnece)
{
   // call next combination
   a_combination(m_max, p, sequnece);
   start += 1;
   for (; start < prime_numbers_count; ++start) {
      p *= prime_numbers[start];
      if (p * p > m_max)
         break;
      m_combination(start, p, m_max, sequnece);
   }
}

int main(int argc, char const *argv[])
{

   // LOAD THE DATA
   int p_max, m_max, n;
   if (scanf("%d %d %d\n", &p_max, &m_max, &n) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }

   vector<long> sequence(n);
   long m_min = 25;
   for (auto &x : sequence) {
      if (scanf("%ld", &x) != 1) {
         fprintf(stderr, "ERROR - can't load the data!\n");
         exit(EXIT_FAILURE);
      }
      // compare M min size
      if (x > m_min)
         m_min = x;
   }

   // cout << multiplicative_inverse(114, 25) << endl;

   /**
    * @brief Generate potencial M
    * M > m_min
    * M < m_max
    * M = (prime_numbers[0] * prime_numbers[1] * ... * prime_numbers[F])^2,
    * where prime_numbers[F]^2 < m_max
    */
   for (int start = 0; start < prime_numbers_count; ++start) {
      if (prime_numbers[start] * prime_numbers[start] > m_max)
         break;
      m_combination(start, prime_numbers[start], m_max, sequence);
   }

   /**
    * @brief Generate potencial A
    * A < M
    * A > min_p
    * A = (prime_number[0] * primer_number[1] * ... * prime_number[F])
    */

   return 0;
}

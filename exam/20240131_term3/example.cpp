long multiplicativeInverse(long a, long modulus)
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
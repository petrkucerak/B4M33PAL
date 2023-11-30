#include <iostream>

int main(int argc, char const *argv[])
{
   int s_length, t_length, length_min, length_max;
   if (scanf("%d %d %d %d\n", &s_length, &t_length, &length_min, &length_max) !=
       4) {
      fprintf(stderr, "Can't load metadata!\n");
      exit(EXIT_FAILURE);
   }
   int *S = new int[s_length];
   int *T = new int[t_length];

   for (int i = 0; i < s_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load S string!\n");
         exit(EXIT_FAILURE);
      }
      S[i] = int(n) - 48;
   }
   scanf("\n");
   for (int i = 0; i < t_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load T string!\n");
         exit(EXIT_FAILURE);
      }
      T[i] = int(n) - 48;
   }

   // Print S array
   for (int i = 0; i < s_length; ++i)
      printf("%d", S[i]);
   printf("\n");
   // Print T array
   for (int i = 0; i < t_length; ++i)
      printf("%d", T[i]);
   printf("\n");

   delete[] S;
   delete[] T;

   return 0;
}

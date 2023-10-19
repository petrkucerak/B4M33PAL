#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
   uint64_t cont = 0;
   printf("Hello world\n");
   for (uint64_t i = 0; i < 100000; ++i) {
      cont += i;
   }
   printf("%ld\n", cont);
   return 0;
}

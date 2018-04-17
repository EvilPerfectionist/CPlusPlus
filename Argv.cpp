#include <stdio.h>

//argv and argc are how command line arguments are passed to main() in C and C++.

//argc will be the number of strings pointed to by argv. This will (in practice)
//be 1 plus the number of arguments, as virtually all implementations will prepend
//the name of the program to the array.

//The variables are named argc (argument count) and argv (argument vector) by convention,
//but they can be given any valid identifier: int main(int num_args, char** arg_strings)
//is equally valid.

int main (int argc, char *argv[])
{
  int count;

  printf ("This program was called with \"%s\".\n",argv[0]);

  if (argc > 1)
    {
      for (count = 1; count < argc; count++)
	{
	  printf("argv[%d] = %s\n", count, argv[count]);
	}
    }
  else
    {
      printf("The command had no other arguments.\n");
    }

  return 0;
}
/*
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
}
*/
// input: ./test a1 b2 c3
// Result:
// Have 4 arguments:
// ./test
// a1
// b2
// c3
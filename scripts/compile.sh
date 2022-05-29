cd .. &&
  rm -r build
mkdir build &&
  cd build &&
  clang++ main.cpp -o test -Xpreprocessor -fopenmp -lomp

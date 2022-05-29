cd .. &&
  rm -r build
mkdir build &&
  cd build &&
  clang++ ../main.cpp -o main -Xpreprocessor -fopenmp -lomp
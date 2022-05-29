cd ..
count=10
for i in $(seq $count); do
  mpirun -np 4 build/main >>results.txt
done

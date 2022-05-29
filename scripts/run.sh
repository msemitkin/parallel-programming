cd ..
count=10
for i in $(seq $count); do
  ./main >>results.txt
done

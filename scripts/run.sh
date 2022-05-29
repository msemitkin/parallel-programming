cd ..
count=10
for i in $(seq $count); do
       build/main >> results.txt
done

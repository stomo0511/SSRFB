#!/bin/bash
echo "b,s,time"
for ((i=96; i<=1152; i=i+16));
do
 ./SS $i
done
# echo "b,s,time"
# for ((i=128; i<=1024; i=i+16));
# do
#  ./LA $i
# done
# echo "b,s,time"
# for ((i=128; i<=1024; i=i+16));
# do
#  ./TS $i
# done
# echo "b,s,time"
# for ((i=128; i<=1024; i=i+16));
# do
#  ./GE $i
#done

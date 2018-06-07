#!/bin/bash
echo "b,s,time"
# for ((i=128; i<=1024; i=i+16));
#for ((i=96; i<=1152; i=i+16));
#for ((i=16; i<=80; i=i+4));
#for ((i=1152; i<=1538; i=i+16));
for ((i=1552; i<=2000; i=i+16));
do
 ./SS $i
done
# echo "b,s,time"
# # for ((i=128; i<=1024; i=i+16));
# for ((i=96; i<=1152; i=i+16));
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

#!/bin/bash
#By Lucas Antonsen, V00923982

seeds=(0022 0023 0028 0029 0032 0038 0039 0082 0083 0089 0092 0093 0098 0099 0223 0228 0229 0232 0238 0239)
quantums=(50 100 250 500)
dispatch=(0 5 10 15 20 25)
declare -a turnarounds
declare -a waiting
declare -a avg_ta
declare -a avg_wait

#TURNAROUND BEGIN

for q in "${quantums[@]}"
do
    for d in "${dispatch[@]}"
    do
        echo "quantum: $q dispatch: $d"
        for s in "${seeds[@]}"
        do
            turnarounds+=($(./simgen 1000 $s | ./rrsim --quantum $q --dispatch $d | grep 'ta=' | awk -F'ta=' '{sum+=$2; num+=1;}END{print sum/num;}'))
        done
        
        totsum=0
        for i in "${turnarounds[@]}"
        do
            totsum=$((totsum + $i))
        done
        avg=$((totsum/20))
        echo "average: $avg"
        avg_ta+=($avg)
        
        turnarounds=()
        echo "${avg_ta[@]}"
    done
done
python graph.py "${avg_ta[@]}" --flag ta

#TURNAROUND END

#WAITING BEGIN

for q in "${quantums[@]}"
do
    for d in "${dispatch[@]}"
    do
        echo "quantum: $q dispatch: $d"
        for s in "${seeds[@]}"
        do
            waiting+=($(./simgen 1000 $s | ./rrsim --quantum $q --dispatch $d | grep 'w=' | awk -F'w=' '{print $2}' | cut -d " " -f1 | awk '{wsum+=$1; wnum+=1;}END{print wsum/wnum;}'))
        done
        
        wtotsum=0
        for y in "${waiting[@]}"
        do
            wtotsum=$((wtotsum + $y))
        done
        wavg=$((wtotsum/20))
        echo "average: $wavg"
        avg_wait+=($wavg)
        
        waiting=()
        echo "${avg_wait[@]}"
    done
done
python graph.py "${avg_wait[@]}" --flag w

#WAITING END


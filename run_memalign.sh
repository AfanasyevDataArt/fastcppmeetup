#!/bin/bash

NAME="memalign"

for i in 0 1; \
  do \
    for j in {1..100}; \
    do
      cmake-build-debug/fastcpp -n $NAME -c $i; \
    done ; \
done \
| grep time > $NAME.json

./graph.py --infile "$NAME.json" --outfile "$NAME"_seq.png

for i in 2 3; \
  do \
    for j in {1..100}; \
    do
      cmake-build-debug/fastcpp -n $NAME -c $i; \
    done ; \
done \
| grep time > $NAME.json

./graph.py --infile "$NAME.json" --outfile "$NAME"_rand.png

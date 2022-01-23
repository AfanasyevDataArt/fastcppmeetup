#!/bin/bash

NAME="prefetcher"

for i in 0 1; \
  do \
    for j in {1..100}; \
    do
      cmake-build-debug/fastcpp -n $NAME -c $i; \
    done ; \
done \
| grep time > $NAME.json

./graph.py --infile "$NAME".json --outfile "$NAME".png

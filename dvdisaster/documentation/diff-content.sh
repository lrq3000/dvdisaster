#!/bin/bash

prefix=/var/tmp/reference

for i in $(find . -name "*.html"); do
  if ! cmp $i ${prefix}/$i; then
     diff $i $prefix/$i
  fi
done

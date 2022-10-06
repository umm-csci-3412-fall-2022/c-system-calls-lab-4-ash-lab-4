#!/bin/bash

find $1 -type d | wc -l | xargs printf "There were %i directories.\n"
find $1 -type f | wc -l | xargs printf "There were %i regular files.\n"
exit 0
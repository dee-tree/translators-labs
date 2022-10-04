#bash

prg="./words"     # program which returns words longer than 3 chars

$prg </dev/stdin | sort -u

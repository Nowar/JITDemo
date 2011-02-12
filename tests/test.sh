#!/bin/bash
function showUsage() {
  echo "Usage: $0 dirname"
}


if [ $# -ne 1 ]; then
  showUsage
  exit 1
fi

if [ -d $1 ]; then
  src_file=$(basename $(find $1 | egrep '\.bc'))
  src_dir=$(dirname $1)

  cd $1 && \
       ../../out/demo $src_file > stdout 2> stderr
  $(diff $src_dir/stdout $src_dir/stdout.expect > /dev/null 2> /dev/null)
  out_diff=$(echo $?)
  $(diff $src_dir/stderr $src_dir/stderr.expect > /dev/null 2> /dev/null)
  err_diff=$(echo $?)
  if [ $out_diff -ne 0 ]; then
    echo "Error: stdout is not correct."
  fi
  if [ $err_diff -ne 0 ]; then
    echo "Error: stderr is not correct."
  fi
else
  showUsage
  exit 1
fi

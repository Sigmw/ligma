#!/bin/bash

set -eu

MYDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
binary=${1:-}
if [ "x$binary" == "x" ] ; then
    echo "Usage: $0 path/to/lisp-interpreter"
    exit 1
fi

find "${MYDIR}/cases/"*.out | while read f; do
    b=$(basename "$f" .out)
    if ! diff -u "$f" <("${binary}" < "${MYDIR}/cases/$b"); then
        echo "^^ ERROR test case caused faulty output: $b"
    fi
done

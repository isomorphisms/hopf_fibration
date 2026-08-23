#!/usr/bin/env sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
if [ -z "${IDRIC:-}" ]; then
    IDRIC="$ROOT/../Idric/build/exec/idris2"
fi
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

cd "$ROOT/src"

"$IDRIC" GenerateHeader.idric -o hopf-header-generator
./build/exec/hopf-header-generator > "$TMP/hopf_math.h"

"$IDRIC" GenerateSource.idric -o hopf-source-generator
./build/exec/hopf-source-generator > "$TMP/hopf_math.c"

cd "$ROOT"

if [ "${1:-}" = "--check" ]; then
    cmp "$TMP/hopf_math.h" app/src/main/cpp/hopf_math.h
    cmp "$TMP/hopf_math.c" app/src/main/cpp/hopf_math.c
else
    cp "$TMP/hopf_math.h" app/src/main/cpp/hopf_math.h
    cp "$TMP/hopf_math.c" app/src/main/cpp/hopf_math.c
fi

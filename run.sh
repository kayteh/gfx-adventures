#!/bin/bash

set -e
premake5 gmake2
make -j
test -z "$SKIP_RUN" && .bin/main "$@"

exit 0
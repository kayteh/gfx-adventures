#!/bin/bash

premake5 gmake2 && make -j && (test $SKIP_RUN -z && .bin/main "$@")
#!/bin/bash

(cd .. && premake5 gmake)
make -j && .bin/main "$@"
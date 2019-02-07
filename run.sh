#!/bin/bash

premake5 gmake2 && make -j && .bin/main "$@"
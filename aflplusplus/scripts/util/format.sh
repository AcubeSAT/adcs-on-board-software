#!/bin/bash
# Use shfmt to find all paths to shell scripts recursively,
# then format each file in-place.

# The formatting flags selected closely resemble
# the Google codestyle guidelines for shell:
# https://google.github.io/styleguide/shellguide.html

~/go/bin/shfmt -f . | xargs ~/go/bin/shfmt -l -w -i 2 -ci -bn

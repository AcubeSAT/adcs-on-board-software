#!/bin/bash
# Use cwtriage to use gdb and gdb's "exploitable" plugin, to
# triage afl-found crashes and probe which
# lead to exploitable conditions and which don't.

# cwdump is used to summarize the crashes in the crashwalk database
# by bucketing by major/minor stack hash.
# Although AFL already de-dupes crashes,
# bucketing summarizes these by an order of magnitude or more.
# Crashes that bucket the same have exactly the same stack contents,
# so they're likely (but NOT guaranteed) to be the same bug.

screen -r "crashwalk" -X stuff $'~/go/bin/cwtriage -root findings/ -afl && ~/go/bin/cwdump ./crashwalk.db > triage\n'

#!/bin/sh
rm moreonigiri_flow.pot
touch moreonigiri_flow.pot
find ../src/logic -type f \( -name '*.hpp' -or -name '*.cpp' \)  -print > list
xgettext --files-from=list --c++ -j -o moreonigiri_flow.pot --keyword=_ -v
rm list
echo finished

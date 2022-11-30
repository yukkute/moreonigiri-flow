#!/bin/sh
rm onigiri_logic.pot
touch onigiri_logic.pot
find ../src/logic -type f \( -name '*.hpp' -or -name '*.cpp' \)  -print > list
xgettext --files-from=list --c++ -j -o onigiri_logic.pot --keyword=_ -v
rm list
echo finished

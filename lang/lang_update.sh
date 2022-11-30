#!/bin/sh
for file in ./*.po;
do
echo `basename ${file}`
msgmerge --update ${file} moreonigiri_flow.pot -v;
echo
done
rm onigiri_logic.pot
echo finished

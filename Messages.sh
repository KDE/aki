#! /bin/sh
$EXTRACTRC `find . -name \*.ui -o -name \*.rc -o -name \*.kcfg` >> rc.cpp
$XGETTEXT `find . -name \*.cpp -name \*.cxx -o -name \*.h -o -name \*.hpp -o $podir/aki.pot
rm -f rc.cpp

#!/bin/bash

echo
echo "Game Rom Mutator Setup"


compile() {
 f=$1
 shift
 gcc "$f.c" -o "$f" "$@" -lm -s -O2 -Wall \
	-fweb -fomit-frame-pointer -fwhole-program \
	-fmerge-all-constants
 strip -s -R .comment "$f"
 ls -l "$f"
}


compileAll() {
 compile mutate
 compile romdiff
 compile romedit
}


update () {
 echo "Installing or updating compiler..."
 echo "You might need to enter your password for apt"
 apt update
 apt install gcc build-essential
}


run () {
 echo "Running..."
 python3 mutate-gui.py &
}


while true
do
 echo
 echo " Menu"
 echo "i Install or update GCC compiler via apt"
 echo "c Compile via GCC"
 echo "r Run the GUI"
 echo "h Show help file"
 echo "w Visit website"
 echo "q Quit"

 read -n 1 -p "?" i
 echo

 case $i in
  i) update ;;
  c) compileAll ;;
  r) run ;;
  h) xdg-open doc.htm ;;
  w) xdg-open "https://github.com/RonAmerica/GameRomMutator" ;;
  q) break ;;
  *) echo "Bad user!" ;;
 esac

done

echo
echo "Done."
exit


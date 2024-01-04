#!/bin/bash

echo
echo "Game Rom Mutator Setup"


compile () {
 echo "Compiling..."
 gcc mutate.c -o mutate -Os -Wall -Wextra -fomit-frame-pointer
 ls -l mutate
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
<<<<<<< HEAD
 echo "w Visit website"
=======
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
 echo "q Quit"

 read -n 1 -p "?" i
 echo

 case $i in
  i) update ;;
  c) compile ;;
  r) run ;;
  h) xdg-open doc.htm ;;
<<<<<<< HEAD
  w) xdg-open "https://github.com/RonAmerica/GameRomMutator" ;;
=======
>>>>>>> ef57c379e499d352f654e9253b5df619f3f10a9e
  q) break ;;
  *) echo "Bad user!" ;;
 esac

done

echo
echo "Done."
exit


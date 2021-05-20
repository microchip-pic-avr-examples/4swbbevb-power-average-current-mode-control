#!/bin/sh
echo $SSH_AUTH_SOCK
cd ../../
echo Pulling changes from subtree 'subrepo-pral' branch 'main' into recently checked out working branch...
git subtree pull --prefix="dspic33c-4swbb-vmc.X/sources/common/p33c_pral" subrepo-pral main --squash 
echo Pull complete
echo
echo Press Enter to exit
read

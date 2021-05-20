#!/bin/sh
echo $SSH_AUTH_SOCK
cd ../../
echo Pulling changes from subtree 'subrepo-4swbb' branch 'main' into recently checked out working branch...
git subtree pull --prefix="dspic33c-4swbb-vmc.X/sources/power_control/devices" subrepo-4swbb main --squash 
echo Pull complete
echo
echo Press Enter to exit
read

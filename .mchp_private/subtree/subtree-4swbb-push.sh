#!/bin/sh
echo $SSH_AUTH_SOCK
cd ../../
echo Pushing changes to subtree 'subrepo-4swbb' branch 'version-update' from recently checked out working branch...
git subtree push --prefix="dspic33c-4swbb-vmc.X/sources/power_control/devices" subrepo-4swbb feature/version-update --squash
echo Pushing changes to subtree directory complete
echo
echo Press Enter to exit
read


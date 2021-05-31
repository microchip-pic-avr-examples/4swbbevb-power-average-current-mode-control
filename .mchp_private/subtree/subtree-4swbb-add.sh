#!/bin/sh
echo $SSH_AUTH_SOCK
cd ../../
echo Adding subtree repository 'subrepo-4swbb' ro project repository...
git remote add subrepo-4swbb https://bitbucket.microchip.com/scm/mcu16asmpsl/subrepo-4swbb-converter.git
echo Adding subtree repository complete
echo 
echo Downloading subtree contents into project repository...
git subtree add --prefix="dspic33c-4swbb-vmc.X/sources/power_control/devices" subrepo-4swbb main --squash
echo Download source files complete
echo
echo Press Enter to exit
read
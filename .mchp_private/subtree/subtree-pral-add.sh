#!/bin/sh
echo $SSH_AUTH_SOCK
cd ../../
echo Adding subtree repository 'subrepo-pral' ro project repository...
git remote add subrepo-pral https://bitbucket.microchip.com/scm/mcu16asmpsl/p33c-peripheral-sfr-abstraction-drivers.git
echo Adding subtree repository complete
echo 
echo Downloading subtree contents into project repository...
git subtree add --prefix="dspic33c-4swbb-vmc.X/sources/common/p33c_pral" subrepo-pral main --squash 
echo Download source files complete
echo
echo Press Enter to exit
read

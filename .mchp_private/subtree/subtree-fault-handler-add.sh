#!/bin/sh
echo $SSH_AUTH_SOCK
cd ../../
echo Adding subtree repository 'subrepo-fault' ro project repository...
git remote add subrepo-fault https://bitbucket.microchip.com/scm/mcu16asmpsl/subrepo-fault-handler.git
echo Adding subtree repository complete
echo 
echo Downloading subtree contents into project repository...
git subtree add --prefix="dspic33c-4swbb-vmc.X/sources/fault_handler/drivers" subrepo-fault main --squash
echo Download source files complete
echo
echo Press Enter to exit
read


#!/bin/bash

cd $HOME
echo 'Installing CAPD...';
if [ ! -d "capd" ]; then
  svn co https://svn.capdnet.ii.uj.edu.pl/capd/ --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other ;
  cd capd ;
  svn co https://svn.capdnet.ii.uj.edu.pl/capdDynSys4 --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other ;
  autoreconf --install ;
  ./configure --with-filib=check --with-mpfr=check ;
  make ;
else
  echo 'Using cached directory.' ;
fi
cd $HOME/capd
sudo make install
cd $HOME
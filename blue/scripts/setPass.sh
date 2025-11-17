#!/bin/bash

users=("falken" "joshua" "lightman" "wopr" "norad" "david")
echo -n "Enter a password: "
read password

for i in "${users[@]}"; do
        echo "$i:$password" | chpasswd
        echo "" > /home/$i/.ssh/known_hosts
done

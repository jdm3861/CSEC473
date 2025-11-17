#!/bin/bash

# install packages 
echo "installing packages"
apt install -y auditd tmux inotify-tools strace


#aduit rules 
echo "setting audit rules"

cp ../audit.rules /etc/audit/rules.d/audit.rules
systemctl enable auditd && systemctl start auditd
augenrules --load

# ssh hardening 
echo "Hardening ssh"
# blue team hosts
hosts=("10.0.2.2" "10.0.2.3" "10.0.2.4" "10.0.2.5" "10.100.2.2")

for host in "${hosts[@]}"; do
        key=$(ssh-keyscan "$host")
        echo "$key" >> /home/norad/.ssh/known_hosts
        echo "$key" >> /home/falken/.ssh/known_hosts
done

#specify ssh users
#echo "AllowUsers falken norad" >> /etc/ssh/sshd_config

# backups 
echo "Backing things up"
BACKUP_DIR="/root/backup"
mkdir "$BACKUP_DIR"
mkdir "$BACKUP_DIR/systemctl"

services=("dovecot" "vsftp" "nginx" "mysql" "audit" "ssh")

for service in "${services[@]}"; do
        cp -r "/etc/$service/" "$BACKUP_DIR"
        systemctl cat "$service" > "$BACKUP_DIR/systemctl/$service.service"
done

find "$BACKUP_DIR" -type f -exec sha256sum {} \; > "$BACKUP_DIR"/hashes.txt

echo "Setting permissions..."
chmod -R go-rwx "$BACKUP_DIR"
chown -R root:root "$BACKUP_DIR"

echo "Making backup immutable..."
chattr -R +i "$BACKUP_DIR"

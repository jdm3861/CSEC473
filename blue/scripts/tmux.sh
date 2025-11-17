#!/bin/bash 

name=$1
tmux new-session -d -s name
tmux split-window -h -t name
tmux split-window -v -t name:0.1

#sending keys 
tmux send-keys -t name:0.1 "sudo tail -f /var/log/audit/audit.log" C-m
tmux send-keys -t name:0.2 "watch -n 1 'systemctl --type=service --state=running'" C-m

# attach to session 
tmux attach-session -t space

## Prereqs 
- ansible controller with ansible installed 
- debian host(s)

## Configuration 
- specify host(s) in inventory.ini
- modify vars in playbook 
    - nginx_user
    - nginx_group 
    - nginx_root
    - index.html (create or modify)
## Commands 
- ansible-playbook -i inventory.ini deploy-web.yml
- or ansible-playbook deploy-web.yml 

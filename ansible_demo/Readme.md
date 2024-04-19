ansible-inventory -i inventory.ini --list
ansible myhosts -u root -m ping -i inventory.ini

192.168.3.7 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
192.168.3.5 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
192.168.3.6 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}

ansible myhosts -u root -m ping -i inventory.yaml
ansible my_host_01 -u root -m ping -i inventory.yaml
ansible-playbook -u root -i inventory.yaml playbook.yaml

PLAY [My first play] **************************************************************************************************************************************************

TASK [Gathering Facts] ************************************************************************************************************************************************
ok: [my_host_01]
ok: [my_host_03]
ok: [my_host_02]

TASK [Ping my hosts] **************************************************************************************************************************************************
ok: [my_host_03]
ok: [my_host_01]
ok: [my_host_02]

TASK [Print message] **************************************************************************************************************************************************
ok: [my_host_01] => {
    "changed": false,
    "msg": "Hello world"
}
ok: [my_host_02] => {
    "changed": false,
    "msg": "Hello world"
}
ok: [my_host_03] => {
    "changed": false,
    "msg": "Hello world"
}

PLAY RECAP ************************************************************************************************************************************************************
my_host_01                 : ok=3    changed=0    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0
my_host_02                 : ok=3    changed=0    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0
my_host_03                 : ok=3    changed=0    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0


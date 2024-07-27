#!/usr/bin/expect
expect
set timeout -1
set ip [lindex $argv 0]
spawn sshpass -p xxxxx ssh -o StrictHostKeyChecking=no admin123@10.127.22.$ip system-view
expect "]" {send "scp server enable\r"}
expect "*" {send "commit\r"}
expect "]" {send "commit\r"}
expect eof
#system-view; scp server enable; commit

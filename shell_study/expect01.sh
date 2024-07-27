#!/usr/bin/expect
expect
set timeout 6
spawn scp lance@127.0.0.1:/tmp/nvi_switch.md .
expect password {send xxxxx\n}
expect eof

#! /bin/bash
echo '\n'
echo '==================== iptables -L ====================\n'
iptables -L
echo '\n'
echo '==================== iptables -L -t nat ====================\n'
iptables -L -t nat

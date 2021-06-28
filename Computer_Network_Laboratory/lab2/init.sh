#! /bin/bash

iptables -A FORWARD -i wlx74da38e6c42b -p tcp --dport 53 -j ACCEPT
iptables -A FORWARD -i wlx74da38e6c42b -p udp --dport 53 -j ACCEPT
iptables -A FORWARD -i wlx74da38e6c42b -j DROP
iptables -t nat -A PREROUTING -i wlx74da38e6c42b -p tcp --dport 80 -j DNAT --to-destination 10.0.2.15:9090
iptables -t nat -A PREROUTING -i wlx74da38e6c42b -p tcp --dport 443 -j DNAT --to-destination 10.0.2.15:9090

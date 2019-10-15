1.指定端口
iptables -I FORWARD -p tcp -m multiport --port 6688  -j DROP

2.指定目的ip
iptables -I FORWARD -p tcp --dst 120.55.19.15 -j DROP
iptables -I FORWARD -s 1.1.1.1 -j DROP

3.结合ipset
ipset create drop_source hash:net
ipset add drop_source 1.1.1.1/32
ipset add drop_source 10.10.10.10/32
ipset add drop_source 100.100.100.100/32
iptables -I FORWARD -m set --match-set drop_source src  -j DROP

4.查看端口号文件：
/etc/services

5.阻断一个范围的源ip
iptables -I FORWARD -m iprange --src-range 10.10.10.1-10.10.10.101 -j DROP

6.匹配一定范围的报文包，并阻断
iptables -A FORWARD -p icmp -m length --length 1000:1500 -j DROP

7.前往转发enp9s0 的包，接受数据包
iptables -A FORWARD -i enp9s0 -j ACCEPT

8.snat转换规则
iptables -t nat -A POSTROUTING -s 172.16.13.0/24 -o enp8s0 -j SNAT --to 172.16.105.16
所有网段
iptables -t nat -A POSTROUTING -s 0.0.0.0/0 -o enp8s0 -j SNAT --to 172.16.105.16
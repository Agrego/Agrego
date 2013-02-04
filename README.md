# The AgreGo project
==================

AgreGo is a network link aggregator. It allows to use several network links as one and sum all characteristics of every links.

It is inspired by the bonding module of the kernel linux, but it works at the IP level. So contrary to the bonding module, it doesn't require the 2 endpoints to be in the same network.

The first goal of the agrego project, is to aggregate xDSL links, to allow to increase the bandwidth of your internet connection by adding several xDSL connections.

## Main features

  * aggregation: group several links into one
  * fail-over: failure detection on a link, and rehabilitation of this one
  * link weighting: if you want to send more data on a link than on others. This allows to prevent to limit all links bandwidth to the lower one.

As aggregation is apparently a part of the IPV6 protocol, AgreGo only manage IPV4.

It is developped in C, and works well on linux (Debian like distribs only for now).

## How it works

AgreGo works in the same manner the bonding module of linux does.
In order to aggregate several network links, the system has to run at both ends of every link. Thus 2 servers (endpoints) are needed to run the AgreGo system (see the link below).
Both endpoints connect to each other by using the UDP protocol, and create a virtual interface. These virtual interfaces will be the both ends of the AgreGo system. 

![image](http://daligro.net/agrego/simple_agrego.png)

AgreGo has been designed to work on xDSL links. In this case, one of the 2 endpoints is in the Local Area Network (LAN), it's called the local endpoint. The other is on Internet, it's called the distant endpoint.

![image](http://daligro.net/agrego/xDSL_agrego.png)

## Links

 * mailing list: agrego@googlegroups.com

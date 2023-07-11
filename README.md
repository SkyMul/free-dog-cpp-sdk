# Unitree Go1 Free-Dog SDK C++

This repository contains an adapted version of [free-dog-sdk](https://github.com/Bin4ry/free-dog-sdk) in CPP.

The code has a number of improvements, but it only has low level control.

## Installation Guide

Use the standard process of cloning this repository, making the build folder, running cmake, running make, and installing.

## Configuration of the dog and the SDK

Execute the following code on the robot's Raspberry Pi：

```
sudo vi /etc/sysctl.conf
```

Remove the comment in front of net.ipv4.ip_forward=1

```
sudo sysctl -p
sudo iptables -F
sudo iptables -t nat -F
sudo iptables -t nat -A POSTROUTING -o wlan1 -j MASQUERADE
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i wlan1 -o eth0 -j ACCEPT
sudo iptables -A FORWARD -i eth0 -o wlan1 -j ACCEPT
```

Do the following on the PC running this code：

```
sudo route add default gw 192.168.12.1
```

#### Configure the connection

You must establish a unitreeConnection object to get data. This takes in a tuple of the following:

- listenPort -- What port you should reserve for listening to connections. Arbitrary.
- addr -- The address of the robot. This should be 192.168.123.10 if you are connected via Ethernet.
- sendPort -- The port to send to. This should be 8007.
- localIP -- The IP of the computer running the code. It is recommended that this is set as 192.168.123.14.

### Lowlevel Example

See example/example_position_lowlevel.cpp for an example.

#### Run the Lowlevel Position Example

Elevate the dog using a box so its feet are not touching the ground and the front right leg is free to move.

Then run

```
./free_dog_sdk_cpp_example_position_lowlevel
```

The dog's front right leg should do a sin motion.

## Warnings

Absolutely NO warranty. This is not intentionally designed to but may inadvertently destroy your robot.

## LICENSE

MIT

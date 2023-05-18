import serial
from scapy.all import *

# Replace the example MAC address with the MAC address of the device you want to monitor
# DEVICE_MAC_ADDRESS = "B6:AE:67:0C:45:1F"
DEVICE_MAC_ADDRESS = "D8:9E:F3:7D:F7:9F"
TIKTOK_DOMAIN_NAME = "tiktok.com"
ARDUINO_SERIAL_PORT = "/dev/ttyACM0"

EXCLUDED_MAC_ADDRESSES = ["5c:ea:1d:0e:4d:6b", "34:5d:9e:f7:85:ed"]

# Create a serial connection to the Arduino
ser = serial.Serial(ARDUINO_SERIAL_PORT, 9600)

# Initialize the device state to False (not networking with TikTok)
device_networking = False


def packet_handler(packet):
    # global device_networking
    # Check if the packet contains both IP and TCP layers
    print("packet")
    if packet.haslayer(IP) and packet.haslayer(TCP):
        # # Check if the packet's source MAC address matches the device's MAC address
        # print(packet.summary())
        if packet[Ether].src.lower() in EXCLUDED_MAC_ADDRESSES:
            return

        print(packet[Ether].src.lower())
        if packet[Ether].src.lower() == DEVICE_MAC_ADDRESS.lower():
            print(packet[IP].src)
            print(packet[IP].dst)
        #     # Check if the packet's destination domain name matches the TikTok domain
        #     if TIKTOK_DOMAIN_NAME in packet[IP].dst:
        #         # Update the device state and send data to the Arduino if it's not already networking with TikTok
        #         if not device_networking:
        #             device_networking = True
        #             ser.write(b'1')
        #     else:
        #         # Update the device state if it's no longer networking with TikTok and send data to the Arduino
        #         if device_networking:
        #             device_networking = False
        #             ser.write(b'0')


# Sniff for all traffic on the network with a timeout of 10 minutes (600 seconds)
sniff(prn=packet_handler, timeout=600)

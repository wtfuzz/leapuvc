# Leap motion UVC

This provides basic support of accessing the raw video data from Leap Motion (http://leapmotion.com) hardware, without using the leap SDK.

The device works like a normal UVC camera, however there is some kind of undocumented initialization that occurs using UVC class USB commands before the video stream will start. The sequence was simply copied from wireshark sniffing of the USB traffic to leapd.

## Install deps
```
sudo apt-get install libusb-1.0-0-dev cmake
```

```
git clone https://github.com/ktossell/libuvc.git
cd libuvc
mkdir build
cd build
cmake ..
make
sudo make install
```

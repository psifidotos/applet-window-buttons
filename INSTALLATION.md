Installation
============

## Building from Source
The provided `install.sh` script will build everything and install it for you. Before running the installation script you have to install the dependencies needed for compiling.

### Build Dependencies

- Ubuntu:
```
sudo apt install g++ extra-cmake-modules qtbase5-dev qtdeclarative5-dev libkf5declarative-dev libkf5plasma-dev libkdecorations2-dev gettext
```

### Building and Installing
Once you have installed the dependencies listed above you can execute the build and install script:

```
sh install.sh
```

## Prebuilt Binaries

- Ubuntu: You can install via a PPA on Ubuntu 18.04 (Bionic) or later including KDE Neon.
```
sudo add-apt-repository ppa:krisives/applet-window-buttons
sudo apt install applet-window-buttons
```

- openSUSE: install the package from the official repo
```
sudo zypper in applet-window-buttons
```

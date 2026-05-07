#!/usr/bin/env bash
# Manual equivalent of ansible/roles/raspberrypi -- for reference, not execution.

# tasks/main.yml: set hostname
sudo hostnamectl set-hostname <host>

# tasks/main.yml: enable camera/I2C/SPI
sudo raspi-config nonint do_camera 0
sudo raspi-config nonint do_i2c 0
sudo raspi-config nonint do_spi 0

#!/usr/bin/env bash
# Manual equivalent of ansible/roles/base_setup -- for reference, not execution.

# tasks/main.yml: update + upgrade packages
sudo apt update && sudo apt full-upgrade -y

# tasks/main.yml: install common packages
sudo apt install -y git build-essential python3 python3-pip vim htop tmux

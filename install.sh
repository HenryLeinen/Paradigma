#! /bin/sh

echo "Copying executable..."
sudo mkdir /usr/local/bin/heizung
sudo cp ./heizungd /usr/local/bin/heizung/

echo "Copying startup script..."
sudo cp ./heizungd.sh /etc/init.d/

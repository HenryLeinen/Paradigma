#! /bin/sh

echo "Copying executable..."
sudo cp ./heizungd /usr/local/bin/heizung/

echo "Copying startup script..."
sudo cp ./heizungd.sh /etc/init.d/

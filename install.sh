#! /bin/sh

echo "Trying to stop any running service instance..."
sudo service heizungd.sh stop

echo "Copying executable..."
sudo mkdir /usr/local/bin/heizung
sudo cp ./heizungd /usr/local/bin/heizung/

echo "Copying startup script..."
sudo cp ./heizungd.sh /etc/init.d/

echo "Starting the service..."
sudo service heizungd.sh start

#!/bin/bash
#
# Daemonn Name: heizungd
#
# chkconfig: - 58 74
# description: Scipt handles the heizung daemon
# author: Henry Leinen

# Source function library.
. /lib/lsb/init-functions

# The directory in which to find the daemon script or executable
DIR=/usr/local/bin/heizung

# The daemon script or executable
DAEMON=$DIR/heizungd

# The name of the daemon
DAEMON_NAME=heizungd

# The next line determines what user the script runs as.
# Root generally not recommended but necessary if you are using the Raspberry Pi GPIO.
DAEMON_USER=root

# The process ID of the script when it runs is stored here
PIDFILE=/var/run/$DAEMON_NAME.pid

# Any options we want to pass to the daemon
DAEMON_OPTS=$PIDFILE

do_start() {
	# Start our daemon daemon
	echo -n $"Starting system daemon $prog."
	start-stop-daemon --start --pidfile "$PIDFILE" --user $DAEMON_USER --chuid $DAEMON_USER --startas $DAEMON --exec $DAEMON -- $DAEMON_OPTS
	log_end_msg $?
	RETVAL=$?
}

do_stop() {
	echo -n $"Stopping system daemon $prog."
	start-stop-daemon --stop  --pidfile $PIDFILE --retry 10
	log_end_msg $?
	RETVAL=$?
}

# See how we were called.
case "$1" in
  start|stop)
	do_${1}
	;;
  status)
	status_of_proc "$DAEMON_NAME" "$DAEMON" && exit 0 || exit $?
	;;
  restart|reload|force-reload)
	do_stop
	do_start
	;;
  *)
	echo $"Usage: $0 {start|stop|status|restart}"
	exit 2
esac
exit 0

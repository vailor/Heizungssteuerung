#!/bin/bash

# Hole Temperaturwerte von den Sensoren und speichere sie in RR Datenbank
#
# Pfad zu digitemp
DIGITEMP=/usr/bin/digitemp_DS9097
# DS Namen der vorhandenen Sensoren
SENSORS=temp0:temp1:temp2:temp3:temp4
# Wechsle ins Verzeichnis mit Datenbank und .digitemprc
cd /home/pi/temperature

data=`$DIGITEMP -a -q -o 2 | awk 'NF>1 {OFS=":"; $1="N"; print}'`
if [ -n "$data" ] ; then
        rrdtool update temperature.rrd -t $SENSORS $data
	set `rrdtool lastupdate temperature.rrd | grep ":"`
	shift
	echo $1 > ramtmp_ram/heizkessel
	echo $2 > ramtmp_ram/puffer_oben
	echo $3 > ramtmp_ram/puffer_mitte
	echo $4 > ramtmp_ram/aussen
	echo $5 > ramtmp_ram/puffer_unten
fi


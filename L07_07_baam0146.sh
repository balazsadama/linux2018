#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: Irjunk felügyelő programot, amely a joe illetve vi(m) programok használatát
# figyeli a rendszerben. 5 másodpercenként lementi, hogy hány különböző felhasználó
# futtat joe-t, illetve hányan használnak vi(m)-t amennyiben az előző megfigyelésnél
# valamelyik felhasználó használta a vi(m)-t, a jelenleginél viszont már nem futtatja,
# írjuk ki a képernyőre, hogy "x intelligens felhaszáló :)", és amennyiben az illető
# csoporttárs, küldjünk neki (poénból) egy elismerő üzenetet, amiért sikeresen kilépett a vi-ból.
# Percenként írjuk ki, hogy:
#    az utóbbi 1 percben, illetve a program indítása óta történt megfigyeléseket figyelembe
# véve  mennyi és mikor volt a "rekord" a joe-t illetve vi(m)-t használók számát tekintve.
#    az utóbbi egy perc megfigyeléseinek átlagát tekintve nőtt-e vagy csökkent a joe-t,
# illetve vi(m)-t használók száma az előző megfigyelési ciklushoz képest.    

if [ $# -gt 0 ]
then
        echo 'No parameters needed'
        exit 1
fi


secondsSinceLastPrint=0		# ha  60 akkor 0-ra allitjuk es kiirjuk amiket ker a feladat
recordJoeNumber=0
recordJoeTime=`date +"%x %T"`
totalJoeUsersThisCycle=0
totalJoeUsersLastCycle=0
recordVimNumber=0
recordVimTime=`date +"%x %T"`
totalVimUsersThisCycle=0
totalVimUsersLastCycle=0

declare -A lastVimCheck

users=`users`
for u in $users
do
	lastVimCheck[$u]=0
done

while :
do
	users=`users`
	for u in $users
	do
		found=0
		for i in ${!lastVimCheck[@]}
		do
			if [ "$u" == "$i" ]
			then
				found=1
			fi
		done

		if [ $found -eq 0 ]
		then
			lastVimCheck[$u]=0
		fi
	done

	usingVimNow=0
	usingJoeNow=0
	for user in $users
	do
		numberOfVimOpenByThisUser=`ps -u $user | egrep -wc "vi[m]{0,1}"`
		numberOfJoeOpenByThisUser=`ps -u $user | egrep -wc "joe"`
		
		if [ $numberOfVimOpenByThisUser -eq 0 ]
		then
			if [ ${lastVimCheck[$user]} -gt 0 ]
			then
				echo $user ' intelligens felhasznalo'

				sameGroup=`ls -1 ../ | grep $user | wc -l`
				echo $samegroup
				if [ $sameGroup -gt 0 ]
				then
					echo 'Sikeresen kilepett Vim-bol!' | write $user
				fi

			fi
		else
			usingVimNow=$((usingVimNow + 1))
		fi
		lastVimCheck[$user]=$numberOfVimOpenByThisUser


		if [ $numberOfJoeOpenByThisUser -gt 0 ]
		then
			usingJoeNow=$((usingJoeNow + 1))
                fi
	done

	
	if [ $usingVimNow -gt $recordVimNumber ]
	then
		recordVimNumber=$usingVimNow
		recordVimTime=`date +"%x %T"`
	fi

	if [ $usingJoeNow -gt $recordJoeNumber ]
	then
		recordJoeNumber=$usingJoeNow
		recordJoeTime=`date +"%x %T"`
	fi

	if [ $secondsSinceLastPrint -eq 60 ]
	then
		echo 'Record Joe usage was ' $recordJoeNumber ' at ' $recordJoeTime
		echo 'Record Vim usage was ' $recordVimNumber ' at ' $recordVimTime


		echo 'Joe' $totalJoeUsersThisCycle ' ' $totalJoeUsersLastCycle
		echo 'Vim' $totalVimUsersThisCycle ' ' $totalVimUsersLastCycle
		if [ $totalJoeUsersThisCycle -ge $totalJoeUsersLastCycle ]
		then
			echo 'There are more Joe users this cycle'
		else
			echo 'There are less Joe users this cycle'
		fi

		if [ $totalVimUsersThisCycle -ge $totalVimUsersLastCycle ]
                then
                        echo 'There are more Vim users this cycle'
                else
                        echo 'There are less Vim users this cycle'
                fi

		totalJoeUsersLastCycle=$totalJoeUsersThisCycle
		totalVimUsersLastCycle=$totalVimUsersThisCycle
		totalJoeUsersThisCycle=0
		totalVimUsersThisCycle=0

		secondsSinceLastPrint=0
	else
		totalJoeUsersThisCycle=$((totalJoeUsersThisCycle + $usingJoeNow))
		totalVimUsersThisCycle=$((totalVimUsersThisCycle + $usingVimNow))
	fi

	sleep 5
	secondsSinceLastPrint=$((secondsSinceLastPrint + 30))
done

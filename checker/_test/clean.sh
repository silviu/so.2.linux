#!/bin/bash

## Script to clean all shared IPC objects after improper closure

#Lists with names of ipc objects as they appear in the syscall
#Ex: shm_open("/my_shm".. ) -> add "my_shm" to SHM_LIST
#    sem_open("/my_sem".. ) -> add "my_sem" to SEM_LIST

SEM_LIST="$1 "
SHM_LIST="$1 "
QUEUE_LIST="gateway __server[0-4]"

function delete_ipc ()
{
	LIST=$1
	PREFIX=$2

	for ipc in $LIST; do
		name_list="$PREFIX$ipc"
		for name in $name_list; do
			if [ -f $name ];then
				echo "deleting $name"
				rm -f $name
			fi
		done
	done
}

delete_ipc "$SEM_LIST" "/dev/shm/sem."
delete_ipc "$SHM_LIST" "/dev/shm/"
delete_ipc "$QUEUE_LIST" "/dev/mqueue/"

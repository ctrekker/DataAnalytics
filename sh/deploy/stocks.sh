#!/bin/bash

# Include configuration values
source sh/deploy/stocks.config

# Set up a temporary directory
mkdir tmp
# Make sure the prediction archive repository path exists
mkdir $ARCHIVE_ROOT
mkdir $ARCHIVE_ROOT/predictions
mkdir $ARCHIVE_ROOT/save

# Download newline separated list of stock collections
node sh/util/mdb mdb.json mdbcollections > tmp/collections.txt

DATE=`cat execution_date.txt`
echo "Execution Date: $DATE"

# Create a temporary mongo batch file
touch tmp/symbol_download.msh
# Loop through each line of the file
while read collection; do
    # Add the download command to the script
    printf "mdbfind $collection timestamp,ASC {\`date\`:\`$DATE\`} > tmp/$collection.csv\r\n" >> tmp/symbol_download.msh
done <tmp/collections.txt

# Execute the batch script which downloads all files
node sh/util/mdb mdb.json mdbbatch "tmp/symbol_download.msh"

# Check if ramdisk option is set
SAVE_OPTION=""
if [ $RAMDISK_SAVE -eq $true ]; then
    if [ $RAMDISK_PERFORM_MOUNT -eq $true ]; then
        mkdir -p $RAMDISK_MOUNT_PATH
        mount -t tmpfs -o size=$RAMDISK_SIZE tmpfs $RAMDISK_MOUNT_PATH
    fi
    # Set the save option to append to the run commands
    SAVE_OPTION=" -a $RAMDISK_MOUNT_PATH"
    # Copy the current disk configuration to the ramdisk
    cp save/* $RAMDISK_MOUNT_PATH
fi

COLLECTION_COUNT=`cat tmp/collections.txt | wc -l`
COMPLETION_COUNT=0
while read collection; do
  ((COMPLETION_COUNT+=1))
  echo "$collection - $COMPLETION_COUNT / $COLLECTION_COUNT"
  # Format for import
  node sh/util/csvformat tmp/$collection.csv close > tmp/$collection-import.csv
  # Import the csv import to the local repo
  ./da2 import -p tmp/$collection-import.csv -n $collection
  # Run the training algorithm
  ./da2 run -p -t -s $collection -n $collection $SAVE_OPTION
  # Clean tmp/ files
  rm tmp/$collection.csv
  rm tmp/$collection-import.csv
done <tmp/collections.txt
COMPLETION_COUNT=0
# Predict outcomes
while read collection; do
  ((COMPLETION_COUNT+=1))
  echo "$collection - $COMPLETION_COUNT / $COLLECTION_COUNT"
  ./da2 run -r -s $collection -n $collection $SAVE_OPTION
done <tmp/collections.txt

if [ $RAMDISK_SAVE -eq $true ]; then
    rm save/*
    cp -r $RAMDISK_MOUNT_PATH save
    umount $RAMDISK_MOUNT_PATH
fi


# Clean out the input repository
rm in/*
# Export data to the prediction archive
tar -cvf $ARCHIVE_ROOT/predictions/data_$DATE.tar.bz2 --use-compress-prog=pbzip2 data
# Export save data to the save archive
tar -cvf $ARCHIVE_ROOT/save/save_$DATE.tar.bz2 --use-compress-prog=pbzip2 save
if [ $FTP_UPLOAD -eq $true ]; then
    # Upload the tar backups to a remote ftp backup server
    node sh/util/ftpupload ftp.json "$ARCHIVE_ROOT/predictions/data_$DATE.tar.bz2" "$FTP_ARCHIVE_ROOT/predictions/data_$DATE.tar.bz2"
    node sh/util/ftpupload ftp.json "$ARCHIVE_ROOT/save/save_$DATE.tar.bz2" "$FTP_ARCHIVE_ROOT/save/save_$DATE.tar.bz2"
fi

rm data/*
# Clean tmp directory
rm -r tmp

# Remove execution date now that execution has completed
rm execution_date.txt

echo "Completed execution for $DATE"

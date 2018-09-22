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

while read collection; do
  echo "$collection"
  # Format for import
  node sh/util/csvformat tmp/$collection.csv close > tmp/$collection-import.csv
  # Import the csv import to the local repo
  ./da2 import -p tmp/$collection-import.csv -n $collection
  # Run the training algorithm
  ./da2 run -p -t -s $collection -n $collection
  # Clean tmp/ files
  rm tmp/$collection.csv
  rm tmp/$collection-import.csv
done <tmp/collections.txt
# Predict outcomes
while read collection; do
  echo "$collection"
  ./da2 run -r -s $collection -n $collection
done <tmp/collections.txt

# Clean out the input repository
rm in/*
# Export data to the prediction archive
tar -cvf $ARCHIVE_ROOT/predictions/data_$DATE.tar.bz2 --use-compress-prog=pbzip2 data
# Export save data to the save archive
tar -cvf $ARCHIVE_ROOT/save/save_$DATE.tar.bz2 --use-compress-prog=pbzip2 save
# Upload the tar backups to a remote ftp backup server
node sh/util/ftpupload ftp.json "$ARCHIVE_ROOT/predictions/data_$DATE.tar.bz2" "$FTP_ARCHIVE_ROOT/predictions/data_$DATE.tar.bz2"
node sh/util/ftpupload ftp.json "$ARCHIVE_ROOT/save/save_$DATE.tar.bz2" "$FTP_ARCHIVE_ROOT/save/save_$DATE.tar.bz2"

rm data/*
# Clean tmp directory
rm -r tmp

# Remove execution date now that execution has completed
rm execution_date.txt

echo "Completed execution for $DATE"

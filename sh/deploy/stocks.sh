#!/bin/bash

# Set up a temporary directory
mkdir tmp

# Download newline separated list of stock collections
node sh/util/mdb mdb.json mdbcollections > tmp/collections.txt

DATE=`cat execution_date.txt`
echo "Execution Date: $DATE"
rm execution_date.txt

# Create a temporary mongo batch file
touch tmp/symbol_download.msh
# Loop through each line of the file
while read collection; do
    # Add the download command to the script
    echo "mdbfind $collection timestamp,ASC {date \`date\`: \`$DATE\` } > tmp/$collection.csv" > tmp/symbol_download.msh
done <tmp/collections.txt

# Execute the batch script which downloads all files
node sh/util/mdb mdb.json mdbbatch "tmp/symbol_download.msh"

while read collection; do
  echo "$collection"
  # Download the csv data for the current collection
  # node sh/util/mdb mdb.json mdbfind "$collection timestamp,ASC { \`date\`: \`$DATE\` }" > tmp/$collection.csv
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

# Clean out the input repository
rm in/*
# Export data out in zip format (TEMPORARY)
zip -r data_$DATE.zip data
rm data/*
# Clean tmp directory
rm -r tmp

echo "Completed execution for $DATE"

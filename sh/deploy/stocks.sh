#!/bin/bash

# Set up a temporary directory
mkdir tmp

# Download newline separated list of stock collections
node sh/util/mdb mdb.json mdbcollections > tmp/collections.txt

DATE=`date +%Y-%m-%d`
#DATE="2018-08-24"

# Loop through each line of the file
while read collection; do
  echo "$collection"
  # Download the csv data for the current collection
  node sh/util/mdb mdb.json mdbfind "$collection timestamp,ASC { \`date\`: \`$DATE\` }" > tmp/$collection.csv
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

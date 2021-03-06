package com.burnscoding.stocks;

import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.model.*;
import org.bson.Document;
import org.bson.conversions.Bson;
import org.json.JSONException;
import org.json.JSONObject;
import pl.zankowski.iextrading4j.api.refdata.ExchangeSymbol;
import pl.zankowski.iextrading4j.api.stocks.Chart;
import pl.zankowski.iextrading4j.api.stocks.ChartRange;
import pl.zankowski.iextrading4j.client.IEXTradingClient;
import pl.zankowski.iextrading4j.client.rest.request.refdata.SymbolsRequestBuilder;
import pl.zankowski.iextrading4j.client.rest.request.stocks.ChartRequestBuilder;

import java.io.*;
import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.util.*;

public class IntradayDownloader {
    private static MongoDatabase db;
    public static void main(String[] args) {
        // Obtain configuration File object
        File configFile = new File("config/IntradayDownload.json");
        if(args.length>0) {
            File tmpConfigFile = new File(args[0]);
            if(tmpConfigFile.exists()) {
                configFile = tmpConfigFile;
            }
            else {
                System.out.println("WARNING: No configuration exists at " + tmpConfigFile.getAbsolutePath());
            }
        }
        System.out.println("Using configuration at " + configFile.getAbsolutePath());

        // Read/parse the configuration file
        JSONObject config = null;
        try {
            BufferedReader configReader = new BufferedReader(new InputStreamReader(new FileInputStream(configFile)));
            StringBuilder configBuilder = new StringBuilder();
            String line;
            while((line = configReader.readLine()) != null) {
                configBuilder.append(line).append("\n");
            }

            config = new JSONObject(configBuilder.toString());
        }
        catch(IOException e) {
            System.out.println("ERROR: An error occurred while reading the configuration file. Printing trace...");
            e.printStackTrace();
            System.exit(1);
        }
        catch(JSONException e) {
            System.out.println("ERROR: An error occurred while parsing the configuration file. Printing trace...");
            e.printStackTrace();
            System.exit(1);
        }

        // Connect to the mongo database
        try {
            JSONObject dbconfig = config.getJSONObject("mongo_database");

            MongoClient mongoClient = MongoClients.create(String.format("mongodb://%s:%s", dbconfig.getString("host"), Integer.toString(dbconfig.getInt("port"))));
            db = mongoClient.getDatabase(dbconfig.getString("database"));
        }
        catch(JSONException e) {
            System.out.println("ERROR: An error occurred while attempting to use configuration values. Printing trace...");
            e.printStackTrace();
            System.exit(1);
        }
        new IntradayDownloader();
    }

    private IEXTradingClient tradingClient;
    private List<ExchangeSymbol> symbolList;
    public IntradayDownloader() {
        tradingClient = IEXTradingClient.create();
        symbolList = tradingClient.executeRequest(new SymbolsRequestBuilder().build());
        int symbolNumber = 0;
        int lastDelay = 0;
        for(ExchangeSymbol symbol : symbolList) {
            new Thread(new DownloadThread(symbol, symbolNumber)).start();
            symbolNumber++;
            if(symbolNumber - lastDelay > 10) {
                lastDelay = symbolNumber;
                try {
                    Thread.sleep(1000);
                } catch(InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        // Write to execution_date.txt
        File executionDateFile = new File("execution_date.txt");
        if(executionDateFile.exists()) {
            executionDateFile.delete();
        }

        Date executionDate = new Date();
        String executionDateStr = new SimpleDateFormat("yyyy-MM-dd").format(executionDate);
        System.out.println("Using date " + executionDateStr + " for execution_date.txt");
        try {
            BufferedWriter writer = new BufferedWriter(new FileWriter(executionDateFile, false));
            writer.write(executionDateStr);
            writer.close();
        }
        catch(IOException e) {
            e.printStackTrace();
        }
    }

    private class DownloadThread implements Runnable {
        private ExchangeSymbol symbol;
        private int symbolNumber;
        public DownloadThread(ExchangeSymbol symbol, int symbolNumber) {
            this.symbol = symbol;
            this.symbolNumber = symbolNumber;
        }

        @Override
        public void run() {
            try {
                List<Chart> chartList = tradingClient.executeRequest(new ChartRequestBuilder()
                        .withChartRange(ChartRange.INTRADAY)
                        .withSymbol(symbol.getSymbol())
                        .build());
                List<Bson> entryDocuments = new ArrayList<>();
                for (Chart entry : chartList) {
                    if (entry.getMarketAverage() == null || entry.getMarketAverage().equals(new BigDecimal(-1)) || entry.getMarketClose().doubleValue() <= 0.0)
                        continue;
                    //System.out.println(entry.getMinute());

                    String s_year = entry.getDate().substring(0, 4);
                    String s_month = entry.getDate().substring(4, 6);
                    String s_day = entry.getDate().substring(6, 8);
                    int year = Integer.parseInt(s_year);
                    int month = Integer.parseInt(s_month);
                    int day = Integer.parseInt(s_day);
                    String formattedDate = s_year + "-" + s_month + "-" + s_day;

                    String s_hour = entry.getMinute().substring(0, 2);
                    String s_minute = entry.getMinute().substring(3, 5);
                    int hour = Integer.parseInt(s_hour);
                    int minute = Integer.parseInt(s_minute);
                    String formattedTime = s_hour + ":" + s_minute + ":00";
                    //System.out.println(formattedTime);

                    entryDocuments.add(new Document("timestamp", new GregorianCalendar(year, month - 1, day, hour, minute).getTime())
                            .append("date", formattedDate)
                            .append("time", formattedTime)
                            .append("open", entry.getMarketOpen())
                            .append("high", entry.getMarketHigh())
                            .append("low", entry.getMarketLow())
                            .append("close", entry.getMarketClose())
                            .append("volume", entry.getMarketVolume()));
                    //System.out.println(entryDocuments.get(entryDocuments.size()-1));
                }
                MongoCollection<Document> dbCollection = db.getCollection(symbol.getSymbol());
                if (entryDocuments.size() > 0) {
                    ReplaceOptions options = new ReplaceOptions()
                            .upsert(true);
                    for(Bson bson : entryDocuments) {
                        dbCollection.replaceOne(new Document("timestamp", ((Document)bson).getDate("timestamp")), ((Document)bson), options);
                    }
                    //dbCollection.bulkWrite(entryDocuments);
                }

                symbolNumber++;
                System.out.println("Finished symbol " + symbolNumber + " / " + symbolList.size());
            }
            catch(Exception e) {
                //System.out.println("An error occurred " + symbolNumber + " / " + symbolList.size());
                e.printStackTrace();
            }
        }
    }
}

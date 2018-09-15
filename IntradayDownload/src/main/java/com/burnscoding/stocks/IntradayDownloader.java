package com.burnscoding.stocks;

import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.model.InsertOneModel;
import com.mongodb.client.model.WriteModel;
import org.bson.Document;
import pl.zankowski.iextrading4j.api.refdata.ExchangeSymbol;
import pl.zankowski.iextrading4j.api.stocks.Chart;
import pl.zankowski.iextrading4j.api.stocks.ChartRange;
import pl.zankowski.iextrading4j.client.IEXTradingClient;
import pl.zankowski.iextrading4j.client.rest.request.refdata.SymbolsRequestBuilder;
import pl.zankowski.iextrading4j.client.rest.request.stocks.ChartRequestBuilder;

import java.math.BigDecimal;
import java.util.*;

public class IntradayDownloader {
    private static String API_BASE_URL = "https://api.iextrading.com/1.0";
    private static MongoDatabase db;
    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create("mongodb://192.168.1.18:27017");
        db = mongoClient.getDatabase("stockdata");
        new IntradayDownloader();
    }

    public IntradayDownloader() {
        IEXTradingClient tradingClient = IEXTradingClient.create();
        List<ExchangeSymbol> symbolList = tradingClient.executeRequest(new SymbolsRequestBuilder().build());
        int symbolNumber = 0;
        for(ExchangeSymbol symbol : symbolList) {
            List<Chart> chartList = tradingClient.executeRequest(new ChartRequestBuilder()
                    .withChartRange(ChartRange.INTRADAY)
                    .withSymbol(symbol.getSymbol())
                    .build());
            List<WriteModel<Document>> entryDocuments = new ArrayList<WriteModel<Document>>();
            for(Chart entry : chartList) {
                if(entry.getMarketAverage()==null||entry.getMarketAverage().equals(new BigDecimal(-1))) continue;
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

                entryDocuments.add(new InsertOneModel<>(new Document("timestamp", new GregorianCalendar(year, month-1, day, hour, minute).getTime())
                        .append("date", formattedDate)
                        .append("time", formattedTime)
                        .append("open", entry.getMarketOpen())
                        .append("high", entry.getMarketHigh())
                        .append("low", entry.getMarketLow())
                        .append("close", entry.getMarketClose())
                        .append("volume", entry.getMarketVolume())));
                //System.out.println(entryDocuments.get(entryDocuments.size()-1));
            }
            MongoCollection<Document> dbCollection = db.getCollection(symbol.getSymbol());
            if(entryDocuments.size()>0) {
                dbCollection.bulkWrite(entryDocuments);
            }

            symbolNumber++;
            System.out.println("Finished symbol "+symbolNumber+" / "+symbolList.size());
        }

//        StockIndex index = StockIndex.fromUrl(API_BASE_URL + "/ref-data/symbols");
//        if(index == null) {
//            System.out.println("ERROR: Stock index is null!");
//            System.exit(1);
//        }
//
//        Set<String> symbols = index.keySet();
//        for(String symbol : symbols) {
//            StockData symbolData = StockData.fromUrl(API_BASE_URL + "/stock/" + symbol + "/chart/1d");
//            if(symbolData == null) {
//                System.out.println("WARNING: Stock symbol data for " + symbol + " is null!");
//                continue;
//            }
//            symbolData.mongoInsert();
//        }


    }
}

package com.burnscoding.stocks;

import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoDatabase;
import pl.zankowski.iextrading4j.api.refdata.ExchangeSymbol;
import pl.zankowski.iextrading4j.api.stocks.Chart;
import pl.zankowski.iextrading4j.api.stocks.ChartRange;
import pl.zankowski.iextrading4j.client.IEXTradingClient;
import pl.zankowski.iextrading4j.client.rest.request.refdata.SymbolsRequestBuilder;
import pl.zankowski.iextrading4j.client.rest.request.stocks.ChartRequestBuilder;

import java.math.BigDecimal;
import java.util.List;
import java.util.Set;

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
        for(ExchangeSymbol symbol : symbolList) {
            List<Chart> chartList = tradingClient.executeRequest(new ChartRequestBuilder()
                    .withChartRange(ChartRange.INTRADAY)
                    .withSymbol(symbol.getSymbol())
                    .build());
            for(Chart entry : chartList) {
                if(entry.getMarketAverage()==null||entry.getMarketAverage().equals(new BigDecimal(-1))) continue;
                //System.out.println(entry.getMarketClose());
            }
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

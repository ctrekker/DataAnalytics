package com.burnscoding.stocks;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;

public class StockData extends ArrayList<StockDataEntry> {
    public StockData() {

    }


    public static StockData fromUrl(String url) {
        try {
            StockData out = new StockData();

            String jsonRaw = HttpUtils.sendGet(url);
            JSONArray json = new JSONArray(jsonRaw);
            for(Object jsonObj : json) {
                if(jsonObj instanceof JSONObject) {
                    JSONObject entryJson = ((JSONObject)jsonObj);
                    System.out.println(entryJson.toString(4));
                    out.add(new StockDataEntry(
                                    entryJson.getDouble("marketOpen"),
                                    entryJson.getDouble("marketHigh"),
                                    entryJson.getDouble("marketLow"),
                                    entryJson.getDouble("marketClose"),
                                    entryJson.getInt("volume")));
                }
            }

            return out;
        } catch(Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Inserts all stock data within object to a mongo server specified in the parameter
     */
    public void mongoInsert() {

    }
}

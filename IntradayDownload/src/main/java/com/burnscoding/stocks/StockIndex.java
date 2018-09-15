package com.burnscoding.stocks;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.HashMap;

public class StockIndex extends HashMap<String, StockMeta> {
    public StockIndex() {

    }

    @Override
    public String toString() {
        return "";
    }

    public static StockIndex fromUrl(String url) {
        try {
            StockIndex out = new StockIndex();
            String jsonRaw = HttpUtils.sendGet(url);
            JSONArray json = new JSONArray(jsonRaw);

            for(Object symbolObj : json) {
                if(symbolObj instanceof JSONObject) {
                    JSONObject symbolJson = ((JSONObject)symbolObj);
                    out.put(symbolJson.getString("symbol"), new StockMeta(symbolJson.getString("symbol"), symbolJson.getString("name")));
                }
            }

            return out;
        } catch(Exception e) {
            e.printStackTrace();

            return null;
        }
    }
}

package com.burnscoding.esdac;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.DataOutputStream;
import java.io.IOException;

public class EsdacRequestBuilder {
    private JSONObject request;
    public EsdacRequestBuilder() {
        request = new JSONObject();
        request.put("command", EsdacCommand.ECHO);
        request.put("message", "<null>");
        request.put("extra", new JSONArray());
    }
    public EsdacRequestBuilder command(EsdacCommand command) {
        request.put("command", command.toString());
        return this;
    }
    public EsdacRequestBuilder message(String message) {
        request.put("message", message);
        return this;
    }
    public EsdacRequestBuilder extra(JSONArray extra) {
        request.put("extra", extra);
        return this;
    }
    public JSONObject build() {
        return request;
    }
    public void send(DataOutputStream out) throws IOException {
        EsdacUtil.writeString(out, build().toString());
    }
}

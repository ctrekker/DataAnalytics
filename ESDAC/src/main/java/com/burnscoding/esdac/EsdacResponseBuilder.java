package com.burnscoding.esdac;

import org.json.JSONObject;

import java.io.DataOutputStream;
import java.io.IOException;

public class EsdacResponseBuilder {
    private JSONObject response;
    public EsdacResponseBuilder() {
        response = new JSONObject();
        response.put("type", EsdacResponseType.ERROR.toString());
        response.put("message", "<null>");
        response.put("code", -1);
    }
    public EsdacResponseBuilder type(EsdacResponseType type) {
        response.put("type", type.toString());
        return this;
    }
    public EsdacResponseBuilder message(String message) {
        response.put("message", message);
        return this;
    }
    public EsdacResponseBuilder code(int code) {
        response.put("code", code);
        return this;
    }
    public EsdacResponseBuilder extra(JSONObject extra) {
        response.put("extra", extra);
        return this;
    }
    public JSONObject build() {
        return response;
    }
    public void send(DataOutputStream out) throws IOException {
        EsdacUtil.writeString(out, build().toString());
    }
}

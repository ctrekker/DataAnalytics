package com.burnscoding.esdac;

import org.json.JSONObject;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

// NOTE: This class follows the "AUTO" protocol for
//       performing operations
public class EsdacClient {
    private String host;
    private String protocol;
    private Socket conn;
    private DataInputStream clientIn;
    private DataOutputStream clientOut;
    private boolean valid = false;


    public EsdacClient(String host, String protocol) {
        this.host = host;
        this.protocol = protocol;
        try {
            this.conn = new Socket(host, 13102);
            clientIn = new DataInputStream(conn.getInputStream());
            clientOut = new DataOutputStream(conn.getOutputStream());

            // Write the protocol string
            EsdacUtil.readString(clientIn);
            clientOut.write(protocol.getBytes());
            valid = true;
        }
        catch(IOException e) {
            e.printStackTrace();
        }
    }
    public String request(JSONObject request) throws IOException {
        if(!valid) return null;
        EsdacUtil.writeString(clientOut, request.toString());

        return EsdacUtil.readString(clientIn);
    }
    public String close() throws IOException {
        String res = request(new EsdacRequestBuilder().command(EsdacCommand.EXIT).build());
        conn.close();

        return res;
    }

    public String getHost() {
        return host;
    }

    public boolean isValid() {
        return valid;
    }

    public String getProtocol() {
        return protocol;
    }
}

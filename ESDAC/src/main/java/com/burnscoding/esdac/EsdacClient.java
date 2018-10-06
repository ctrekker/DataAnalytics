package com.burnscoding.esdac;

import org.json.JSONObject;

import java.io.*;
import java.net.Socket;
import java.util.Arrays;

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

        if(request.has("extra")) {
            JSONObject extra = request.getJSONObject("extra");
            switch(extra.getString("type").toUpperCase()) {
                case "FILE":
                    fileTransfer(extra);
                    break;
            }
        }

        return EsdacUtil.readString(clientIn);
    }
    public void fileTransfer(JSONObject extra) throws IOException {
        final int chunkSize = 2048;

        File localFile = new File(extra.getString("localPath"));
        DataInputStream fileReader = new DataInputStream(new FileInputStream(localFile));
        long byteNum = localFile.length();
        clientOut.writeLong(byteNum);

        long currentIndex = 0;

        long lastPrint = System.currentTimeMillis();
        long start = System.currentTimeMillis();
        while(currentIndex < byteNum) {
            byte[] buffer = new byte[chunkSize];
            fileReader.read(buffer);
            clientOut.write(buffer);

            if(System.currentTimeMillis() - lastPrint > 500) {
                System.out.println((((double)currentIndex/byteNum)*100)+"%");
                lastPrint = System.currentTimeMillis();
            }

            currentIndex += chunkSize;
        }

        System.out.println("Finished in "+(System.currentTimeMillis()-start)+"ms");

        fileReader.close();
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

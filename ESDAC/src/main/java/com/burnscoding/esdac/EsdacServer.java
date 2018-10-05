package com.burnscoding.esdac;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class EsdacServer implements Runnable {
    private ServerSocket serverSocket = null;
    private Thread serverThread;

    public EsdacServer() {

    }
    public void start() {
        Thread st = new Thread(this, "");
        st.start();
        this.serverThread = st;
    }

    @Override
    public void run() {
        try {
            serverSocket = new ServerSocket(13102);
        } catch(IOException e) {
            e.printStackTrace();
        }

        if(serverSocket != null) {
            while(true) {
                try {
                    Thread connHandler = new Thread(new EsdacConnectionHandler(serverSocket.accept()));
                    connHandler.start();
                } catch(IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public Thread getServerThread() {
        return serverThread;
    }

    class EsdacConnectionHandler implements Runnable {
        private Socket client;
        public EsdacConnectionHandler(Socket client) {
            this.client = client;
        }

        @Override
        public void run() {
            EsdacProtocol protocol = new EsdacProtocol();
            try {
                DataInputStream clientIn = new DataInputStream(client.getInputStream());
                DataOutputStream clientOut = new DataOutputStream(client.getOutputStream());

                EsdacUtil.writeString(clientOut, "ENTER CONNECTION PROTOCOL: ");

                char protocolString = (char)clientIn.readByte();

                if(protocolString=='T') {
                    protocol.tel(clientIn, clientOut);
                }
                else if(protocolString=='A') {
                    protocol.auto(clientIn, clientOut);
                }
                else {
                    clientOut.write(("UNKNOWN PROTOCOL "+protocolString).getBytes());
                    client.close();
                }
            } catch(IOException e) {
                e.printStackTrace();
            }
        }
        class EsdacProtocol {
            public void tel(DataInputStream clientIn, DataOutputStream clientOut) throws IOException {
                EsdacCommandExecutor executor = new EsdacCommandExecutor(client, clientIn, clientOut);
                while(clientIn.available()>0) {
                    clientIn.read();
                }
                clientOut.write("\r\n".getBytes());
                while(!client.isClosed()) {
                    clientOut.write("ENTER REQUEST: ".getBytes());

                    StringBuilder requestBuilder = new StringBuilder();
                    char last;
                    while((last=(char)clientIn.readByte())!='~') {
                        requestBuilder.append(last);
                    }
                    clientOut.write("\r\n".getBytes());

                    try {
                        executor.execute(new JSONObject(requestBuilder.toString()));
                    } catch(JSONException e) {
                        clientOut.write("ERROR PARSING COMMAND. PRINTING TRACE\r\n".getBytes());

                        StringWriter strWriter = new StringWriter();
                        PrintWriter writer = new PrintWriter(strWriter);
                        e.printStackTrace(writer);
                        clientOut.write(strWriter.toString().getBytes());
                    }
                }
            }
            public void auto(DataInputStream clientIn, DataOutputStream clientOut) throws IOException {
                EsdacCommandExecutor executor = new EsdacCommandExecutor(client, clientIn, clientOut);
                while(clientIn.available()>0) {
                    clientIn.read();
                }
                while(!client.isClosed()) {
                    JSONObject request = new JSONObject(EsdacUtil.readString(clientIn));
                    System.out.println(request);
                    executor.execute(request);
                }
            }
        }
    }
}

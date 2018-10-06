package com.burnscoding.esdac;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;
import java.net.Socket;

public class EsdacCommandExecutor {
    private Socket client;
    private DataInputStream clientIn;
    private DataOutputStream clientOut;

    private JSONObject lastReq;
    public EsdacCommandExecutor(Socket client, DataInputStream clientIn, DataOutputStream clientOut) {
        this.client = client;
        this.clientIn = clientIn;
        this.clientOut = clientOut;
    }
    public void execute(JSONObject request) throws IOException {
        lastReq = request;
        EsdacCommand command = EsdacCommand.valueOf(request.getString("command").toUpperCase());
        try {
            switch (command) {
                case RPS:
                    rps();
                    break;
                case ECHO:
                    echo();
                    break;
                case PREDICT:
                    predict();
                    break;
                case FILE_TRANSFER:
                    fileTransfer();
                    break;
                case SAVE_TRANSFER:
                    saveTransfer();
                    break;
                case PREDICTION_TRANSFER:
                    predictionTransfer();
                    break;
                case EXIT:
                    close();
                    break;
            }
            if (command != EsdacCommand.EXIT) {
                while (clientIn.available() > 0) {
                    clientIn.read();
                }
            }
        } catch(JSONException e) {
            e.printStackTrace();
            clientOut.writeUTF("COMMAND MISSING ARGUMENTS");
        }
    }

    // Command functions
    public void rps() throws IOException {
        while(true) {
            clientOut.write("Enter a selection (rock, paper, scissors, exit): ".getBytes());
            String response = clientIn.readLine();
            int r = -1;
            switch(response.toUpperCase()) {
                case "ROCK":
                    r = 0;
                    break;
                case "PAPER":
                    r = 1;
                    break;
                case "SCISSORS":
                    r = 2;
                    break;
                case "EXIT":
                    r = 3;
                    break;
            }
            if(r==-1) {
                clientOut.write("Please enter a valid selection\n".getBytes());
                continue;
            }
            if(r==3) {
                break;
            }
            int c = (int)(Math.random()*3);
            if(r==c) {
                clientOut.write("It's a tie!\n".getBytes());
            }
            else if((r==0&&c==2)||(r==1&&c==0)||(r==2&&c==1)) {
                clientOut.write("You win!\n".getBytes());
            }
            else {
                clientOut.write("You lose!\n".getBytes());
            }
        }
    }
    public void echo() throws IOException {
        new EsdacResponseBuilder()
                .type(EsdacResponseType.SUCCESS)
                .message(lastReq.getString("message"))
                .code(0)
                .send(clientOut);
    }
    public void predict() {

    }
    public void fileTransfer() throws IOException {
        JSONObject extra = lastReq.getJSONObject("extra");

        final int chunkSize = 2048;

        File outFile = new File(extra.getString("remotePath"));
        DataOutputStream fileWriter = new DataOutputStream(new FileOutputStream(outFile, false));

        long byteNum = clientIn.readLong();

        long currentIndex = 0;
        while(currentIndex < byteNum) {
            byte[] buffer = new byte[chunkSize];
            clientIn.readFully(buffer);

            if(byteNum - currentIndex < chunkSize) {
                byte[] trimmedBuffer = new byte[(int)(byteNum - currentIndex)];
                for(int i=0; i<((int)(byteNum - currentIndex)); i++) {
                    trimmedBuffer[i] = buffer[i];
                }
                fileWriter.write(trimmedBuffer);
            }
            else {
                fileWriter.write(buffer);
            }

            currentIndex += chunkSize;
        }

        String hash = EsdacUtil.fileHash(outFile);

        fileWriter.close();

        if(hash != null && hash.equalsIgnoreCase(extra.getString("hash"))) {
            new EsdacResponseBuilder()
                    .type(EsdacResponseType.SUCCESS)
                    .message("Transferred file")
                    .code(0)
                    .extra(new JSONObject("{\"hash\":\"" + hash + "\"}"))
                    .send(clientOut);
        }
        else {
            new EsdacResponseBuilder()
                    .type(EsdacResponseType.ERROR)
                    .message("Bad checksum")
                    .code(1)
                    .extra(new JSONObject("{\"hash\":\"" + hash + "\"}"))
                    .send(clientOut);
        }
    }
    public void saveTransfer() throws IOException {
        JSONObject extra = lastReq.getJSONObject("extra");

        File tmp = new File("tmp");
        tmp.mkdir();
        File tmpFile = new File("tmp/"+System.currentTimeMillis()+".tar.bz2");

        extra.put("remotePath", tmpFile.getPath());

        fileTransfer();
        Runtime.getRuntime().exec("tar -xvf "+tmpFile.getPath());
    }
    public void predictionTransfer() {

    }
    public void close() throws IOException {
        EsdacUtil.writeString(clientOut, "CLOSED");
        new EsdacResponseBuilder()
                .type(EsdacResponseType.SUCCESS)
                .message("Connection close scheduled")
                .code(0)
                .send(clientOut);
        client.close();
    }
}

package com.burnscoding.esdac;

import org.json.JSONObject;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class EsdacCommandExecutor {
    private Socket client;
    private DataInputStream clientIn;
    private DataOutputStream clientOut;
    public EsdacCommandExecutor(Socket client, DataInputStream clientIn, DataOutputStream clientOut) {
        this.client = client;
        this.clientIn = clientIn;
        this.clientOut = clientOut;
    }
    public void execute(String command) throws IOException {
        execute(command, new JSONObject());
    }
    public void execute(String command, JSONObject meta) throws IOException {
        try {
            execute(EsdacCommand.valueOf(command.toUpperCase()), meta);
        }
        catch(IllegalArgumentException e) {
            clientOut.write("COMMAND NOT FOUND\n".getBytes());
        }
    }
    public void execute(JSONObject request) throws IOException {
        if(request.has("command")) {
            if (request.has("meta")) {
                execute(request.getString("command"), request.getJSONObject("meta"));
            } else {
                execute(request.getString("command"));
            }
        }
        else {
            clientOut.write("NO COMMAND PROVIDED\n".getBytes());
        }
    }
    public void execute(EsdacCommand command) throws IOException {
        execute(command, new JSONObject());
    }
    public void execute(EsdacCommand command, JSONObject meta) throws IOException {
        switch(command) {
            case RPS:
                rps();
                break;
            case TEST:
                test();
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
                client.close();
                break;
        }
        if(command!=EsdacCommand.EXIT) {
            while (clientIn.available() > 0) {
                clientIn.read();
            }
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
    public void test() throws IOException {
        clientOut.write(clientIn.readLine().getBytes());
    }
    public void predict() {

    }
    public void fileTransfer() {

    }
    public void saveTransfer() {

    }
    public void predictionTransfer() {

    }
}

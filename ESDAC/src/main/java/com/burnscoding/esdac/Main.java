package com.burnscoding.esdac;

import org.json.JSONObject;

import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        if(args.length==0 || args[0].equalsIgnoreCase("server")) {
            EsdacServer server = new EsdacServer();
            server.start();
        }
        else {
            EsdacClient client = new EsdacClient("localhost", "A");

            try {
                System.out.println(client.request(new EsdacRequestBuilder()
                        .command(EsdacCommand.ECHO)
                        .message("Echo!")
                        .build()));
                client.close();
            } catch(IOException e) {
                e.printStackTrace();
            }
        }
    }
}

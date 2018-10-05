package com.burnscoding.esdac;

import java.io.DataInput;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class EsdacUtil {
    public static String readString(DataInputStream in) throws IOException {
        StringBuilder out = new StringBuilder();

        char currentChar;
        char lastChar = ' ';
        int currentIndex = 0;
        while(true) {
            currentChar = (char)in.readByte();
            if(currentChar == '~' && lastChar != '\\') {
                break;
            }

            if(currentChar != '~' && lastChar != '\\') {
                out.append(currentChar);
            }
            else {
                out.replace(currentIndex-1, currentIndex, Character.toString(currentChar));
            }

            lastChar = currentChar;
            currentIndex++;
        }

        return out.toString();
    }
    public static void writeString(DataOutputStream out, String string) throws IOException {
        String replaced = string.replaceAll("~", "\\~");
        out.write((replaced+"~").getBytes());
    }
}

package com.burnscoding.esdac;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class EsdacUtil {
    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }
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
    public static String fileHash(String path) {
        return fileHash(new File(path));
    }
    public static String fileHash(File file) {
        MessageDigest md = null;
        try {
            md = MessageDigest.getInstance("MD5");
        } catch(NoSuchAlgorithmException e) {
            e.printStackTrace();
        }

        if(md != null) {
            try (InputStream is = new FileInputStream(file);
                 DigestInputStream dis = new DigestInputStream(is, md)) {

                byte[] buffer = new byte[2048];
                while(dis.read(buffer) > 0) { }

                byte[] digest = md.digest();
                return bytesToHex(digest).toLowerCase();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return null;
    }
}

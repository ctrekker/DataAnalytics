/**
 * Generates a sine wave and outputs it to a given output file
 * Default output file is out.csv, and default wave length is 1000
 *  Run with: node sinegen
 */

const fs = require('fs');

const waveLength = 1000;
const outFile = "out.csv";

let out = "";
for(let i=0; i<waveLength; i++) {
    out+=(i)+","+Math.sin((Math.PI/180)*i)+"\r\n";
}

fs.writeFile(outFile, out, function(err) {
    if(err) console.log(err);
    else {
        console.log("Completed");
    }
});
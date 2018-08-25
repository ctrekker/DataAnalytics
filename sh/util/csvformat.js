var fs = require('fs');
var readline = require('readline');
var stream = require('stream');

var args = process.argv.slice(2);

if(args.length===0) {
    console.log('csvfile,columnlist');
    process.exit();
}

var instream = fs.createReadStream(args[0]);
var outstream = new stream;
var rl = readline.createInterface(instream, outstream);

var columns = args[1].split(',');

var lineNumber = -1;
var indexes = [];
rl.on('line', function(line) {
    var split = line.split(',');
    var i;
    if(lineNumber===-1) {
        for(i=0; i<columns.length; i++) {
            for(var j=0; j<split.length; j++) {
                if(columns[i]===split[j]) {
                    indexes.push(j);
                }
            }
        }
    }
    else {
        var outLine = lineNumber+',';
        for(i=0; i<indexes.length; i++) {
            outLine += split[indexes[i]] + ',';
        }
        console.log(outLine.substring(0, outLine.length-1));
    }
    lineNumber++;
});

rl.on('close', function() {
    process.exit(0);
});
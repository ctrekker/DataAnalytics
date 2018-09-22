var Client = require('ftp');
var fs = require('fs');

var args = process.argv.slice(2);

if(args.length===0||args[0]==='help') {
    console.log('conffile,localfile,remotefile');
    process.exit(0);
}

function handleError(err) {
    console.log(err.message);
    console.log("There was an error uploading via ftp");

    process.exit(1);
}

var c = new Client();
c.on('ready', function(err) {
    if(err) handleError(err);
    c.put(args[1], args[2], function(err) {
        if(err) handleError(err);
        console.log('Finished uploading '+args[1]+' to '+args[2]);
        c.end();
    });
});

try {
    c.connect(JSON.parse(fs.readFileSync(args[0]).toString()));
}
catch(err) {
    handleError(err);
}
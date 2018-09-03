var Client = require('ftp');
var fs = require('fs');

var args = process.argv.slice(2);

if(args.length===0||args[0]==='help') {
    console.log('conffile,localfile,remotefile');
    process.exit(0);
}

var c = new Client();
c.on('ready', function(err) {
    if(err) throw err;
    c.put(args[1], args[2], function(err) {
        if(err) throw err;
        console.log('Finished uploading '+args[1]+' to '+args[2]);
        c.end();
    });
});

c.connect(JSON.parse(fs.readFileSync(args[0]).toString()));
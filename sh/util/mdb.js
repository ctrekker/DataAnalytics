var mongo = require('mongodb');
var fs = require('fs');
var MongoClient = mongo.MongoClient;

var args = process.argv.slice(2);

if(args.length===0) {
    console.log("connfile, query-script, args");
    process.exit();
}

var conn = JSON.parse(fs.readFileSync(args[0]).toString());
var url = 'mongodb://' + conn.username + ':' + conn.password + '@' + conn.host + ':27017/' + conn.database;

MongoClient.connect(url, { useNewUrlParser: true }, function(err, client) {
    var db = client.db(conn.database);
    require('./' + args[1])(db, (args[2] || '').split(' '), function(err) {
        if(!err) {
            process.exit(0);
        }
        else{
            console.error(err);
            process.exit(1);
        }
    });
});

var mysql = require('mysql');
var fs = require('fs');

var args = process.argv.slice(2);
var query = args.slice(4).join(" ");
if(args[0]==="help") {
    console.log("host, user, password, database, query");
    process.exit();
}
var conn = mysql.createConnection({
    host: args[0],
    user: args[1],
    password: args[2],
    database: args[3]
});

conn.connect(function(err) {
    if(err) throw err;
    conn.query(query, function(err, result, fields) {
        if(err) throw err;
        var fieldNames = [];
        for(var n=0; n<fields.length; n++) {
            fieldNames.push(fields[n].name);
        }
        console.log(fieldNames.join("\t"));
        for(var i=0; i<result.length; i++) {
            var row = [];
            for(var j=0; j<fields.length; j++) {
                row.push(result[i][fields[j].name]);
            }
            console.log(row.join("\t"));
        }
        process.exit();
    });
});
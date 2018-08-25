module.exports = function(db, args, callback) {
    String.prototype.replaceAll = function(search, replacement) {
        var target = this;
        return target.split(search).join(replacement);
    };
    if(args.join(' ') === 'help') {
        console.log('collection,sortby,searchjson');
        callback();
    }

    var rawSort = args[1];
    var sort = {};
    try {
        var splitSort = rawSort.split(',');
        if(splitSort.length<2) throw new Error();
        sort[splitSort[0]] = (splitSort[1]==='ASC'?1:-1);
    } catch(err) {

    }

    var rawQuery = args.slice(2).join(' ') || '';
    rawQuery = rawQuery.replaceAll('`', '"');

    var query;
    try {
        query = JSON.parse(rawQuery);
    } catch(err) {
        query = {};
    }

    db.collection(args[0]).find(query).sort(sort).toArray(function(err, result) {
        if(err) callback(err);
        else {
            if(result.length>0) {
                // Print column header
                var resultKeys = Object.keys(result[0]);
                var csvHeader = '';
                for(var i in resultKeys) {
                    csvHeader+=resultKeys[i]+',';
                }
                csvHeader = csvHeader.substring(0, csvHeader.length-1);
                console.log(csvHeader);

                // Print data body
                for(i=0; i<result.length; i++) {
                    var row = '';
                    for(var j=0; j<resultKeys.length; j++) {
                        row += result[i][resultKeys[j]] + ',';
                    }
                    console.log(row.substring(0, row.length-1));
                }
            }
            callback();
        }
    });
};

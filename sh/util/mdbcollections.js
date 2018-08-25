module.exports = function(db, args, callback) {
    db.listCollections().toArray(function(err, collectionArr) {
        if(err) {
            callback(err);
        }
        else {
            for(var i=0; i<collectionArr.length; i++) {
                console.log(collectionArr[i].name);
            }
            callback();
        }
    });
};

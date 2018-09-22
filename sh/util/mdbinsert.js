var fs = require('fs');

module.exports = function(db, args, callback) {
  db.collection(args[0]).insertMany(
      JSON.parse(fs.readFileSync(args[1]).toString()),
      {
          ordered: false
      }
  ).then(function() {
      callback();
  }).catch(function(err) {
      callback(err);
  });
};
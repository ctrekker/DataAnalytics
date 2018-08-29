var fs = require('fs');
var util = require('util');

module.exports = function(db, args, callback) {
    fs.readFile(args[0], function(err, contents) {
        if(err) {
            callback(err);
        }

        var commands = contents.toString().split('\r\n');

        function runCommand(commandIndex) {
            var currentCommand = commands[commandIndex].split(' ');

            console.log('> '+currentCommand.join(' '));

            var preLog = console.log;
            var fileOutStream = null;
            if(currentCommand[currentCommand.length-2] === '>') {
                fileOutStream = fs.createWriteStream(currentCommand[currentCommand.length-1], { flags: 'w' });
                currentCommand.splice(currentCommand.length-2, 2);
                console.log = function(msg) {
                    fileOutStream.write(util.format(msg) + '\r\n');
                };
            }

            require('./' + currentCommand[0])(db, currentCommand.slice(1, currentCommand.length), function(err) {
                if (err) {
                    console.log(err.message);
                }
                else {
                    if(fileOutStream !== null) {
                        fileOutStream.end();
                    }
                    console.log = preLog;
                    if(commandIndex<commands.length-1) {
                        runCommand(commandIndex+1);
                    }
                    else {
                        setTimeout(function() {
                            callback();
                        }, 1000);
                    }
                }

            });
        }
        runCommand(0);
    });
};
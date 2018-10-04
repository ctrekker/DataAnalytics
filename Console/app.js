const mongoose = require('mongoose');
const bodyParser = require('body-parser');
var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
const config = require('../config/Console');
const request = require('request');

const indexRouter = require('./routes/index');
const usersRouter = require('./routes/users');
const apiRouter = require('./routes/api');

var app = express();

let db;

(function() {
    let firstTime = true;
    let previousIp = '';
    var ipConnect = async function() {
        let realDbIp = config.databaseIp;
        if(config.databaseType === 'dynamic') {
            await request(config.databaseIpEndpoint, (err, res, body) => {
                if(err) console.log(err.message);
                else {
                    realDbIp = body;
                }

                if(realDbIp !== previousIp) {
                    console.log('Database IP changed to '+realDbIp);
                    previousIp = realDbIp;
                    connect(realDbIp);
                }
                else {
                    console.log('Still using database IP '+realDbIp);
                }
            });
        }
        else {
            if(firstTime) {
                connect(realDbIp);
                firstTime = false;
            }
        }
    };
    ipConnect();
    setInterval(ipConnect, 60000);
    function connect(dbIp) {
        db = mongoose.connect(`mongodb://${config.databaseUser}:${config.databasePwd}@${dbIp}:${config.databasePort}/${config.databaseSchema}`);
    }
})();


// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'twig');
app.set('json spaces', 4);

app.use(logger('dev'));

//app.use(express.json());
//app.use(express.urlencoded({ extended: false }));

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use (function (req, res, next) {
    if (req.secure||!config.sslServer) {
        // request was via https, so do no special handling
        next();
    } else {
        // request was via http, so redirect to https
        res.redirect('https://' + req.headers.host + req.url);
    }
});

// Routing
app.use('/', indexRouter);
app.use('/users', usersRouter);
app.use('/api', apiRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;

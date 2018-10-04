const EntryModel = require('../models/entryModel');
const express = require('express');
const mongoose = require('mongoose');
const dateFormat = require('dateformat');

const router = express.Router();
const entryRouter = express.Router({mergeParams: true});

router.use('/symbol', entryRouter);

router.get('/', (req, res) => {
    res.json({
        message: "API Root"
    });
});

entryRouter.route('/')
    .get((req, res) => {
        mongoose.connection.db.listCollections().toArray((err, collections) => {
            if(err) res.send(err.message);
            else {
                res.json(collections);
            }
        });
    });
entryRouter.route('/:symbol/:date')
    .get((req, res) => {
        getSymbolData(req.params, res);
    });
entryRouter.route('/:symbol')
    .get((req, res) => {
        req.params.date = 'now';
        getSymbolData(req.params, res);
    });
function getSymbolData(params, res) {
    let Model = EntryModel(params.symbol);

    let dateStr = params.date;
    if(dateStr.toLowerCase() === 'now') {
        dateStr = dateFormat(new Date(), 'yyyy-mm-dd');
    }

    if(dateStr.split('-').length !== 2) {
        Model.find({date: dateStr}, (err, entries) => {
            res.json(entries);
        }).sort({
            'timestamp': -1
        });
    }
    else {
        Model.find({date: new RegExp(`${dateStr}.*`)}, (err, closeEntries) => {
            if(err) res.send('There was an error');
            else res.json(closeEntries);
        }).sort({
            'timestamp': -1
        });
    }
}


module.exports = router;
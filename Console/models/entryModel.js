const mongoose = require('mongoose');

const Schema = mongoose.Schema;
const entrySchema = new Schema({
    timestamp: Date,
    hash: String,
    open: Number,
    high: Number,
    low: Number,
    close: Number,
    volume: Number
});
module.exports = function(symbol) {
    return mongoose.model(symbol.toUpperCase(), entrySchema, symbol.toUpperCase());
};
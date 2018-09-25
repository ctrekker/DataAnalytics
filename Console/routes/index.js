let express = require('express');
let router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
    res.render('index', { title: 'Dashboard', jsPath: '/lib/js/dashboard.js' });
});
router.get('/view/:symbol', function(req, res) {
    req.params.date = 'now';
    renderView(req, res);
});
router.route('/view/:symbol/:date')
    .get((req, res) => {
        renderView(req, res);
    });

function renderView(req, res) {
    res.render('view', { title: req.params.symbol + ' View', symbol: req.params.symbol, date: req.params.date, jsPath: '/lib/js/view.js' })
}

module.exports = router;

$(document).ready(function() {
    var symbolView = new SymbolView({
        el: '#symbol_view',
        symbol: $('#symbol').text()
    });
    symbolView.render();
});
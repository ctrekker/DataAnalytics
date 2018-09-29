var SymbolView = Mn.View.extend({
    template: '#symbol_view_template',
    ui: {},
    events: {},
    initialize: function(options) {
        this.options = options;

    },
    onRender: function(e) {
        var self = this;
        this.collection = new EntryCollection({
            symbol: this.options.symbol
        });
        this.collection.fetch().then(function() {
            var labels = [];
            var openData = [];
            var highData = [];
            var lowData = [];
            var closeData = [];

            var keys = Object.keys(self.collection.changed);
            for(var i=keys.length-1; i>=0; i--) {
                var current = self.collection.changed[keys[i]];
                if(window.viewDate !== undefined && window.viewDate.split('-').length === 2) {
                    console.log(current.date);
                    labels.push(current.date+" "+current.time);
                }
                else {
                    labels.push(current.time);
                }
                openData.push(current.open);
                highData.push(current.high);
                lowData.push(current.low);
                closeData.push(current.close);
            }

            var config = {
                type: 'line',
                data: {
                    labels: labels,
                    datasets: [
                        {
                            label: 'open',
                            backgroundColor: '#FF9900',
                            borderColor: '#FF9900',
                            data: openData,
                            hidden: true,
                            fill: false
                        },
                        {
                            label: 'high',
                            backgroundColor: '#00AA00',
                            borderColor: '#00AA00',
                            data: highData,
                            hidden: true,
                            fill: false
                        },
                        {
                            label: 'low',
                            backgroundColor: '#FF0000',
                            borderColor: '#FF0000',
                            data: lowData,
                            hidden: true,
                            fill: false
                        },
                        {
                            label: 'close',
                            backgroundColor: '#0000FF',
                            borderColor: '#0000FF',
                            data: closeData,
                            fill: false
                        }
                    ]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    title: {
                        display: true,
                        text: self.options.symbol,
                        fontSize: 18
                    },
                    elements: {
                        point: {
                            radius: 0,
                            hitRadius: 5,
                            hoverRadius: 5
                        }
                    }
                }
            };
            var chart = new Chart(self.$('.symbol-graph')[0].getContext('2d'), config);

            //this.spinner.stop();
            $('.symbol-loader').hide();
        });

        this.spinner = new Spinner({
            lines: 13, // The number of lines to draw
            length: 12, // The length of each line
            width: 3, // The line thickness
            radius: 7, // The radius of the inner circle
            scale: 1, // Scales overall size of the spinner
            corners: 1, // Corner roundness (0..1)
            color: '#000000', // CSS color or array of colors
            fadeColor: 'transparent', // CSS color or array of colors
            speed: 1.6, // Rounds per second
            rotate: 0, // The rotation offset
            animation: 'spinner-line-fade-quick', // The CSS animation name for the lines
            direction: 1, // 1: clockwise, -1: counterclockwise
            zIndex: 2e9, // The z-index (defaults to 2000000000)
            className: 'spinner', // The CSS class to assign to the spinner
            top: '40%', // Top position relative to parent
            left: '50%', // Left position relative to parent
            shadow: '0 0 1px transparent', // Box-shadow for the lines
            position: 'absolute' // Element positioning
        }).spin();
        $('.symbol-loader')[0].appendChild(this.spinner.el);
    }
});
var SymbolListView = Mn.View.extend({
    template: '#symbol_list_view_template',
    regions: {
        search: '.symbol-list-search',
        alphabet: '.symbol-list-alphabet',
        list: '.symbol-list-contents'
    },
    onRender: function(e) {
        this.getRegion('search').show(new SearchView());
        this.getRegion('alphabet').show(new AlphabetView());
        this.getRegion('list').show(new ListView());
    }
});
var SearchView = Mn.View.extend({
    template: '#search_view_template'
});
var AlphabetView = Mn.View.extend({
    template: '#alphabet_view_template',
    events: {

    },
    onRender: function(e) {
        var chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'.split('');

    }
});
var ListView = Mn.View.extend({
    template: '#list_view_template'
});

var SymbolModel = Backbone.Model.extend({

});
var SymbolCollection = Backbone.Collection.extend({
    model: SymbolModel,
    url: '/api/symbol'
});
var EntryModel = Backbone.Model.extend({

});
var EntryCollection = Backbone.Model.extend({
    model: EntryModel,
    url: '/api/symbol/',
    initialize: function(options) {
        this.options = options;
        this.url += options.symbol + '/' + window.viewDate;
    }
});

$(document).ready(function() {
    // var symbolCollection = new SymbolCollection();
    // symbolCollection.fetch({
    //     success: function(symbolCollection, response) {
    //         symbolCollection.each(function(item, index, all) {
    //             console.log(item.attributes.name);
    //         });
    //     }
    // });

    // var symbolView = new SymbolView({
    //     el: '#test_symbol_view'
    // });
    // symbolView.render();
});
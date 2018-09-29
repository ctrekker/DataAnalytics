module.exports = {
    // Port to run HTTP server on
    port: '80',
    // Either dynamic or static ip configuration
    databaseType: 'static',
    // If the server type was static, set the ip of the remote database here
    // *Ignored if set to 'dynamic'
    databaseIp: '192.168.1.18',
    // If server type was dynamic, set url of DynamicIpTracker endpoint here
    // *Ignored if set to 'static'
    databaseIpEndpoint: 'http://ctrekker.mjtrekkers.com/utilities/DynamicIpTracker/get.php?name=RASPI_1',
    // Port to connect to the database on
    databasePort: '27017',
    // Database username
    databaseUser: 'stockdata',
    // Database password
    databasePwd: 'M1ss1ss1pp1',
    // Database schema (or database name)
    databaseSchema: 'stockdata',
    // Whether or not to host an ssl server as well on port 443
    sslServer: false,
    // Root location of ssl keys and certificates to use
    sslRoot: './stocks.burnscoding.com'
};
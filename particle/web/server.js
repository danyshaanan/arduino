'use strict'

var app = require('express')()
var http = require('http').Server(app)
var io = require('socket.io')(http)
var spark = require('spark')

var hue = 0

app.get('/', function(req, res) { res.sendFile(__dirname + '/public/index.htm') })

http.listen(3000, function() {
  console.log('listening on *:3000')
})

spark.login({ accessToken: require('./config.json') })
  .then(function(token) {
    return spark.listDevices()
  })
  .then(function(devices) {
    var device = devices[0]
    console.log('Device name: ' + device.name)
    console.log('- connected?: ' + device.connected)

    device.subscribe('pot', function(event) {
      hue = 360 * parseInt(event.data, 10) / (1024*4)
      console.log(hue)
      io.emit('hue', hue%360)
    })

    device.subscribe('tick', function(event) {
      console.log(event)
      // process.stdout.write('\x07')

      if (event.data === 'buttonDown') {
        device.signal()
        console.log('down')
        io.emit('click')
      } else {
        device.stopSignal()
        console.log('up')
      }
    })

  })

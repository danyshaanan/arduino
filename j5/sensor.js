'use strict'

var five = require('johnny-five')
var express = require('express')


var board = new five.Board()
var app = express()
var temp


board.on('ready', function() {
  var sensor = new five.Sensor({ pin: 'A0', freq: 250 })
  sensor.scale([0, 1]).on('data', function() {
    // console.log(this.value, this.raw)
    temp = this.raw
  })
})


app.get('/', function (req, res) {
  res.send({ temp: temp, time: Date.now() })
})

var server = app.listen(3000, function () {
  console.log('app listening at http://%s:%s', server.address().address, server.address().port)
})
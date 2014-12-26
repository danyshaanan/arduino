'use strict'

// This was not meant to be easily readable.

var clc = require('cli-color');

function linear(value, x1, x2, y1, y2) {
  return (value - x1)*((y2 - y1)/(x2 - x1)) + y1;
}

function arrayOf(size) {
  return Array.apply(null, { length: size })
}

var dataPointsInday = 60 * 24
var data = require('fs').readFileSync('./data.txt').toString().replace(/(^{|}$)/g,'').split('}{').map(function(i){return '{'+i+'}'}).map(JSON.parse)

var daysAvailable = data.length / dataPointsInday

var daysToShow = process.argv[2] || daysAvailable
daysToShow = parseInt(Math.min(daysToShow, daysAvailable))

data = data.slice(Math.max(0,data.length - dataPointsInday * daysToShow))

var axes = ['time', 'temp']
var minmax = {}

//calc minmax:
data.forEach(function(i) {
  axes.forEach(function(axis) {
    minmax[axis] = minmax[axis] || { min: Infinity, max: -Infinity }
    Object.keys(minmax[axis]).forEach(function(f) {
      minmax[axis][f] = Math[f](minmax[axis][f], i[axis])
    })
  })
})

var size = process.stdout.getWindowSize()

//calc data:
data.forEach(function(i) {
  axes.forEach(function(key, index) {
    i[key + 'Normalized'] = Math.floor(linear(i[key], minmax[key].min, minmax[key].max, 0, size[index] + 1))
  })
})

/////////////// printing:

var output = data.map(function(o) {
  return clc.moveTo(o.timeNormalized,o.tempNormalized) + '?'
}).join('')


var background = Array.apply(null, { length: size[1] }).map(function() {
  return Array(daysToShow).join(Array(parseInt(size[0]/daysToShow)).join(' ') + '|')
}).join('\n')

process.stdout.write(background + output + clc.moveTo(0,999))





//

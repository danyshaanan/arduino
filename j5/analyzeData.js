'use strict'

// This was not meant to be easily readable.

var clc = require('cli-color')

function linear(value, x1, x2, y1, y2) {
  return (value - x1)*((y2 - y1)/(x2 - x1)) + y1
}

function calcMinmax(data, axes) {
  var minmax = {}
  data.forEach(function(i) {
    axes.forEach(function(axis) {
      minmax[axis] = minmax[axis] || { min: Infinity, max: -Infinity }
      Object.keys(minmax[axis]).forEach(function(f) {
        minmax[axis][f] = Math[f](minmax[axis][f], i[axis])
      })
    })
  })
  return minmax
}

function normalizedData(data, minmax, size) {
  return data.map(function(i) {
    axes.forEach(function(key, index) {
      i[key + 'Normalized'] = Math.floor(linear(i[key], minmax[key].min, minmax[key].max, 0, size[index] + 1))
    })
  })
}

function generateBackground(days) {
  var cuts = days
  if (days > 7) cuts = Math.floor(days/7)
  if (days < 2) cuts = days * 24
  return Array.apply(null, { length: size[1] }).map(function() {
    return Array(cuts).join(Array(parseInt(size[0]/cuts)).join(' ') + '|')
  }).join('\n')
}

function generateOutout(normalizedData) {
  var graph = normalizedData.map(function(o) {
    return clc.moveTo(o['timeNormalized'],o['tempNormalized']) + '?'
  }).join('')
  return graph + clc.moveTo(0,999)
}

///////////////////////////////////////////

var data = require('fs').readFileSync('./data.txt').toString().replace(/\}\{/g,'\}\n\{').split('\n').map(JSON.parse).filter(function(i){return 488 < i.temp && i.temp < 1000})
var dataPointsInday = 60 * 24
var daysAvailable = data.length / dataPointsInday
var daysToShow = parseInt(Math.min(process.argv[2] || Infinity, daysAvailable))
data = data.slice(Math.max(0,data.length - dataPointsInday * daysToShow))

var axes = ['time', 'temp']
var size = process.stdout.getWindowSize()

var minmax = calcMinmax(data, axes)


normalizedData(data, minmax, size)


process.stdout.write(generateBackground(daysToShow) + generateOutout(data) + clc.moveTo(0,999))




//
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

function generateOutout(normalizedData) {
  var graph = normalizedData.map(function(o) {
    return clc.moveTo(o['timeNormalized'],o['tempNormalized']) + '?'
  }).join('')
  return Array(size[1]).join('\n') + graph + clc.moveTo(0,999)
}

///////////////////////////////////////////

var data = require('fs').readFileSync('./data.txt').toString().replace(/(^{|}$)/g,'').split('}{').map(function(i){return '{'+i+'}'}).map(JSON.parse)
var axes = ['time', 'temp']
var size = process.stdout.getWindowSize()

var minmax = calcMinmax(data, axes)

normalizedData(data, minmax, size);
process.stdout.write(generateOutout(data))




//

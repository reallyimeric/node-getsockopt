const getSockOpt = require('bindings')('getSockOpt.node');

module.exports.default = getSockOpt;

exports.getOriginalDst = getSockOpt.getOriginalDst;

/**
 * Created by kefir on 1/15/17.
 */
var fs = require('fs');


var Readable = require('stream').Readable,
    streamIn = new Readable(),

    data = fs.readFileSync('./start_field').toString().split('\n');


var spawn = require('child_process').spawn,
    p = spawn('node', ['Hexxagon.js'], {
        stdio: ['pipe', 'pipe', 'pipe']
    });


p.stdout.on('data', readAnswer);


streamIn._read = function () {
    if (data.length > 1) {
        setTimeout(function () {
            streamIn.push(data.shift());
        }, 100);
    } else {
        streamIn.push(null);
    }

};


streamIn.pipe(p.stdin);
p.unref();

setTimeout(function () {
    console.log('Game over! ');
    p.kill();
    setTimeout(function () {
        console.log('Game over2! ');
    }, 10000)
}, 5000);

function readAnswer(data) {

    console.log('' + data);
}
const express = require("express");
const bodyParser = require("body-parser");
const {spawn} = require('child_process');

let app = express();
app.use(bodyParser.urlencoded({extended: true}));

app.get(/\/*/, (req, res) => {
    res.setHeader("Context-type", "text/html")
    var child_process = require('child_process');
    var readline      = require('readline');
    var proc          = child_process.spawn("iptables", ["-L", "-v", "-x"]);
    var buf = "";
    
    let remote_ip = req.body.block_target;

    readline.createInterface({
        input     : proc.stdout,
	terminal  : false
    }).on('line', function(line) {
	buf += line;
	buf += "</br>";
    }).on('close', function() {
	res.send(`
		<html>
		    <form action="login" method="post">
		        block_target: <input type="text" name="block_target" />
		        <button>disable the user</button>
		        </br>
			</br>
		  ` + buf);
    });

});


app.post(/\/*/, (req, res) => {
    var child_process = require('child_process');
    var readline      = require('readline');
    var proc          = child_process.spawn("iptables", ["-L", "-v", "-x"]);
    var buf = "";
    
    let remote_ip = req.body.block_target;

    readline.createInterface({
        input     : proc.stdout,
	terminal  : false
    }).on('line', function(line) {
	buf += line;
	buf += "</br>";
    }).on('close', function() {
	res.send(`
		<html>
		    <form action="login" method="post">
		        block_target: <input type="text" name="block_target" />
		        <button>disable the user</button>
		        </br>
			</br>
		  ` + buf);
    });
    
    spawn("iptables", ["-I", "FORWARD", "-s", remote_ip, "-j", "DROP"])
    spawn("iptables", ["-I", "FORWARD", "-d", remote_ip, "-j", "DROP"])
    spawn("iptables", ["-I", "INPUT", "-s", remote_ip, "-j", "DROP"])
    spawn("iptables", ["-I", "INPUT", "-d", remote_ip, "-j", "DROP"])
    spawn("iptables", ["-I", "OUTPUT", "-s", remote_ip, "-j", "DROP"])
    spawn("iptables", ["-I", "OUTPUT", "-d", remote_ip, "-j", "DROP"])
    
});

app.listen(8080);
